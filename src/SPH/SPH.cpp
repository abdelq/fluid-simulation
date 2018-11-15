#include "SPH.h"
#include <cmath>

SPH::SPH( AbstractObject* parent, const Geometry& container, float smoothingRadius, float viscosity, float pressure, float surfaceTension,
          unsigned int nbCellX, unsigned int nbCellY, unsigned int nbCellZ, unsigned int nbCubeX,
          unsigned int nbCubeY, unsigned int nbCubeZ, unsigned int nbParticles, float restDensity,
          float totalVolume, float maxDTime, const QVector3D& gravity )
    : AbstractObject( parent )
    , _container( container )
    , _coeffPoly6( 0 )
    , _coeffSpiky( 0 )
    , _coeffVisc( 0 )
    , _restDensity( restDensity )
    , _smoothingRadius( smoothingRadius )
    , _smoothingRadius2( smoothingRadius * smoothingRadius )
    , _viscosity( viscosity )
    , _pressure( pressure )
    , _surfaceTension( surfaceTension )
    , _maxDeltaTime( maxDTime )
    , _gravity( gravity )
    , _particles( nbParticles )
    , _grid( inflatedContainerBoundingBox(), nbCellX, nbCellY, nbCellZ, smoothingRadius )
    , _marchingTetrahedra( inflatedContainerBoundingBox(), nbCubeX, nbCubeY, nbCubeZ )
    , _renderMode( RenderParticles )
    , _material( QColor( 0, 125, 200, 255 ) )
{
    initializeCoefficients();
    initializeParticles( totalVolume );
}

SPH::~SPH()
{
}

void SPH::animate( const TimeState& timeState )
{
    float deltaTime = timeState.deltaTime();

    // Clamp 'dt' to avoid instabilities
    if ( deltaTime > _maxDeltaTime )
        deltaTime = _maxDeltaTime;

    computeDensities();
    computeForces();
    moveParticles( deltaTime );
}

void SPH::render( GLShader& shader )
{
    shader.setMaterial( _material );

    switch( _renderMode )
    {
        case RenderParticles : _particles.render( globalTransformation(), shader ); break;
        case RenderImplicitSurface : _marchingTetrahedra.render( globalTransformation(), shader, *this ); break;
    }
}

void SPH::changeRenderMode()
{
    if ( _renderMode == RenderParticles )
        _renderMode = RenderImplicitSurface;
    else
        _renderMode = RenderParticles;
}

void SPH::changeMaterial()
{
    _material = Material( QColor( 0, 0, 255, 255 ) );
}

void SPH::resetVelocities()
{
    for ( int i=0 ; i<_particles.size() ; ++i )
        _particles[i].setVelocity( QVector3D() );
}

BoundingBox SPH::inflatedContainerBoundingBox() const
{
    BoundingBox boundingBox = _container.boundingBox();
    QVector3D center = ( boundingBox.minimum() + boundingBox.maximum() ) * 0.5;

    return BoundingBox( ( boundingBox.minimum() - center ) * 1.2 + center,
                        ( boundingBox.maximum() - center ) * 1.2 + center );
}

void SPH::initializeCoefficients()
{
	// H^n
    float h6 = _smoothingRadius2 * _smoothingRadius2 * _smoothingRadius2;
    float h9 = h6 * _smoothingRadius2 * _smoothingRadius;

	// Poly6
    _coeffPoly6 = 315.0 / ( 64.0 * M_PI * h9 );

	// Spiky
    _coeffSpiky = 3.0 * 15.0 / ( M_PI * h6 );

	// Visc
    _coeffVisc = 45.0 / ( M_PI * h6 );
}

void SPH::initializeParticles( float totalVolume )
{
    float totalMass = totalVolume * _restDensity;
    float mass = totalMass / _particles.size();

    // set particle position and mass
    for ( int i=0 ; i<_particles.size() ; ++i )
	{
        _particles[i].setMass( mass );
        _particles[i].setDensity( _restDensity );
        _particles[i].setVolume( mass / _restDensity );
        _particles[i].setPosition( _container.randomInteriorPoint() );
        _particles[i].setCellIndex( _grid.cellIndex( _particles[i].position() ) );

        _grid.addParticle( _particles[i].cellIndex(), i );
    }
}

float SPH::densityKernel( float r2 ) const
{
    float diff = _smoothingRadius2 - r2;

    return _coeffPoly6 * diff * diff * diff;
}

float SPH::densitykernelGradient( float r2 ) const
{
    float diff = _smoothingRadius2 - r2;

    return -3 * _coeffPoly6 * diff * diff;
}

float SPH::pressureKernel( float r ) const
{
    if ( r == 0 )
		return 0;

    float diff = _smoothingRadius - r;

    return _coeffSpiky * diff * diff / r;
}

float SPH::viscosityKernel( float r ) const
{
    return _coeffVisc * ( _smoothingRadius - r );
}

float SPH::pressure( float density ) const
{
    return density / _restDensity - 1;
}

void SPH::computeDensities()
{
    // For each particle
    #pragma omp parallel for schedule( guided )
    for ( int i=0 ; i<_particles.size() ; ++i )
	{
        float density = 0;
        float correction = 0;
        Particle& particle = _particles[i];
        const QVector<unsigned int>& neighborhood = _grid.neighborhood( particle.cellIndex() );

		// For each neighbor cell
        for ( int j=0 ; j<neighborhood.size() ; ++j )
		{
            const QVector<unsigned int>& neighbors = _grid.cellParticles( neighborhood[j] );

			// For each particle in a neighboring cell
            for ( int k=0 ; k<neighbors.size() ; ++k )
			{
                const Particle& neighbor = _particles[neighbors[k]];
                QVector3D difference = particle.position() - neighbor.position();
                float r2 = difference.lengthSquared();

				// If the neighboring particle is inside a sphere of radius 'h'
                if ( r2 < _smoothingRadius2 )
				{
					// Add density contribution
                    float kernelMass = densityKernel( r2 ) * neighbor.mass();
                    density += kernelMass;
                    correction += kernelMass / neighbor.density();
				}
			}
		}

        particle.setDensity( density / correction );
        particle.setVolume( particle.mass() / particle.density() );
        particle.setPressure( pressure( density ) );
    }
}

void SPH::computeForces()
{
	// Compute gravity vector
    QVector3D gravity = localTransformation().inverted().mapVector( _gravity );

    // For each particle
    #pragma omp parallel for schedule( guided )
    for ( int i=0 ; i<_particles.size() ; ++i )
	{
        QVector3D pressureForce;
        QVector3D viscosityForce;
        QVector3D tensionForce;
        float correction = 0;

        Particle& particle = _particles[i];
        const QVector<unsigned int>& neighborhood = _grid.neighborhood( particle.cellIndex() );

		// For each neighbor cell
        for ( int j=0 ; j<neighborhood.size() ; ++j )
		{
            const QVector<unsigned int>& neighbors = _grid.cellParticles( neighborhood[j] );

			// For each particle in a neighboring cell
            for ( int k=0 ; k<neighbors.size() ; ++k )
			{
                const Particle& neighbor = _particles[neighbors[k]];
                QVector3D difference = particle.position() - neighbor.position();
                float r2 = difference.lengthSquared();

				// If the neighboring particle is inside a sphere of radius 'h'
                if ( r2 < _smoothingRadius2 )
				{
                    float r = ::sqrt( r2 );
                    float volume = neighbor.volume();
                    float meanPressure = ( neighbor.pressure() + particle.pressure() ) * 0.5;

					// Add forces contribution
                    pressureForce -= difference * ( pressureKernel( r ) * meanPressure * volume );
                    viscosityForce += ( neighbor.velocity() - particle.velocity() ) * ( viscosityKernel( r ) * volume );

                    float kernelRR = densityKernel( r2 );
                    tensionForce += difference * kernelRR; // * Mass_b / Mass_a, but in our case, this equals 1
                    correction += kernelRR * volume;
				}
			}
		}

		// Normalize results and apply uniform coefficients;
        pressureForce *= _pressure / correction;
        viscosityForce *= _viscosity / correction;
        tensionForce *= _surfaceTension / correction;

        // Compute the sum of all forces and convert it to an acceleration
        particle.setAcceleration( ( viscosityForce - pressureForce - tensionForce ) / particle.density() + gravity );
    }
}

void SPH::moveParticles(float deltaTime) {
    const float epsilon = 2e-3f; // XXX

    // Mettre à jour la vitesse et la position de chaque particule à l'aide de la méthode d'intégration
    // semi-explicite d'Euler, en traitant correctement les intersections avec la paroi (_container).

    #pragma omp parallel for schedule(guided)
    for (int i = 0; i < _particles.size(); ++i) {
        QVector3D currPos = _particles[i].position();
        QVector3D nextVel = _particles[i].velocity() + deltaTime * _particles[i].acceleration();
        QVector3D nextPos = currPos + deltaTime * nextVel;

        QVector3D remMove = nextPos - currPos;
        QVector3D dirMove = remMove.normalized();

        Intersection inter;
        while (_container.intersect(Ray(currPos, dirMove), inter) &&
               remMove.length() > (inter.rayParameterT() * dirMove).length()) {
            remMove = nextPos - inter.position();

            currPos = inter.position() - epsilon * inter.normal();
            nextVel -= QVector3D::dotProduct(nextVel, inter.normal()) * inter.normal();
            nextPos -= (QVector3D::dotProduct(remMove, inter.normal()) + epsilon) * inter.normal();

            dirMove = (nextPos - currPos).normalized();
        }

        _particles[i].setVelocity(nextVel);
        _particles[i].setPosition(nextPos);
    }

    // Vérifiez si la particule a changé de cellule de la grille régulière (classe Grid). Si c'est le cas
    // changez-la de cellule (méthodes 'removeParticle' et 'addParticle' avant de mettre à jour son index).

    for (int i = 0; i < _particles.size(); ++i) {
        unsigned int currCell = _particles[i].cellIndex();
        unsigned int nextCell = _grid.cellIndex(_particles[i].position());

        if (currCell != nextCell) {
            _particles[i].setCellIndex(nextCell);

            _grid.removeParticle(currCell, unsigned(i));
            _grid.addParticle(nextCell, unsigned(i));
        }
    }
}

void SPH::surfaceInfo(const QVector3D& position, float& value, QVector3D& normal) {
    // Calculez la valeur de la fonction 'f' ainsi que l'approximation de la normale à
    // la surface au point 'position'. Cette fonction est appelée par la classe
    // 'MarchingTetrahedra' à chaque sommet de sa grille. Inspirez-vous de la fonction
    // 'computeDensities' pour savoir comment accéder aux particules voisines.

    float density = 0;
    QVector3D gradient = QVector3D();

    for (unsigned int neighborhood : _grid.neighborhood(_grid.cellIndex(position))) {
        for (unsigned int neighbor : _grid.cellParticles(neighborhood)) {
            Particle& particle = _particles[int(neighbor)];
            QVector3D diffPos = position - particle.position();

            float r2 = diffPos.lengthSquared();
            if (r2 < _smoothingRadius2) {
                density += particle.mass() * densityKernel(r2);
                gradient -= particle.mass() * densitykernelGradient(r2) * diffPos;
            }
        }
    }

    value = density / _restDensity - (1 - .3f);
    normal = (2 * gradient / _restDensity).normalized();
}

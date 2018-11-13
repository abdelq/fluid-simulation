#ifndef SPH_H
#define SPH_H

#include "Geometry/Geometry.h"
#include "Geometry/ImplicitSurface.h"
#include "Geometry/MarchingTetrahedra.h"
#include "SPH/Particles.h"
#include "SPH/Grid.h"
#include "TimeState.h"

/* SPH is responsible for animating the particles and rendering the fluid given a
 * rendering method ( particles or marhcing tetrahedra ).
 *
 * See M. Müller, D. Charypar et M. Gross. 2003
 *     Particle-based fluid simulation for interactive applications.
 */

class SPH : public AbstractObject, public ImplicitSurface
{
public:
    SPH( AbstractObject* parent, const Geometry& container, float smoothingRadius, float viscosity, float pressure, float surfaceTension,
         unsigned int nbCellX, unsigned int nbCellY, unsigned int nbCellZ, unsigned int nbCubeX,
         unsigned int nbCubeY, unsigned int nbCubeZ, unsigned int nbParticles, float restDensity,
         float totalVolume, float maxDTime, const QVector3D& gravity );
    virtual ~SPH();

    virtual void animate( const TimeState& timeState );
    virtual void render( GLShader& shader );

    void changeRenderMode();
    void changeMaterial();
    void resetVelocities();

private:
	// Pre-computations
    BoundingBox inflatedContainerBoundingBox() const;
    void initializeCoefficients();
    void initializeParticles( float totalVolume );

	// Kernels and pressure fonction
    float densityKernel( float r2 ) const;
    float densitykernelGradient( float r2 ) const;
    float pressureKernel( float r ) const;
    float viscosityKernel( float r ) const;
    float pressure( float density ) const;

	// Animation steps
    void computeDensities();
    void computeForces();
    void moveParticles( float deltaTime );

    // Marching tetrahedra rendering
    virtual void surfaceInfo( const QVector3D& position, float& value, QVector3D& normal );

private:
    const Geometry& _container;

	// Pre-computations
    float _coeffPoly6;
    float _coeffSpiky;
    float _coeffVisc;
    float _restDensity;

	// Animation global properties
    float _smoothingRadius;
    float _smoothingRadius2;
    float _viscosity;
    float _pressure;
    float _surfaceTension;
    float _maxDeltaTime;
    QVector3D _gravity;

	// Particles and cells
    Particles _particles;
    Grid _grid;
    MarchingTetrahedra _marchingTetrahedra;

    // Rendering
    enum RenderMode { RenderParticles, RenderImplicitSurface };
    RenderMode _renderMode;
    Material _material;
};

#endif //SPH_H

#include "Cube.h"
#include <cassert>
#include <cstdlib>
#include <limits>

namespace
{
    float side = 0.5;
}

Cube::Cube( AbstractObject* parent, const Material& material )
    : Geometry( parent, material )
{
}

void Cube::createOpenGLBuffers()
{
    createVertices();
    createNormals();
    createIndices();
}

void Cube::createVertices()
{
    QVector<QVector3D> vertices;

    // Face -x
    vertices.append( QVector3D( -side, -side, -side ) );
    vertices.append( QVector3D( -side, -side,  side ) );
    vertices.append( QVector3D( -side,  side,  side ) );
    vertices.append( QVector3D( -side,  side, -side ) );

    // Face +x
    vertices.append( QVector3D(  side, -side, -side ) );
    vertices.append( QVector3D(  side,  side, -side ) );
    vertices.append( QVector3D(  side,  side,  side ) );
    vertices.append( QVector3D(  side, -side,  side ) );

    // Face -y
    vertices.append( QVector3D( -side, -side, -side ) );
    vertices.append( QVector3D(  side, -side, -side ) );
    vertices.append( QVector3D(  side, -side,  side ) );
    vertices.append( QVector3D( -side, -side,  side ) );

    // Face +y
    vertices.append( QVector3D( -side,  side, -side ) );
    vertices.append( QVector3D( -side,  side,  side ) );
    vertices.append( QVector3D(  side,  side,  side ) );
    vertices.append( QVector3D(  side,  side, -side ) );

    // Face -z
    vertices.append( QVector3D( -side, -side, -side ) );
    vertices.append( QVector3D( -side,  side, -side ) );
    vertices.append( QVector3D(  side,  side, -side ) );
    vertices.append( QVector3D(  side, -side, -side ) );

    // Face +z
    vertices.append( QVector3D( -side, -side,  side ) );
    vertices.append( QVector3D(  side, -side,  side ) );
    vertices.append( QVector3D(  side,  side,  side ) );
    vertices.append( QVector3D( -side,  side,  side ) );

    createVertexBuffer( vertices );
}

void Cube::createNormals()
{
    QVector<QVector3D> normals;

    // Face -x
    normals.append( QVector3D( -1,  0,  0 ) );
    normals.append( QVector3D( -1,  0,  0 ) );
    normals.append( QVector3D( -1,  0,  0 ) );
    normals.append( QVector3D( -1,  0,  0 ) );

    // Face +x
    normals.append( QVector3D(  1,  0,  0 ) );
    normals.append( QVector3D(  1,  0,  0 ) );
    normals.append( QVector3D(  1,  0,  0 ) );
    normals.append( QVector3D(  1,  0,  0 ) );

    // Face -y
    normals.append( QVector3D(  0, -1,  0 ) );
    normals.append( QVector3D(  0, -1,  0 ) );
    normals.append( QVector3D(  0, -1,  0 ) );
    normals.append( QVector3D(  0, -1,  0 ) );

    // Face +y
    normals.append( QVector3D(  0,  1,  0 ) );
    normals.append( QVector3D(  0,  1,  0 ) );
    normals.append( QVector3D(  0,  1,  0 ) );
    normals.append( QVector3D(  0,  1,  0 ) );

    // Face -z
    normals.append( QVector3D(  0,  0, -1 ) );
    normals.append( QVector3D(  0,  0, -1 ) );
    normals.append( QVector3D(  0,  0, -1 ) );
    normals.append( QVector3D(  0,  0, -1 ) );

    // Face +z
    normals.append( QVector3D(  0,  0,  1 ) );
    normals.append( QVector3D(  0,  0,  1 ) );
    normals.append( QVector3D(  0,  0,  1 ) );
    normals.append( QVector3D(  0,  0,  1 ) );

    createNormalBuffer( normals );
}

void Cube::createIndices()
{
    QVector<unsigned int> indices;

    for ( unsigned int i=0, base=0 ; i<6 ; ++i, base+=4 )
    {
        // Traingle 1
        indices.append( base + 0 );
        indices.append( base + 1 );
        indices.append( base + 2 );

        // Triangle 2
        indices.append( base + 0 );
        indices.append( base + 2 );
        indices.append( base + 3 );
    }

    createIndexBuffer( indices );
}

bool Cube::intersect( const Ray& ray, Intersection& intersection ) const
{
    float nearestT = std::numeric_limits<float>::max();

    for ( unsigned int i=0 ; i<3 ; ++i )
    {
        for ( int j=-1 ; j<=1 ; j+=2 )
        {
            float t;
            Intersection facetIntersection;

            if ( intersectFacet( ray, facetIntersection, i, j, t ) && ( t < nearestT ) )
            {
                nearestT = t;
                intersection = facetIntersection;
            }
        }
    }

    if ( nearestT < std::numeric_limits<float>::max() )
        return true;

    return false;
}

bool Cube::intersectFacet( const Ray& ray, Intersection& intersection, unsigned int coord, int sign, float& t ) const
{
    float oy = vectorCoord( ray.origin(), coord );
    float dy = vectorCoord( ray.direction(), coord );

    if ( dy == 0 )
        return false;

    t = -( oy - sign * side ) / dy;
    float eps = 1e-6;
    QVector3D position = ray.origin() + t * ray.direction();
    float px = vectorCoord( position, (coord+2)%3 );
    float pz = vectorCoord( position, (coord+1)%3 );

    if ( ( t >= 0 ) &&
         ( px >= -side - eps ) && ( px <= side + eps ) &&
         ( pz >= -side - eps ) && ( pz <= side + eps ) )
    {
        QVector3D normal( ( coord == 0 ) ? sign : 0,
                         ( coord == 1 ) ? sign : 0,
                         ( coord == 2 ) ? sign : 0 );
        intersection = Intersection( position, normal, t );

        return true;
    }

    return false;
}

float Cube::vectorCoord( const QVector3D& vector, unsigned int axis ) const
{
    switch( axis )
    {
        case 0 : return vector.x();
        case 1 : return vector.y();
        case 2 : return vector.z();
    }

    assert( !"Should not have reached this point" );
    return 0;
}

BoundingBox Cube::boundingBox() const
{
    return BoundingBox( QVector3D( -0.5, -0.5, -0.5 ), QVector3D( 0.5, 0.5, 0.5 ) );
}

QVector3D Cube::randomInteriorPoint() const
{
    float x = 2 * ( rand() / (float)RAND_MAX ) - 1;
    float y = 2 * ( rand() / (float)RAND_MAX ) - 1;
    float z = 2 * ( rand() / (float)RAND_MAX ) - 1;

    return QVector3D( x, y, z ) * side;
}

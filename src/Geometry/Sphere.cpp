#include "Sphere.h"
#include <cmath>

namespace
{
    static float radius = 0.5;
    static unsigned int nbThetas = 30;
    static unsigned int nbPhis = 30;
}

Sphere::Sphere( AbstractObject* parent, const Material& material )
    : Geometry( parent, material )
{
}

void Sphere::createOpenGLBuffers()
{
    createVerticesNormals();
    createIndices();
}

void Sphere::createVerticesNormals()
{
    QVector<QVector3D> vertices;
    QVector<QVector3D> normals;

    // Bottom vertex
    vertices.append( QVector3D( 0, -radius, 0 ) );
    normals.append( QVector3D( 0, -1, 0 ) );

    // Middle vertices
    for ( unsigned int i=1 ; i<nbPhis ; ++i )
    {
        for ( unsigned int j=0 ; j<nbThetas ; ++j )
        {
            float theta = 2.0 * M_PI * j / nbThetas;
            float phi = M_PI * i / nbThetas;
            normals.append( QVector3D( ::cos( theta ) * ::sin( phi ), -::cos( phi ), ::sin( theta ) * ::sin( phi ) ) );
            vertices.append( normals.back() * radius );
        }
    }

    // Top vertex
    vertices.append( QVector3D( 0, radius, 0 ) );
    normals.append( QVector3D( 0, 1, 0 ) );

    createVertexBuffer( vertices );
    createNormalBuffer( normals );
}

void Sphere::createIndices()
{
    QVector<unsigned int> indices;

    // Bottom cap
    for ( unsigned int i=0 ; i<nbThetas ; ++i )
    {
        indices.append( 0 );
        indices.append( ( i + 1 ) % nbThetas + 1 );
        indices.append( i + 1 );
    }

    // Middle faces
    for ( unsigned int i=0 ; i<nbPhis-2 ; ++i )
    {
        for ( unsigned int j=0 ; j<nbThetas ; ++j )
        {
            // Compute indices
            unsigned int base0 = i * nbThetas + 1;
            unsigned int base1 = ( i + 1 ) * nbThetas + 1;
            unsigned int i00 = base0 + j;
            unsigned int i01 = base0 + ( j + 1 ) % nbThetas;
            unsigned int i10 = base1 + j;
            unsigned int i11 = base1 + ( j + 1 ) % nbThetas;

            // Face 0
            indices.append( i00 );
            indices.append( i01 );
            indices.append( i11 );

            // Face 1
            indices.append( i00 );
            indices.append( i11 );
            indices.append( i10 );
        }
    }

    // Top cap
    unsigned int lastVertex = nbThetas * ( nbPhis - 1 ) + 1;

    for ( unsigned int i=0 ; i<nbThetas ; ++i )
    {
        indices.append(  lastVertex );
        indices.append(  lastVertex - nbThetas + i );
        indices.append(  lastVertex - nbThetas + ( i + 1 ) % nbThetas );
    }

    createIndexBuffer( indices );
}

bool Sphere::intersect( const Ray& ray, Intersection& intersection ) const
{
    float a = ray.direction().lengthSquared();
    float b = 2 * QVector3D::dotProduct( ray.direction(), ray.origin() );
    float c = ray.origin().lengthSquared() - radius * radius;
    float discriminant = b * b - 4 * a * c;
    float epsilon = 1e-3;

    if ( discriminant >= 0 )
    {
        float sqrtDiscriminant = sqrt( discriminant );
        float t1 = ( -b - sqrtDiscriminant ) / ( 2 * a );
        float t2 = ( -b + sqrtDiscriminant ) / ( 2 * a );

        if ( t1 >= -epsilon )
        {
            QVector3D position = ray.origin() + t1 * ray.direction();
            intersection = Intersection( position, position, t1 );
            return true;
        }
        else if ( t2 >= -epsilon )
        {
            QVector3D position = ray.origin() + t2 * ray.direction();
            intersection = Intersection( position, position, t2 );
            return true;
        }
    }

    return false;
}

BoundingBox Sphere::boundingBox() const
{
    return BoundingBox( QVector3D( -radius, -radius, -radius ),
                        QVector3D(  radius,  radius,  radius ) );
}

QVector3D Sphere::randomInteriorPoint() const
{
    while ( true )
    {
        QVector3D result( 2 * ( rand() / (float)RAND_MAX ) - 1,
                          2 * ( rand() / (float)RAND_MAX ) - 1,
                          2 * ( rand() / (float)RAND_MAX ) - 1 );

        if ( result.lengthSquared() < 1 )
            return result * radius;
    }

    return QVector3D();
}

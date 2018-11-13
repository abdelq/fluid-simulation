#include "Cylinder.h"
#include <QVector2D>
#include <cmath>
#include <limits>

namespace
{
    static float radius = 0.5;
    static float side = 0.5;
    static unsigned int nbThetas = 60;
}

Cylinder::Cylinder( AbstractObject* parent, const Material& material )
    : Geometry( parent, material )
{
}

void Cylinder::createOpenGLBuffers()
{
    createVerticesNormals();
    createIndices();
}

void Cylinder::createVerticesNormals()
{
    QVector<QVector3D> vertices;
    QVector<QVector3D> normals;

    // Bottom vertex
    vertices.append( QVector3D( 0, -side, 0 ) );
    normals.append( QVector3D( 0, -1, 0 ) );

    // Bottom loop vertices
    for ( unsigned int i=0 ; i<nbThetas ; ++i )
    {
        float theta = 2.0 * M_PI * i / nbThetas;
        vertices.append( QVector3D( radius * ::cos( theta ), -side, radius * ::sin( theta ) ) );
        normals.append( QVector3D( 0, -1, 0 ) );
    }

    // Middle vertices
    for ( unsigned int i=0 ; i<nbThetas ; ++i )
    {
        float theta = 2.0 * M_PI * i / nbThetas;
        vertices.append( QVector3D( radius * ::cos( theta ), -side, radius * ::sin( theta ) ) );
        vertices.append( QVector3D( radius * ::cos( theta ),  side, radius * ::sin( theta ) ) );
        normals.append(  QVector3D( ::cos( theta ),  0, ::sin( theta ) ) );
        normals.append(  QVector3D( ::cos( theta ),  0, ::sin( theta ) ) );
    }

    // Top loop vertices
    for ( unsigned int i=0 ; i<nbThetas ; ++i )
    {
        float theta = 2.0 * M_PI * i / nbThetas;
        vertices.append( QVector3D( radius * ::cos( theta ), side, radius * ::sin( theta ) ) );
        normals.append( QVector3D( 0, 1, 0 ) );
    }

    // Top vertex
    vertices.append( QVector3D( 0, side, 0 ) );
    normals.append( QVector3D( 0, 1, 0 ) );

    createVertexBuffer( vertices );
    createNormalBuffer( normals );
}

void Cylinder::createIndices()
{
    QVector<unsigned int> indices;

    // Bottom cap
    for ( unsigned int i=0 ; i<nbThetas ; ++i )
    {
        indices.append( 0 );
        indices.append( ( i + 1 ) % nbThetas + 1 );
        indices.append( i + 1 );
    }

    // Middle vertices
    unsigned int offset = nbThetas + 1;

    for ( unsigned int i=0 ; i<nbThetas ; ++i )
    {
        unsigned int i0 = 2 * i;
        unsigned int i1 = 2 * ( ( i + 1 ) % nbThetas );

        // Face 0
        indices.append( offset + i0 + 0 );
        indices.append( offset + i1 + 0 );
        indices.append( offset + i1 + 1 );

        // Face 1
        indices.append( offset + i0 + 0 );
        indices.append( offset + i1 + 1 );
        indices.append( offset + i0 + 1 );
    }

    // Top cap
    unsigned int lastVertex = 4 * nbThetas;
    offset += 2 * nbThetas;

    for ( unsigned int i=0 ; i<nbThetas ; ++i )
    {
        indices.append( lastVertex );
        indices.append( offset + i );
        indices.append( offset + i + 1 );
    }

    createIndexBuffer( indices );
}

bool Cylinder::intersect( const Ray& ray, Intersection& intersection ) const
{
    float eps = 1e-4;
    const QVector3D& p = ray.origin();
    const QVector3D& d = ray.direction();
    float nearestT = std::numeric_limits<float>::max();
    QVector3D position;
    QVector3D normal;

    // Contour
    float a = d.x() * d.x() + d.z() * d.z();
    float b = 2.0 * ( d.x() * p.x() + d.z() * p.z() );
    float c = p.x() * p.x() + p.z() * p.z() - radius * radius;
    float discriminant = b * b - 4 * a * c;

    if ( discriminant >= 0 )
    {
        float t1 = ( -b - sqrt( discriminant ) ) / ( 2 * a );
        float t2 = ( -b + sqrt( discriminant ) ) / ( 2 * a );
        float y1 = p.y() + t1 * d.y();
        float y2 = p.y() + t2 * d.y();

        if ( ( t1 >= -eps ) && ( y1 >= -side - eps ) && ( y1 <= side + eps ) )
        {
            nearestT = t1;
            position = p + t1 * d;
            normal = QVector3D( position.x(), 0, position.z() ) / radius;
        }
        else if ( ( t2 >= -eps ) && ( y2 >= -side - eps ) && ( y2 <= side + eps ) )
        {
            nearestT = t2;
            position = p + t2 * d;
            normal = QVector3D( position.x(), 0, position.z() ) / radius;
        }
    }

    // Caps
    if ( d.y() != 0 )
    {
        // Cap Y=1
        float t = -( p.y() - side ) / d.y();

        if ( t >= 0 && t < nearestT )
        {
            QVector3D testPosition = p + t * d;
            float r = ::sqrt( testPosition.x() * testPosition.x() + testPosition.z() * testPosition.z() );

            if ( r <= radius + eps )
            {
                position = testPosition;
                normal = QVector3D( 0, 1, 0 );
                nearestT = t;
            }
        }

        // Cap Y=-1
        t = -( p.y() + side ) / d.y();

        if ( t >= 0 && t < nearestT )
        {
            QVector3D testPosition = p + t * d;
            float r = ::sqrt( testPosition.x() * testPosition.x() + testPosition.z() * testPosition.z() );

            if ( r <= radius + eps )
            {
                position = testPosition;
                normal = QVector3D( 0, -1, 0 );
                nearestT = t;
            }
        }
    }

    if ( nearestT < std::numeric_limits<float>::max() )
    {
        intersection = Intersection( position, normal, nearestT );
        return true;
    }

    return false;
}

BoundingBox Cylinder::boundingBox() const
{
    return BoundingBox( QVector3D( -radius, -side, -radius ), QVector3D( radius, side, radius ) );
}

QVector3D Cylinder::randomInteriorPoint() const
{
    while ( true )
    {
        QVector3D result( radius * ( 2 * ( rand() / (float)RAND_MAX ) - 1 ),
                          side   * ( 2 * ( rand() / (float)RAND_MAX ) - 1 ),
                          radius * ( 2 * ( rand() / (float)RAND_MAX ) - 1 ) );
        QVector2D xyProjection( result.x(), result.z() );

        if ( xyProjection.length() < radius )
            return result;
    }

    return QVector3D();
}

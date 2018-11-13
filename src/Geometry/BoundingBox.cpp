#include "BoundingBox.h"
#include <limits>

BoundingBox::BoundingBox()
{
}

BoundingBox::BoundingBox( const QVector3D& minimum, const QVector3D& maximum )
    : _minimum( minimum )
    , _maximum( maximum )
{
}

BoundingBox::BoundingBox( const QVector<QVector3D>& vertices )
{
    if ( !vertices.isEmpty() )
    {
        _minimum = vertices[0];
        _maximum = vertices[0];
    }

    for ( int i=1 ; i<vertices.size() ; ++i )
    {
        _minimum.setX( std::min<float>( _minimum.x(), vertices[i].x() ) );
        _minimum.setY( std::min<float>( _minimum.y(), vertices[i].y() ) );
        _minimum.setZ( std::min<float>( _minimum.z(), vertices[i].z() ) );
        _maximum.setX( std::max<float>( _maximum.x(), vertices[i].x() ) );
        _maximum.setY( std::max<float>( _maximum.y(), vertices[i].y() ) );
        _maximum.setZ( std::max<float>( _maximum.z(), vertices[i].z() ) );
    }
}

const QVector3D& BoundingBox::minimum() const
{
    return _minimum;
}

const QVector3D& BoundingBox::maximum() const
{
    return _maximum;
}

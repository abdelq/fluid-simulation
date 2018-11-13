#include "Ray.h"

Ray::Ray()
    : _direction( 1, 0, 0 )
{
}

Ray::Ray( const QVector3D& origin, const QVector3D& direction )
    : _origin( origin )
    , _direction( direction )
{
}

const QVector3D& Ray::origin() const
{
    return _origin;
}

const QVector3D& Ray::direction() const
{
    return _direction;
}

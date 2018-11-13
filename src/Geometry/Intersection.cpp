#include "Intersection.h"

Intersection::Intersection()
{
}

Intersection::Intersection( const QVector3D& position,
                            const QVector3D& normal,
                            float rayParameterT )
    : _position( position )
    , _normal( normal )
    , _rayParameterT( rayParameterT )
{
}

const QVector3D& Intersection::position() const
{
    return _position;
}

const QVector3D& Intersection::normal() const
{
    return _normal;
}

float Intersection::rayParameterT() const
{
    return _rayParameterT;
}

#ifndef INTERSECTION_H
#define INTERSECTION_H

#include "Geometry/Ray.h"
#include <QVector3D>

/* Container for intersection information. The 'rayParameterT' attribute
 * is the parameter of the ray used to compute the intersection such that
 * intersection.position = ray.origin + rayParameterT * ray.direction.
 */

class Intersection
{
public:
    Intersection();
    Intersection( const QVector3D& position,
                  const QVector3D& normal,
                  float rayParameterT );

    const QVector3D& position() const;
    const QVector3D& normal() const;
    float rayParameterT() const;

private:
    QVector3D _position;
    QVector3D _normal;
    float _rayParameterT;
};

#endif // INTERSECTION_H

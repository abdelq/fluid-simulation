#ifndef RAY_H
#define RAY_H

#include <QVector3D>

/* A parametric representation of ray of the form
 * position = origin + t * direction.
 */

class Ray
{
public:
    Ray();
    Ray( const QVector3D& origin, const QVector3D& direction );

    const QVector3D& origin() const;
    const QVector3D& direction() const;

private:
    QVector3D _origin;
    QVector3D _direction;
};

#endif // RAY_H

#ifndef IMPLICITSURFACE_H
#define IMPLICITSURFACE_H

#include <QVector3D>

/* This is an interface that must be implemented so that the marching
 * tetrahedra algorithm can retreive surface information. Given a position,
 * the method 'surfaceInfo' compute the value of implicit function and its
 * gradient ( i.e. the normal ).
 *
 */

class ImplicitSurface
{
public:
    virtual void surfaceInfo( const QVector3D& position, float& value, QVector3D& normal )=0;
};

#endif // IMPLICITSURFACE_H

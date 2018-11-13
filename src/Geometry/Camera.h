#ifndef CAMERA_H
#define CAMERA_H

#include "Geometry/AbstractObject.h"
#include <QVector3D>

/* This is the perspective camera. It uses QMatrix4x4 perspective helper
 * method to create the actual perspective matrix. It inherits from AbstractObject
 * so it can be plugged into a hierarchy and have local/global transformations.
 *
 * The resizeViewport method must be called when resizing screen so the screen size
 * matches the perspective matrix parameters.
 *
 * Use the Camera::lookAt instead of QMatrix4x4::lookAt since Qt return an inverted
 * matrix ready for OpenGL instead of a standard local transformation compatible
 * with the other objects of the hierarchy.
 *
 * The camera is looking in the -Z axis.
 *
 */

class Camera : public AbstractObject
{
public:
    Camera( AbstractObject* parent,
            float fieldOfView = 45.0,
            unsigned int viewportWidth = 800,
            unsigned int viewportHeight = 600,
            float nearClippingPlane = 0.01,
            float farClippingPlane = 200.0 );

    void resizeViewport( unsigned int width, unsigned int height );
    void lookAt( const QVector3D& eye, const QVector3D& center, const QVector3D& up );

    const QMatrix4x4& projectionMatrix() const;

private:
    void buildProjectionMatrix();

private:
    float _fieldOfView;
    unsigned int _viewportWidth;
    unsigned int _viewportHeight;
    float _nearClippingPlane;
    float _farClippingPlane;
    QMatrix4x4 _projectionMatrix;
};

#endif // CAMERA_H

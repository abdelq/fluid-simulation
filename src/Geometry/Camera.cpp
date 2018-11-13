#include "Camera.h"

Camera::Camera( AbstractObject* parent, float fieldOfView,
                unsigned int viewportWidth, unsigned int viewportHeight,
                float nearClippingPlane, float farClippingPlane )
    : AbstractObject( parent )
    , _fieldOfView( fieldOfView )
    , _viewportWidth( viewportWidth )
    , _viewportHeight( viewportHeight )
    , _nearClippingPlane( nearClippingPlane )
    , _farClippingPlane( farClippingPlane )
{
    buildProjectionMatrix();
}

void Camera::resizeViewport( unsigned int width, unsigned int height )
{
    _viewportWidth = width;
    _viewportHeight = height;
    buildProjectionMatrix();
}

void Camera::lookAt( const QVector3D& eye, const QVector3D& center, const QVector3D& up )
{
    QMatrix4x4 local;
    local.lookAt( eye, center, up );
    localTransformation() = local.inverted();
}

const QMatrix4x4& Camera::projectionMatrix() const
{
    return _projectionMatrix;
}

void Camera::buildProjectionMatrix()
{
    _projectionMatrix.setToIdentity();
    _projectionMatrix.perspective( _fieldOfView, (float)_viewportWidth / _viewportHeight, _nearClippingPlane, _farClippingPlane );
}

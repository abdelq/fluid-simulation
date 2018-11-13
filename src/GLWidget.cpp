#include "GLWidget.h"
#include <QKeyEvent>
#include <QApplication>
#include <cmath>

GLWidget::GLWidget( QWidget* parent )
    : QGLWidget( QGLFormat( QGL::DepthBuffer | QGL::DoubleBuffer ), parent )
    , _scene( 0 )
    , _paused( false )
    , _mouseButtons( Qt::NoButton )
    , _moveContainer( false )
{
}

GLWidget::~GLWidget()
{
}

void GLWidget::setScene( Scene* scene )
{
    _scene = scene;
    _scene->resizeViewport( size().width(), size().height() );
}

void GLWidget::onIdle()
{
    updateGL();
}

void GLWidget::initializeGL()
{
    glEnable( GL_DEPTH_TEST );
    glClearColor( 0.0, 0.0, 0.0, 1.0 );

    _shader.initialize();
    _shader.bind();
}

void GLWidget::resizeGL( int width, int height )
{
    glViewport( 0, 0, width, height );
    _scene->resizeViewport( width, height );
}

void GLWidget::paintGL()
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    if ( _scene )
    {
        _timeState.newFrame();
        _scene->update();

        if ( !_paused )
            _scene->animate( _timeState );

        _scene->update();
        _shader.setupCamera( _scene->activeCamera() );
        _scene->render( _shader );
    }
}

void GLWidget::keyPressEvent( QKeyEvent* event )
{
    if ( event->key() == Qt::Key_M )
        _scene->sph().changeRenderMode();

    if ( event->key() == Qt::Key_R )
        _scene->sph().changeMaterial();

    if ( event->key() == Qt::Key_0 )
        _scene->sph().resetVelocities();

    if ( event->key() == Qt::Key_Space )
        _moveContainer = true;

    if ( event->key() == Qt::Key_P )
        _paused = !_paused;
}

void GLWidget::keyReleaseEvent( QKeyEvent* /*event*/ )
{
    _moveContainer = false;
}

void GLWidget::mousePressEvent( QMouseEvent* event )
{
    _mouseButtons |= event->button();
    _mousePosition = QPoint( event->pos() );
}

void GLWidget::mouseReleaseEvent( QMouseEvent* event )
{
    _mouseButtons &= ~event->button();
    _mousePosition = QPoint( event->pos() );
}

void GLWidget::mouseMoveEvent( QMouseEvent* event )
{
    if ( _scene )
    {
        Camera& camera = _scene->activeCamera();
        SPH& sph = _scene->sph();
        QMatrix4x4& cameraTransformation = camera.localTransformation();
        QMatrix4x4& sphTransformation = sph.localTransformation();
        QPoint delta = event->pos() - _mousePosition;
        _mousePosition = event->pos();

        if ( _mouseButtons & Qt::LeftButton )
        {
            QMatrix4x4 rotation;
            QVector3D axisX = cameraTransformation.column( 0 ).toVector3D();
            QVector3D axisY = cameraTransformation.column( 1 ).toVector3D();
            QVector3D axisZ = cameraTransformation.column( 2 ).toVector3D();
            QVector2D center( size().width() * 0.5, size().height() * 0.5 );
            QVector2D centerRelative = QVector2D( _mousePosition ) - center;
            float roll = centerRelative.x() * delta.y() - centerRelative.y() * delta.x();
            roll = ::sqrt( ::fabs( roll ) ) * ( ( roll < 0 ) ? -1 : 1 );
            rotation.rotate( -delta.x() / 5.0, axisY );
            rotation.rotate( -delta.y() / 5.0, axisX );
            rotation.rotate( roll / 35.0, axisZ );

            if ( _moveContainer )
                sphTransformation = rotation.inverted() * sphTransformation;
            else
                cameraTransformation = rotation * cameraTransformation;
        }

        if ( _mouseButtons & Qt::RightButton )
        {
            QVector3D translation = cameraTransformation.column( 3 ).toVector3D();
            cameraTransformation.setColumn( 3, QVector4D( translation * ( 1 + delta.y() / 60.0 ), 1 ) );
        }
    }
}

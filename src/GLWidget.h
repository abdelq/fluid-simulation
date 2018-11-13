#ifndef GL_WIDGET_H
#define GL_WIDGET_H

#include "Scenes/Scene.h"
#include "GLShader.h"
#include "TimeState.h"
#include <QGLWidget>
#include <QLabel>
#include <QTime>

/* The GLWidget displays a scene and move the scene camera on
 * mouse events.
 */

class GLWidget : public QGLWidget
{
    Q_OBJECT

public:
    explicit GLWidget( QWidget *parent = 0 );
    virtual ~GLWidget();

    void setScene( Scene* scene );
    void onIdle();

public slots:
    virtual void paintGL();

protected:
    virtual void initializeGL();
    virtual void resizeGL( int width, int height );
    virtual void keyPressEvent( QKeyEvent* event );
    virtual void keyReleaseEvent( QKeyEvent* event );
    virtual void mousePressEvent( QMouseEvent* event );
    virtual void mouseReleaseEvent( QMouseEvent* event );
    virtual void mouseMoveEvent( QMouseEvent* event );

private:
    GLShader _shader;
    Scene* _scene;
    bool _paused;
    TimeState _timeState;
    Qt::MouseButtons _mouseButtons;
    QPoint _mousePosition;
    bool _moveContainer;
};

#endif // GL_WIDGET_H

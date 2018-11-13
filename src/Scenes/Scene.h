#ifndef SCENE_H
#define SCENE_H

#include "Geometry/Camera.h"
#include "SPH/SPH.h"
#include "TimeState.h"

/* Every scene must implement derive from Scene, and added in
 * MainWindow::buildSceneList() to be visible in the scene list
 * widget.
 */

class Scene : public AbstractObject
{
public:
    Scene();
    virtual ~Scene();

    void resizeViewport( unsigned int width, unsigned int height );
    Camera& activeCamera();

    virtual SPH& sph()=0;

protected:
    Camera _camera;
};

#endif // SCENE_H

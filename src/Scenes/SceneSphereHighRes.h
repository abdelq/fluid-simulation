#ifndef SCENESPHEREHIGHRES_H
#define SCENESPHEREHIGHRES_H

#include "Scene.h"
#include "Geometry/Sphere.h"
#include "SPH/SPH.h"

class SceneSphereHighRes : public Scene
{
public:
    SceneSphereHighRes();
    virtual ~SceneSphereHighRes();

    virtual SPH& sph();

private:
    Sphere _sphere;
    SPH _water;
};

#endif // SCENESPHEREHIGHRES_H

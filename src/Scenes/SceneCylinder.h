#ifndef SCENECYLINDER_H
#define SCENECYLINDER_H

#include "Scene.h"
#include "Geometry/Cylinder.h"
#include "SPH/SPH.h"

class SceneCylinder : public Scene
{
public:
    SceneCylinder();
    virtual ~SceneCylinder();

    virtual SPH& sph();

private:
    Cylinder _cylinder;
    SPH _water;
};

#endif // SCENECYLINDER_H

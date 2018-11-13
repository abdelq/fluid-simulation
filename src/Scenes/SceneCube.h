#ifndef SCENECUBE_H
#define SCENECUBE_H

#include "Scene.h"
#include "Geometry/Cube.h"
#include "SPH/SPH.h"

class SceneCube : public Scene
{
public:
    SceneCube();
    virtual ~SceneCube();

    virtual SPH& sph();

private:
    Cube _cube;
    SPH _water;
};

#endif // SCENECUBE_H

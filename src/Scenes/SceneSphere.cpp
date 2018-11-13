#include "Scenes/SceneSphere.h"

SceneSphere::SceneSphere()
    : _sphere( 0, Material() )
    , _water( this, _sphere,
              0.09, 20, 5000, 0.3,
              20, 20, 20,
              30, 30, 30,
              3000,
              998.29,
              0.5,
              0.01,
              QVector3D( 0, -9.81, 0 ) )
{
    _sphere.setParent( &_water );
    _camera.lookAt( QVector3D(  0,  2, -2 ),
                    QVector3D(  0,  0,  0 ),
                    QVector3D(  0,  1,  0 ) );
}

SceneSphere::~SceneSphere()
{
}

SPH& SceneSphere::sph()
{
    return _water;
}

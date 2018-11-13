#include "Scenes/SceneCylinder.h"

SceneCylinder::SceneCylinder()
    : _cylinder( 0, Material() )
    , _water( this, _cylinder,
              0.09, 20, 5000, 0.3,
              20, 20, 20,
              30, 30, 30,
              3000,
              998.29,
              1,
              0.01,
              QVector3D( 0, -9.81, 0 ) )
{
    _cylinder.setParent( &_water );
    _camera.lookAt( QVector3D(  0,  2, -2 ),
                    QVector3D(  0,  0,  0 ),
                    QVector3D(  0,  1,  0 ) );
}

SceneCylinder::~SceneCylinder()
{
}

SPH& SceneCylinder::sph()
{
    return _water;
}

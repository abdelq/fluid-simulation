#include "Scenes/SceneSphereHighRes.h"

SceneSphereHighRes::SceneSphereHighRes()
    : _sphere( 0, Material() )
    , _water( this, _sphere,
              0.06, 20, 5000, 0.3,
              40, 40, 40,
              80, 80, 80,
              25000,
              998.29,
              0.5,
              0.008,
              QVector3D( 0, -9.81, 0 ) )
{
    _sphere.setParent( &_water );
    _camera.lookAt( QVector3D(  0,  2, -2 ),
                    QVector3D(  0,  0,  0 ),
                    QVector3D(  0,  1,  0 ) );
}

SceneSphereHighRes::~SceneSphereHighRes()
{
}

SPH& SceneSphereHighRes::sph()
{
    return _water;
}

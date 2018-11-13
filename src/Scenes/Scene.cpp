#include "Scene.h"

Scene::Scene()
    : _camera( this )
{
}

Scene::~Scene()
{
}

void Scene::resizeViewport( unsigned int width, unsigned int height )
{
    _camera.resizeViewport( width, height );
}

Camera& Scene::activeCamera()
{
    return _camera;
}

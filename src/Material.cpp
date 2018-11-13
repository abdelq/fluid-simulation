#include "Material.h"

Material::Material()
{
}

Material::Material( const QColor& diffuse )
    : _diffuse( diffuse )
{
}

const QColor& Material::diffuse() const
{
    return _diffuse;
}

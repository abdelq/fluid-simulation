#ifndef MATERIAL_H
#define MATERIAL_H

#include <QColor>

/* A diffuse material
 */

class Material
{
public:
    Material();
    Material( const QColor& diffuse ); // Diffuse constructor

    const QColor& diffuse() const;

private:
    QColor _diffuse;
};

#endif // MATERIAL_H

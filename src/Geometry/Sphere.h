#ifndef SPHERE_H
#define SPHERE_H

#include "Geometry/Geometry.h"

/* A simple sphere S of radius 0.5 that inherits from Geometry and create
 * the geometry on construction
 */

class Sphere : public Geometry
{
public:
    Sphere( AbstractObject* parent, const Material& material );

    virtual bool intersect( const Ray& ray, Intersection& intersection ) const;
    virtual BoundingBox boundingBox() const;
    virtual QVector3D randomInteriorPoint() const;

private:
    virtual void createOpenGLBuffers();
    void createVerticesNormals();
    void createIndices();
};

#endif // SPHERE_H

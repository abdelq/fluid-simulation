#ifndef CYLINDER_H
#define CYLINDER_H

#include "Geometry/Geometry.h"

/* A simple cylinder Circle*[-0.5,0.5] that inherits from Geometry and create
 * the geometry on construction
 */

class Cylinder : public Geometry
{
public:
    Cylinder( AbstractObject* parent, const Material& material );

    virtual bool intersect( const Ray& ray, Intersection& intersection ) const;
    virtual BoundingBox boundingBox() const;
    virtual QVector3D randomInteriorPoint() const;

private:
    virtual void createOpenGLBuffers();
    void createVerticesNormals();
    void createIndices();
};

#endif // CYLINDER_H

#ifndef CUBE_H
#define CUBE_H

#include "Geometry/Geometry.h"

/* A simple unit cube [-0.5,0.5]^3 that inherits from Geometry and create
 * the geometry on construction
 */

class Cube : public Geometry
{
public:
    Cube( AbstractObject* parent, const Material& material );

    virtual bool intersect( const Ray& ray, Intersection& intersection ) const;
    virtual BoundingBox boundingBox() const;
    virtual QVector3D randomInteriorPoint() const;

private:
    bool intersectFacet( const Ray& ray, Intersection& intersection, unsigned int coord, int sign, float& t ) const;
    float vectorCoord( const QVector3D& vector, unsigned int axis ) const;

private:
    virtual void createOpenGLBuffers();
    void createVertices();
    void createNormals();
    void createIndices();
};

#endif // CUBE_H

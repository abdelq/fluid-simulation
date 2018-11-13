#ifndef GEOMETRY_H
#define GEOMETRY_H

#include "Material.h"
#include "Geometry/AbstractObject.h"
#include "Geometry/BoundingBox.h"
#include "Geometry/Intersection.h"
#include <QGLBuffer>

/* This is the parent class of every visual object in the scene. It contains
 * the material and OpenGL buffers.
 *
 * It also requires derived class to implement an intersection test with a ray.
 *
 */

class Geometry : public AbstractObject
{
public:
    Geometry( AbstractObject* parent, const Material& material );
    virtual ~Geometry();

    virtual void render( GLShader& shader );
    virtual bool intersect( const Ray& ray, Intersection& intersection ) const;
    virtual BoundingBox boundingBox() const=0;
    virtual QVector3D randomInteriorPoint() const=0;

    const Material& material() const;

protected:
    virtual void createOpenGLBuffers()=0;
    void createVertexBuffer( const QVector<QVector3D>& vertices );
    void createNormalBuffer( const QVector<QVector3D>& normals );
    void createIndexBuffer( const QVector<unsigned int>& indices );

private:
    Material _material;
    QGLBuffer _vertexBuffer;
    QGLBuffer _normalBuffer;
    QGLBuffer _indexBuffer;
    unsigned int _nbIndices;
};

#endif // GEOMETRY_H

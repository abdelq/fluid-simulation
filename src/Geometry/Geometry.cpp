#include "Geometry.h"
#include "GLShader.h"
#include <QtOpenGL>
#include <cassert>

Geometry::Geometry( AbstractObject* parent, const Material& material )
    : AbstractObject( parent )
    , _material( material )
    , _vertexBuffer( QGLBuffer::VertexBuffer )
    , _normalBuffer( QGLBuffer::VertexBuffer )
    , _indexBuffer( QGLBuffer::IndexBuffer )
    , _nbIndices( 0 )
{
}

Geometry::~Geometry()
{
}

void Geometry::render( GLShader& shader )
{
    shader.setGlobalTransformation( globalTransformation() );
    shader.setMaterial( _material );

    if ( !_indexBuffer.isCreated() )
        createOpenGLBuffers();

    _vertexBuffer.bind();
    shader.setVertexAttributeBuffer();
    shader.enableVertexAttributeArray();
    _vertexBuffer.release();

    if ( _normalBuffer.isCreated() )
    {
        _normalBuffer.bind();
        shader.setNormalAttributeBuffer();
        shader.enableNormalAttributeArray();
        _normalBuffer.release();
    }

    _indexBuffer.bind();
    glDrawElements( GL_TRIANGLES, _nbIndices, GL_UNSIGNED_INT, 0 );
    _indexBuffer.release();

    shader.disableVertexAttributeArray();
    shader.disableNormalAttributeArray();

    AbstractObject::render( shader );
}

bool Geometry::intersect( const Ray& /*ray*/, Intersection& /*intersection*/ ) const
{
    assert( !"Not impemented" );
    return false;
}

void Geometry::createVertexBuffer( const QVector<QVector3D>& vertices )
{
    _vertexBuffer.create();
    _vertexBuffer.bind();
    _vertexBuffer.setUsagePattern( QGLBuffer::StaticDraw );
    _vertexBuffer.allocate( &vertices[0], vertices.size() * sizeof( vertices[0] ) );
    _vertexBuffer.release();
}

void Geometry::createNormalBuffer( const QVector<QVector3D>& normals )
{
    _normalBuffer.create();
    _normalBuffer.bind();
    _normalBuffer.setUsagePattern( QGLBuffer::StaticDraw );
    _normalBuffer.allocate( &normals[0], normals.size() * sizeof( normals[0] ) );
    _normalBuffer.release();
}

void Geometry::createIndexBuffer( const QVector<unsigned int>& indices )
{
    _indexBuffer.create();
    _indexBuffer.bind();
    _indexBuffer.setUsagePattern( QGLBuffer::StaticDraw );
    _indexBuffer.allocate( &indices[0], indices.size() * sizeof( indices[0] ) );
    _indexBuffer.release();

    _nbIndices = indices.size();
}

const Material& Geometry::material() const
{
    return _material;
}

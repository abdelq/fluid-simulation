#include "Particles.h"
#include <cmath>

namespace
{
    static unsigned int nbThetas = 10;
    static unsigned int nbPhis = 10;
}

Particles::Particles( unsigned int nbParticles )
    : QVector<Particle>( nbParticles )
    , _vertexBuffer( QGLBuffer::VertexBuffer )
    , _normalBuffer( QGLBuffer::VertexBuffer )
    , _indexBuffer( QGLBuffer::IndexBuffer )
    , _nbIndices( 0 )
{
}

void Particles::render( const QMatrix4x4& transformation, GLShader& shader )
{
    if ( !_indexBuffer.isCreated() )
        createOpenGLBuffers();

    _vertexBuffer.bind();
    shader.setVertexAttributeBuffer();
    shader.enableVertexAttributeArray();
    _vertexBuffer.release();

    _normalBuffer.bind();
    shader.setNormalAttributeBuffer();
    shader.enableNormalAttributeArray();
    _normalBuffer.release();

    _indexBuffer.bind();

    for ( int i=0 ; i<size() ; ++i )
    {
        const Particle& particle = (*this)[i];
        QMatrix4x4 translation;
        float radius = ::pow( ( 3.0 * particle.volume() ) / ( 4.0 * M_PI ), 1.0 / 3.0 );

        translation.scale( radius );
        translation.setColumn( 3, QVector4D( particle.position(), 1 ) );
        shader.setGlobalTransformation( transformation * translation );

        glDrawElements( GL_TRIANGLES, _nbIndices, GL_UNSIGNED_INT, 0 );
    }

    _indexBuffer.release();

    shader.disableVertexAttributeArray();
    shader.disableNormalAttributeArray();
}

void Particles::createOpenGLBuffers()
{
    createVerticesNormals();
    createIndices();
}

void Particles::createVerticesNormals()
{
    QVector<QVector3D> vertices;

    // Bottom vertex
    vertices.append( QVector3D( 0, -1, 0 ) );

    // Middle vertices
    for ( unsigned int i=1 ; i<nbPhis ; ++i )
    {
        for ( unsigned int j=0 ; j<nbThetas ; ++j )
        {
            float theta = 2.0 * M_PI * j / nbThetas;
            float phi = M_PI * i / nbThetas;
            vertices.append( QVector3D( ::cos( theta ) * ::sin( phi ), -::cos( phi ), ::sin( theta ) * ::sin( phi ) ) );
        }
    }

    // Top vertex
    vertices.append( QVector3D( 0, 1, 0 ) );

    createVertexBuffer( vertices );
    createNormalBuffer( vertices );
}

void Particles::createIndices()
{
    QVector<unsigned int> indices;

    // Bottom cap
    for ( unsigned int i=0 ; i<nbThetas ; ++i )
    {
        indices.append( 0 );
        indices.append( ( i + 1 ) % nbThetas + 1 );
        indices.append( i + 1 );
    }

    // Middle faces
    for ( unsigned int i=0 ; i<nbPhis-2 ; ++i )
    {
        for ( unsigned int j=0 ; j<nbThetas ; ++j )
        {
            // Compute indices
            unsigned int base0 = i * nbThetas + 1;
            unsigned int base1 = ( i + 1 ) * nbThetas + 1;
            unsigned int i00 = base0 + j;
            unsigned int i01 = base0 + ( j + 1 ) % nbThetas;
            unsigned int i10 = base1 + j;
            unsigned int i11 = base1 + ( j + 1 ) % nbThetas;

            // Face 0
            indices.append( i00 );
            indices.append( i01 );
            indices.append( i11 );

            // Face 1
            indices.append( i00 );
            indices.append( i11 );
            indices.append( i10 );
        }
    }

    // Top cap
    unsigned int lastVertex = nbThetas * ( nbPhis - 1 ) + 1;

    for ( unsigned int i=0 ; i<nbThetas ; ++i )
    {
        indices.append(  lastVertex );
        indices.append(  lastVertex - nbThetas + i );
        indices.append(  lastVertex - nbThetas + ( i + 1 ) % nbThetas );
    }

    createIndexBuffer( indices );
}

void Particles::createVertexBuffer( const QVector<QVector3D>& vertices )
{
    _vertexBuffer.create();
    _vertexBuffer.bind();
    _vertexBuffer.setUsagePattern( QGLBuffer::StaticDraw );
    _vertexBuffer.allocate( &vertices[0], vertices.size() * sizeof( vertices[0] ) );
    _vertexBuffer.release();
}

void Particles::createNormalBuffer( const QVector<QVector3D>& normals )
{
    _normalBuffer.create();
    _normalBuffer.bind();
    _normalBuffer.setUsagePattern( QGLBuffer::StaticDraw );
    _normalBuffer.allocate( &normals[0], normals.size() * sizeof( normals[0] ) );
    _normalBuffer.release();
}

void Particles::createIndexBuffer( const QVector<unsigned int>& indices )
{
    _indexBuffer.create();
    _indexBuffer.bind();
    _indexBuffer.setUsagePattern( QGLBuffer::StaticDraw );
    _indexBuffer.allocate( &indices[0], indices.size() * sizeof( indices[0] ) );
    _indexBuffer.release();

    _nbIndices = indices.size();
}

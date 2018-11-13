#ifndef PARTICLES_H
#define PARTICLES_H

#include "SPH/Particle.h"
#include "GLShader.h"
#include <QGLBuffer>

#define M_PI 3.14159265358979323846264338327950288

/* A list of particles + a render function.
 */

class Particles : public QVector<Particle>
{
public:
    Particles( unsigned int nbParticles );

    void render( const QMatrix4x4& transformation, GLShader& shader );

private:
    void createOpenGLBuffers();
    void createVerticesNormals();
    void createIndices();
    void createVertexBuffer( const QVector<QVector3D>& vertices );
    void createNormalBuffer( const QVector<QVector3D>& normals );
    void createIndexBuffer( const QVector<unsigned int>& indices );

private:
    QGLBuffer _vertexBuffer;
    QGLBuffer _normalBuffer;
    QGLBuffer _indexBuffer;
    unsigned int _nbIndices;
};

#endif // PARTICLESET_H

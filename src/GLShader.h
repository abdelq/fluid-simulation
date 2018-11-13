#ifndef GLSHADER_H
#define GLSHADER_H

#include "Material.h"
#include <QGLShaderProgram>

class Camera;
class Light;

/* An uber shader that tries to do everything at the same thing.
 * It can morph into a diffuse, environment or refractive shader.
 */

class GLShader
{
public:
    GLShader();

    void initialize();
    void setupCamera( const Camera& camera );
    void bind();
    void setVertexAttributeBuffer();
    void setNormalAttributeBuffer();
    void setVertexAttributeArray( QVector3D* vertices );
    void setNormalAttributeArray( QVector3D* normals );
    void enableVertexAttributeArray();
    void enableNormalAttributeArray();
    void disableVertexAttributeArray();
    void disableNormalAttributeArray();
    void setGlobalTransformation( const QMatrix4x4& globalTransformation );
    void setMaterial( const Material& material );
    void release();

private:
    QGLShaderProgram _shader;

    // Locations
    unsigned int _vertexLocation;
    unsigned int _normalLocation;
    unsigned int _viewProjectionMatrixLocation;
    unsigned int _modelMatrixLocation;
    unsigned int _normalMatrixLocation;
    unsigned int _lightDirectionLocation;
    unsigned int _cameraPositionLocation;
    unsigned int _materialDiffuseLocation;
};

#endif // GLSHADER_H

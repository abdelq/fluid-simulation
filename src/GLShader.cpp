#include "GLShader.h"
#include "Geometry/Camera.h"
#include <cmath>

GLShader::GLShader()
    : _vertexLocation( 0 )
    , _normalLocation( 0 )
    , _viewProjectionMatrixLocation( 0 )
    , _modelMatrixLocation( 0 )
    , _normalMatrixLocation( 0 )
    , _lightDirectionLocation( 0 )
    , _cameraPositionLocation( 0 )
    , _materialDiffuseLocation( 0 )
{
}

void GLShader::initialize()
{
    _shader.addShaderFromSourceFile( QGLShader::Vertex,  "shaders/diffuse.vs" );
    _shader.addShaderFromSourceFile( QGLShader::Fragment,  "shaders/diffuse.fs" );
    _shader.link();

    _vertexLocation = _shader.attributeLocation( "vertex" );
    _normalLocation = _shader.attributeLocation( "normal" );
    _viewProjectionMatrixLocation = _shader.uniformLocation( "viewProjectionMatrix" );
    _modelMatrixLocation = _shader.uniformLocation( "modelMatrix" );
    _normalMatrixLocation = _shader.uniformLocation( "normalMatrix" );
    _lightDirectionLocation = _shader.uniformLocation( "light.direction" );
    _cameraPositionLocation = _shader.uniformLocation( "camera.position" );
    _materialDiffuseLocation = _shader.uniformLocation( "material.diffuse" );
}

void GLShader::setupCamera( const Camera& camera )
{
    QMatrix4x4 viewMatrix = camera.globalTransformation().inverted();
    _shader.setUniformValue( _viewProjectionMatrixLocation, camera.projectionMatrix() * viewMatrix );
    _shader.setUniformValue( _lightDirectionLocation, camera.globalTransformation().column(2).toVector3D() );
    _shader.setUniformValue( _cameraPositionLocation, camera.globalTransformation().column(3).toVector3D() );
}

void GLShader::bind()
{
    _shader.bind();
}

void GLShader::setVertexAttributeBuffer()
{
    _shader.setAttributeBuffer( _vertexLocation, GL_FLOAT, 0, 3 );
}

void GLShader::setNormalAttributeBuffer()
{
    _shader.setAttributeBuffer( _normalLocation, GL_FLOAT, 0, 3 );
}

void GLShader::setVertexAttributeArray( QVector3D* vertices )
{
    _shader.setAttributeArray( _vertexLocation, vertices );
}

void GLShader::setNormalAttributeArray( QVector3D* normals )
{
    _shader.setAttributeArray( _normalLocation, normals );
}

void GLShader::enableVertexAttributeArray()
{
    _shader.enableAttributeArray( _vertexLocation );
}

void GLShader::enableNormalAttributeArray()
{
    _shader.enableAttributeArray( _normalLocation );
}

void GLShader::disableVertexAttributeArray()
{
    _shader.disableAttributeArray( _vertexLocation );
}

void GLShader::disableNormalAttributeArray()
{
    _shader.disableAttributeArray( _normalLocation );
}

void GLShader::setGlobalTransformation( const QMatrix4x4& globalTransformation )
{
    _shader.setUniformValue( _modelMatrixLocation, globalTransformation );
    _shader.setUniformValue( _normalMatrixLocation, globalTransformation.normalMatrix() );
}

void GLShader::setMaterial( const Material& material )
{
    _shader.setUniformValue( _materialDiffuseLocation, material.diffuse() );
}

void GLShader::release()
{
    _shader.release();
}

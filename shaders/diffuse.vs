#version 120

struct Camera
{
    vec3 position;
};

uniform mat4 modelMatrix;
uniform mat3 normalMatrix;
uniform mat4 viewProjectionMatrix;
uniform Camera camera;

attribute vec3 vertex;
attribute vec3 normal;

varying vec3 vVertex;
varying vec3 vNormal;
varying vec3 vEyeDirection;

void main()
{
    vVertex = vec3( modelMatrix * vec4( vertex, 1.0 ) );
    vNormal = normalize( normalMatrix * normal );
    vEyeDirection = camera.position - vVertex;
    gl_Position = viewProjectionMatrix * vec4( vVertex, 1.0 );
}

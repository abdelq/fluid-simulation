#version 120

struct Light
{
    vec3 direction;
};

struct Material
{
    vec4 diffuse;
};

uniform Light light;
uniform Material material;
uniform samplerCube environment;

varying vec3 vVertex;
varying vec3 vNormal;
varying vec3 vEyeDirection;

void main()
{
    gl_FragColor = vec4(0,0,0,1);
    vec3 N = normalize( vNormal );
  
    // Diffuse
    float nDotD = abs( dot( N, light.direction ) );
    gl_FragColor.rgb = material.diffuse.rgb * nDotD;
    gl_FragColor.a = material.diffuse.a;
}


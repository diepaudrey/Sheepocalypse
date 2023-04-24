#version 330

in vec3 vPosition_vs;
in vec3 vNormal_vs;
in vec2 vTexCoords;

uniform vec3 uKD;
uniform vec3 uKs;
uniform float uShininess;

uniform vec3 uLightDir_vs;
uniform vec3 uLightIntensity;

vec3 blinnPhong(){

    vec3 wi = normalize(uLightDir_vs);
    vec3 Li =uLightIntensity;
    vec3 N = vNormal_vs;
    vec3 wo = normalize(-vPosition_vs);
    vec3 halfVector =(wo + wi)/2.f;
    return Li*(uKD*max(dot(wi,N),0.) + uKs*pow(max(dot(halfVector,N), 0.), uShininess));
    
}

out vec3 fFragColor;

void main()
{
    fFragColor = blinnPhong();
}
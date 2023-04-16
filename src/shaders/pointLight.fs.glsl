#version 330

in vec3 vPosition_vs;
in vec3 vNormal_vs;
in vec2 vTexCoords;

uniform vec3 uKa;
uniform vec3 uKD;
uniform vec3 uKs;
uniform float uShininess;

uniform vec3 uLightPos_vs;
uniform vec3 uLightIntensity;

vec3 blinnPhong(){

    float d = distance(uLightPos_vs, vPosition_vs);
    vec3 wi = normalize(uLightPos_vs - vPosition_vs);
    vec3 Li = (uLightIntensity / (d * d) );
    vec3 N = vNormal_vs;
    vec3 wo = normalize(-vPosition_vs);
    vec3 halfVector =(wo + wi)/2.f;
    
    return uKa +Li*(uKD*max(dot(wi,N),0.) + uKs*pow(max(dot(halfVector,N), 0.), uShininess));
    
}

out vec4 fFragColor;

void main()
{
    fFragColor = vec4(blinnPhong(), 1);
}
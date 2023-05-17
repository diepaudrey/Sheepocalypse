#version 330

in vec3 vPosition_vs;
in vec3 vNormal_vs;
in vec2 vTexCoords;
in vec4 vFragPosLight;

out vec4 fFragColor;

uniform vec3 uKa;
uniform vec3 uKD;
uniform vec3 uKs;
uniform float uShininess;

uniform vec3 uLightPos_vs;
uniform vec3 uLightIntensity;

uniform int uNumTextures;
uniform sampler2D uTextures[4];
uniform sampler2D uDepthTexture;

vec3 blinnPhong(){

    vec3 wi = normalize(uLightPos_vs);
    vec3 Li =uLightIntensity;
    vec3 N = vNormal_vs;
    vec3 wo = normalize(-vPosition_vs);
    vec3 halfVector =(wo + wi)/2.f;
    return Li*(uKD*max(dot(wi,N),0.) + uKs*pow(max(dot(halfVector,N), 0.), uShininess));
    
}
float ShadowCalculation(vec4 vFragPosLight)
{
    // perform perspective divide
    vec3 projCoords = vFragPosLight.xyz/vFragPosLight.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(uDepthTexture, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // check whether current frag pos is in shadow
    float shadow = currentDepth > closestDepth  ? 1.0 : 0.0;

    return shadow;
}

void main() {

    // Calcul de la couleur de la lumière
    // float d = distance(uLightPos_vs, vPosition_vs);
    // vec3 wi = normalize(uLightPos_vs - vPosition_vs);
    // vec3 Li = (uLightIntensity / (d * d) );
    // vec3 N = vNormal_vs;
    // vec3 wo = normalize(-vPosition_vs);
    // vec3 halfVector =(wo + wi)/2.f;
    //vec3 lightColor = uKa + Li * (uKD * max(dot(wi,N),0.) + uKs * pow(max(dot(halfVector,N), 0.), uShininess));

    vec3 lightColor = blinnPhong();
    // Calcul de la couleur des textures
    vec2 fCorrectCoords = vec2(vTexCoords.x, 1.0 - vTexCoords.y);
    vec4 textureColor = vec4(0.0, 0.0, 0.0, 0.0);
    for (int i = 0; i < uNumTextures; ++i) {
        textureColor += texture(uTextures[i], fCorrectCoords);
    }
    //textureColor +=  texture(uDepthTexture, fCorrectCoords);

    // Combinaison des couleurs de la lumière et des textures
    //fFragColor = vec4(lightColor.rgb*(1.0f - ShadowCalculation(vFragPosLight)) * textureColor.rgb, 1.0);
    fFragColor = vec4(lightColor.rgb*(1.0f - ShadowCalculation(vFragPosLight)) * textureColor.rgb, 1.0);
    
}

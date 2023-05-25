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

//float ShadowCalculation(vec4 vFragPosLight)
//{
    //float shadow = 0.f;
    //vec3 lightCoords = vFragPosLight.xyz/vFragPosLight.w;
   // lightCoords = lightCoords * 0.5 + 0.5;
    //if(lightCoords.z <= 1.0f)
    //{
        //lightCoords = (lightCoords + 1.0f) / 2.0f;
        //float closestDepth = texture(uDepthTexture,lightCoords.xy).r;
        //float currentDepth = lightCoords.z;
        //float bias = 0.005f;
       // if(currentDepth > closestDepth + bias){
       //     shadow = 1.0f ;
       // }
    //}
    //return shadow;
//}

//Use for DirectionalLight
vec3 blinnPhong(){

    vec3 wi = normalize(uLightPos_vs);
    vec3 Li =uLightIntensity;
    vec3 N = vNormal_vs;
    vec3 wo = normalize(-vPosition_vs);
    vec3 halfVector =(wo + wi)/2.f;
    // specular = uks , ukd = diffuse , uka = ambient ;
    return (uKa + Li*((uKD*max(dot(wi,N),0.) + uKs*pow(max(dot(halfVector,N), 0.), uShininess))));   
}

//Use for pointLight
//vec3 blinnPhong(){

   // float d = distance(uLightPos_vs, vPosition_vs);
   // vec3 wi = normalize(uLightPos_vs - vPosition_vs);
    //vec3 Li = (uLightIntensity / (d * d) );
    //vec3 N = vNormal_vs;
    //vec3 wo = normalize(-vPosition_vs);
    //vec3 halfVector =(wo + wi)/2.f;
    // specular = uks , ukd = diffuse , uka = ambient ;
    //return (uKa +  (1.0f - ShadowCalculation(vFragPosLight))*Li*((uKD*max(dot(wi,N),0.) + uKs*pow(max(dot(halfVector,N), 0.), uShininess))));
    
//}

void main() {

    vec3 lightColor = blinnPhong();

    // Calculate the color of the texture
    vec2 fCorrectCoords = vec2(vTexCoords.x, 1.0 - vTexCoords.y);
    vec4 textureColor = vec4(0.0, 0.0, 0.0, 0.0);
    for (int i = 0; i < uNumTextures; ++i) {
        textureColor += texture(uTextures[i], fCorrectCoords);
    }

    // Combine Texture and Light
    fFragColor = vec4(lightColor.rgb * textureColor.rgb, 1.0);
}

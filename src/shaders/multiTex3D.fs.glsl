#version 330
#extension GL_ARB_draw_instanced : enable

//variables d'entrées
in vec3 vPosition_vs;
in vec3 vNormal_vs;
in vec2 vTexCoords;

vec2 fCorrectCoords;

out vec4 fFragColor;

// uniform sampler2D uTexture1;
// uniform sampler2D uTexture2;
// uniform sampler2D uTexture3;
// uniform sampler2D uTexture4;

uniform int uNumTextures;
uniform sampler2D uTextures[4];

void main() {
    fCorrectCoords = vec2(vTexCoords.x, 1-vTexCoords.y);
    //fFragColor = texture(uTexture1, fCorrectCoords) + texture(uTexture2, fCorrectCoords);
    //fFragColor = texture(uTexture1, fCorrectCoords) + texture(uTexture2, fCorrectCoords) + texture(uTexture3, fCorrectCoords) + texture(uTexture4, fCorrectCoords);
    vec4 color = vec4(0.0, 0.0, 0.0, 1.0);
    if (uNumTextures >= 1) {
        color += texture(uTextures[0], fCorrectCoords);
    }
    if (uNumTextures >= 2) {
        color += texture(uTextures[1], fCorrectCoords);
    }
    if (uNumTextures >= 3) {
        color += texture(uTextures[2], fCorrectCoords);
    }
    if (uNumTextures >= 4) {
        color += texture(uTextures[3], fCorrectCoords);
    }

    

    fFragColor = color;

    
}
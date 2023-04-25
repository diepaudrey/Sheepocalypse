#version 330

//variables d'entrées
in vec3 vPosition_vs;
in vec3 vNormal_vs;
in vec2 vTexCoords;

vec2 fCorrectCoords;

out vec4 fFragColor;

uniform sampler2D uTexture1;
uniform sampler2D uTexture2;
uniform sampler2D uTexture3;
uniform sampler2D uTexture4;

void main() {
    fCorrectCoords = vec2(vTexCoords.x, 1-vTexCoords.y);
    //fFragColor = texture(uTexture1, fCorrectCoords) + texture(uTexture2, fCorrectCoords);
    fFragColor = texture(uTexture1, fCorrectCoords) + texture(uTexture2, fCorrectCoords) + texture(uTexture3, fCorrectCoords) + texture(uTexture4, fCorrectCoords);
}
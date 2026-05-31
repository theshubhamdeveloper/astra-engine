#version 330 core

in vec4 color;
in vec2 uv;
in float texId;

uniform sampler2D uTex[16];

out vec4 FragColor;

void main() {
    vec4 finalColor = color;

    int id = int(texId);
    finalColor *= texture(uTex[id], uv);

    FragColor = finalColor;
}
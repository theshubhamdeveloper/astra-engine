#version 330 core

in vec2 uv;
in vec4 color;
in float texId;

uniform sampler2D uTex[16];

out vec4 FragColor;

void main() {
    FragColor = color * texture(uTex[int(texId)], uv);
}
#version 330 core

in vec4 color;
in vec2 uv;

uniform bool uUseTex;
uniform sampler2D uTex;

out vec4 FragColor;

void main() {
    vec4 finalColor = color;

    if (uUseTex) {
        finalColor *= texture(uTex, uv);
    }

    FragColor = finalColor;
}
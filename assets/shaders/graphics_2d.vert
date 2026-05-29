#version 330 core

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aUV;
layout (location = 2) in vec4 aColor;

uniform mat3 uProjection;
uniform mat3 uView;

out vec2 uv;
out vec4 color;

void main() {
    vec3 MVP = uProjection * uView * vec3(aPos, 1.0f);

    gl_Position = vec4(MVP.x, MVP.y, 0.0, 1.0);

    color = aColor;
    uv = aUV;
}

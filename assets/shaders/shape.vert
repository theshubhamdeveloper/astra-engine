#version 330 core

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aUV;
layout (location = 2) in vec4 aColor;
layout (location = 3) in vec2 aSize;
layout (location = 4) in vec4 aRadius;
layout (location = 5) in vec4 aStrokeColor;
layout (location = 6) in float aStrokeWidth;
layout (location = 7) in float aTexId;

uniform mat3 uProjection;
uniform mat3 uView;

out vec2 uv;
out vec4 color;
flat out vec2 size;
flat out vec4 radius;
flat out vec4 strokeColor;
flat out float strokeWidth;
flat out float texId;

void main() {
    vec3 MVP = uProjection * uView * vec3(aPos, 1.0f);

    gl_Position = vec4(MVP.x, MVP.y, 0.0, 1.0);

    color = aColor;
    uv = aUV;
    size = aSize;
    radius = aRadius;
    strokeColor = aStrokeColor;
    strokeWidth = aStrokeWidth;
    texId = aTexId;
}

#version 330 core

in vec4 color;
in vec2 uv;
flat in vec2 size;
flat in vec4 radius;
flat in float texId;

uniform sampler2D uTex[16];

out vec4 FragColor;

float sdfRoundedBox(vec2 p, vec2 b, vec4 r) {
    r.xy = (p.x > 0.0) ? r.xy : r.zw;
    r.x = (p.y > 0.0) ? r.x : r.y;
    vec2 q = abs(p) - b + r.x;
    return min(max(q.x, q.y), 0.0) + length(max(q, 0.0)) - r.x;
}

void main() {
    vec4 finalColor = color;

    int id = int(texId);
    finalColor *= texture(uTex[id], uv);

    vec2 p = (uv - vec2(0.5)) * size;
    vec2 halfSize = size * 0.5;

    if (sdfRoundedBox(p, halfSize, radius) <= 0.0) {
        FragColor = finalColor;
    } else {
        FragColor = vec4(0.0);
    }
}
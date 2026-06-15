#version 330 core

in vec4 color;
in vec2 uv;

flat in vec2 size;
flat in vec4 radius;
flat in vec4 strokeColor;
flat in float strokeWidth;
flat in float texId;

uniform sampler2D uTex[16];

out vec4 FragColor;

float sdfRoundedBox(vec2 p, vec2 halfSize, vec4 radius) {
    radius.xy = (p.x > 0.0) ? radius.xy : radius.zw;
    radius.x = (p.y > 0.0) ? radius.x : radius.y;

    vec2 q = abs(p) - halfSize + radius.x;

    return min(max(q.x, q.y), 0.0) + length(max(q, 0.0)) - radius.x;
}

void main() {
    const float aaPadding = 1.0;

    vec2 strokeSize = vec2(strokeWidth * 2.0);
    vec2 shapeSize = size + strokeSize;
    vec2 renderSize = shapeSize + aaPadding * 2.0;

    vec2 localPos = (uv - 0.5) * renderSize;

    vec2 contentOffset = vec2(strokeWidth + aaPadding);

    vec2 contentUV = (uv * renderSize - contentOffset) / size;
    contentUV = clamp(contentUV, 0.0, 1.0);

    vec4 fillColor = color * texture(uTex[int(texId)], contentUV);

    float distanceToShape = sdfRoundedBox(localPos, shapeSize * 0.5, radius);

    float aaWidth = fwidth(distanceToShape);

    if (strokeWidth <= 0.0) {
        float alpha = 1.0 - smoothstep(0.0, aaWidth, distanceToShape);

        fillColor.a *= alpha;
        FragColor = fillColor;
        return;
    }

    float outerMask = 1.0 - smoothstep(0.0, aaWidth, distanceToShape);
    float innerMask = 1.0 - smoothstep(-strokeWidth - aaWidth, -strokeWidth + aaWidth, distanceToShape);

    vec4 finalColor = mix(strokeColor, fillColor, innerMask);
    finalColor.a *= outerMask;

    FragColor = finalColor;
}
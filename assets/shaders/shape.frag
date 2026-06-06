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

float sdfRoundedBox(vec2 p, vec2 b, vec4 r)
{
    r.xy = (p.x > 0.0) ? r.xy : r.zw;
    r.x = (p.y > 0.0) ? r.x : r.y;

    vec2 q = abs(p) - b + r.x;

    return min(max(q.x, q.y), 0.0)
    + length(max(q, 0.0))
    - r.x;
}

void main()
{
    vec4 fillColor = color;

    fillColor *= texture(uTex[int(texId)], uv);


    vec2 totalSize = size + strokeWidth * 2.0;
    // Padding 2px for aa
    vec2 p = (uv - 0.5) * (totalSize + 2);

    float d = sdfRoundedBox(
        p,
        totalSize * 0.5,
        radius
    );

    float aa = fwidth(d);

    if (strokeWidth <= 0.0)
    {
        float alpha = 1.0 - smoothstep(0.0, aa, d);
        fillColor.a *= alpha;
        FragColor = fillColor;
        return;
    }

    float outerAlpha = 1.0 - smoothstep(0.0, aa, d);

    float innerAlpha = 1.0 - smoothstep(-strokeWidth - aa,
                                        -strokeWidth + aa,
                                        d);

    vec4 result = mix(strokeColor, fillColor, innerAlpha);

    result.a *= outerAlpha;

    FragColor = result;
}
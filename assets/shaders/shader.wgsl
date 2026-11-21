struct VertexInput {
    @location(0) position: vec2<f32>,
    @location(1) uv: vec2<f32>,
    @location(2) use_tex: u32,
    @location(3) color: vec4<f32>,
}

struct VertexOutput {
    @builtin(position) position: vec4<f32>,
    @location(1) uv: vec2<f32>,
    @location(2) @interpolate(flat) use_tex: u32,
    @location(3) color: vec4<f32>,
}

@group(1) @binding(0) var<uniform> projection: mat4x4<f32>;

@vertex
fn vs_main(model: VertexInput) -> VertexOutput {
    var out: VertexOutput;
    out.color = model.color;
    out.uv = model.uv;
    out.position = projection * vec4<f32>(model.position, 0.0, 1.0);
    out.use_tex = model.use_tex;
    return out;
}

@group(0) @binding(0) var t_diffuse: texture_2d<f32>;
@group(0) @binding(1) var s_diffuse: sampler;

@fragment
fn fs_main(in: VertexOutput) -> @location(0) vec4<f32> {
    if (in.use_tex == 1u) {
        return textureSample(t_diffuse, s_diffuse, in.uv) * in.color;
    }

    return in.color;
}

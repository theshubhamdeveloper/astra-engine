use crate::engine::Color;
use crate::engine::resource_manager::Handle;
use crate::engine::texture::GpuTexture;

#[derive(Copy, Clone, Default)]
pub struct Paint {
    pub color: Color,
    pub texture_id: Option<Handle<GpuTexture>>,
}

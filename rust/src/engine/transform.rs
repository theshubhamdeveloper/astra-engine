pub struct Transform {
    pub position: glam::Vec2,
    pub rotation: f32,
    pub scale: glam::Vec2,
}

impl Default for Transform {
    fn default() -> Self {
        Self {
            position: glam::Vec2::ZERO,
            rotation: 0.0,
            scale: glam::Vec2::ONE,
        }
    }
}

impl Transform {
    pub fn to_mat3(&self) -> glam::Mat3 {
        glam::Mat3::from_scale_angle_translation(
            self.scale,
            self.rotation.to_radians(),
            self.position,
        )
    }
}

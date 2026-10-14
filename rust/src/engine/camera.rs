pub struct Camera {
    pub position: glam::Vec2,
    pub zoom: f32,
    pub screen_size: glam::Vec2,
    pub projection: glam::Mat4,
}

impl Camera {
    pub fn new(screen_size: glam::Vec2, position: glam::Vec2, zoom: f32) -> Self {
        let mut camera = Self {
            position,
            zoom,
            screen_size,
            projection: glam::Mat4::IDENTITY,
        };
        camera.update_matrix();
        camera
    }

    pub fn update_matrix(&mut self) {
        let half_width = (self.screen_size.x / 2.0) / self.zoom;
        let half_height = (self.screen_size.y / 2.0) / self.zoom;

        let left = -half_width + self.position.x;
        let right = half_width + self.position.x;
        let bottom = -half_height + self.position.y;
        let top = half_height + self.position.y;
        
        self.projection = glam::camera::rh::proj::directx::orthographic(
            left, right,
            bottom, top,
            -1.0, 1.0,
        );
    }
}
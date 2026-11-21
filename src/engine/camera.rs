pub struct Camera {
    pub position: glam::Vec2,
    pub zoom: f32,
    pub rotation: f32,
    pub screen_size: glam::Vec2,
    pub projection: glam::Mat4,
}

impl Camera {
    const MIN_ZOOM: f32 = 0.05;
    const MAX_ZOOM: f32 = 100.0;

    pub fn new(screen_size: glam::Vec2, position: glam::Vec2, zoom: f32) -> Self {
        let mut camera = Self {
            position,
            zoom: if zoom.is_finite() {
                zoom.clamp(Self::MIN_ZOOM, Self::MAX_ZOOM)
            } else {
                1.0
            },
            rotation: 0.0,
            screen_size,
            projection: glam::Mat4::IDENTITY,
        };
        camera.update_matrix();
        camera
    }

    pub fn update_matrix(&mut self) {
        if self.screen_size.x <= 0.0 || self.screen_size.y <= 0.0 {
            return;
        }

        let view_size = self.screen_size / self.zoom;
        let center = self.position + view_size * 0.5;
        let projection = glam::camera::rh::proj::directx::orthographic(
            -view_size.x * 0.5,
            view_size.x * 0.5,
            view_size.y * 0.5,
            -view_size.y * 0.5,
            -1.0,
            1.0,
        );
        let view = glam::Mat4::from_rotation_translation(
            glam::Quat::from_rotation_z(-self.rotation),
            glam::Vec3::new(-center.x, -center.y, 0.0),
        );
        self.projection = projection * view;
    }

    pub fn pan_screen(&mut self, delta: glam::Vec2) {
        if !delta.is_finite() {
            return;
        }

        let world_delta = glam::Vec2::from_angle(self.rotation).rotate(delta / self.zoom);
        self.position -= world_delta;
        self.update_matrix();
    }

    pub fn zoom_at(&mut self, screen_position: glam::Vec2, scale_factor: f32) {
        if !scale_factor.is_finite() || scale_factor <= 0.0 || !screen_position.is_finite() {
            return;
        }

        let world_position = self.screen_to_world(screen_position);
        self.zoom = (self.zoom * scale_factor).clamp(Self::MIN_ZOOM, Self::MAX_ZOOM);
        self.update_matrix();
        self.position += world_position - self.screen_to_world(screen_position);
        self.update_matrix();
    }

    pub fn rotate(&mut self, delta_radians: f32) {
        if !delta_radians.is_finite() {
            return;
        }

        self.rotation = (self.rotation + delta_radians).rem_euclid(std::f32::consts::TAU);
        self.update_matrix();
    }

    pub fn reset(&mut self) {
        self.position = glam::Vec2::ZERO;
        self.zoom = 1.0;
        self.rotation = 0.0;
        self.update_matrix();
    }

    fn screen_to_world(&self, screen_position: glam::Vec2) -> glam::Vec2 {
        let screen_center = self.screen_size * 0.5;
        let view_center = self.position + screen_center / self.zoom;
        view_center
            + glam::Vec2::from_angle(self.rotation)
                .rotate((screen_position - screen_center) / self.zoom)
    }
}

#[cfg(test)]
mod tests {
    use super::Camera;
    use glam::Vec2;

    #[test]
    fn zoom_at_keeps_world_point_under_cursor() {
        let mut camera = Camera::new(Vec2::new(800.0, 600.0), Vec2::ZERO, 1.0);
        let cursor = Vec2::new(200.0, 150.0);
        let world_position = camera.screen_to_world(cursor);

        camera.zoom_at(cursor, 2.0);

        assert!((camera.screen_to_world(cursor) - world_position).length() < 0.001);
        assert_eq!(camera.zoom, 2.0);
    }

    #[test]
    fn pan_and_rotation_update_the_camera_matrix() {
        let mut camera = Camera::new(Vec2::new(800.0, 600.0), Vec2::ZERO, 1.0);
        let initial_projection = camera.projection;

        camera.pan_screen(Vec2::new(10.0, 20.0));
        camera.rotate(std::f32::consts::FRAC_PI_2);

        assert_eq!(camera.position, Vec2::new(-10.0, -20.0));
        assert_ne!(camera.projection, initial_projection);
        assert!((camera.rotation - std::f32::consts::FRAC_PI_2).abs() < 0.0001);
    }
}

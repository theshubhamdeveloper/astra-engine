use crate::engine::{
    draw_command::DrawCommand, paint::Paint, renderer::Renderer, shapes::Rect, transform::Transform,
};
use std::sync::{Arc, Mutex};

pub struct Canvas {
    renderer: Arc<Mutex<Renderer>>,
}

impl Canvas {
    pub fn new(renderer: Arc<Mutex<Renderer>>) -> Self {
        Self { renderer }
    }

    pub fn draw_rect(&mut self, rect: &Rect, transform: &Transform, paint: &Paint) {
        let mut renderer_guard = self.renderer.lock().unwrap();
        renderer_guard.draw_commands.push(DrawCommand::FillRect {
            rect: *rect,
            transform: transform.to_mat3(),
            paint: *paint,
        });
    }
}

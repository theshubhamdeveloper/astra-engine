use crate::engine::{Canvas, Color, Paint, Rect, Renderer, Time, Transform};

use winit::{
    application::ApplicationHandler,
    event::WindowEvent,
    event_loop::ActiveEventLoop,
    window::{Window, WindowAttributes, WindowId},
};

use glam::Vec2;

use std::sync::{Arc, Mutex};

pub struct Application {
    window: Option<Arc<Window>>,
    window_attributes: Option<WindowAttributes>,
    renderer: Option<Arc<Mutex<Renderer>>>,
    time: Time,
    canvas: Option<Canvas>,
}

impl ApplicationHandler for Application {
    fn resumed(&mut self, event_loop: &ActiveEventLoop) {
        let window = Arc::new(
            event_loop
                .create_window(self.window_attributes.take().unwrap())
                .unwrap(),
        );

        let renderer = Arc::new(Mutex::new(
            pollster::block_on(Renderer::new(window.clone())).unwrap(),
        ));

        self.window = Some(window);
        self.canvas = Some(Canvas::new(renderer.clone()));
        self.renderer = Some(renderer);
    }

    fn window_event(&mut self, event_loop: &ActiveEventLoop, _id: WindowId, event: WindowEvent) {
        let renderer_exists = self.renderer.is_some();
        if !renderer_exists {
            return;
        }

        match event {
            WindowEvent::CloseRequested => {
                event_loop.exit();
            }

            WindowEvent::Resized(size) => {
                if let Some(ref renderer) = self.renderer {
                    renderer.lock().unwrap().resize(size.width, size.height);
                }
            }

            WindowEvent::RedrawRequested => {
                self.update();
                if let Some(ref renderer) = self.renderer {
                    renderer.lock().unwrap().render().unwrap();
                }
            }

            _ => (),
        }
    }
    fn about_to_wait(&mut self, _event_loop: &ActiveEventLoop) {
        if let Some(ref window) = self.window {
            window.request_redraw();
        }
    }
}

impl Application {
    pub fn new(window_attributes: WindowAttributes) -> Self {
        Self {
            window: None,
            window_attributes: Some(window_attributes),
            renderer: None,
            time: Time::new(),
            canvas: None,
        }
    }

    pub fn update(&mut self) {
        self.time.update();

        println!("{:?}", self.time.delta_time() * 1000.0);

        if let Some(ref mut canvas) = self.canvas {
            let steps = 100;
            for i in 0..steps {
                let progress = i as f32 / (steps - 1) as f32;
                let rotation = 180.0 * (1.0 - progress);
                let alpha = 1.0 / (i as f32 + 1.0);

                canvas.draw_rect(
                    &Rect {
                        width: 200.0,
                        height: 200.0,
                        corner_radius: [0.0, 0.0, 0.0, 0.0],
                    },
                    &Transform {
                        position: glam::vec2(0.0, 0.0),
                        rotation,
                        scale: Vec2::ONE,
                    },
                    &Paint {
                        color: Color::new(0.0, 0.0, 1.0, alpha),
                    },
                );
            }
        }
    }
}

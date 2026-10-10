use crate::engine::renderer::Renderer;
use std::sync::Arc;
use winit::application::ApplicationHandler;
use winit::event::WindowEvent;
use winit::event_loop::ActiveEventLoop;
use winit::window::{Window, WindowAttributes, WindowId};

pub struct Application {
    window: Option<Arc<Window>>,
    window_attributes: Option<WindowAttributes>,
    renderer: Option<Renderer>,
}

impl ApplicationHandler for Application {
    fn resumed(&mut self, event_loop: &ActiveEventLoop) {
        let window = Arc::new(
            event_loop
                .create_window(self.window_attributes.take().unwrap())
                .unwrap(),
        );

        let renderer = pollster::block_on(Renderer::new(window.clone())).unwrap();

        self.window = Some(window);
        self.renderer = Some(renderer);
    }

    fn window_event(&mut self, event_loop: &ActiveEventLoop, _id: WindowId, event: WindowEvent) {
        let renderer = match &mut self.renderer {
            Some(renderer) => renderer,
            None => return,
        };

        match event {
            WindowEvent::CloseRequested => {
                event_loop.exit();
            }

            WindowEvent::Resized(size) => renderer.resize(size.width, size.height),

            WindowEvent::RedrawRequested => renderer.render().unwrap(),

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
        }
    }
}

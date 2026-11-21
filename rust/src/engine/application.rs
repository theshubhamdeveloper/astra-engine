use crate::engine::{Canvas, Color, Paint, Rect, Renderer, Time, Transform};
use crate::engine::resource_manager::{Handle, ResourceManager};
use crate::engine::text::textbuffer::TextLayout;
use crate::engine::text::{font::Font, textbuffer::TextBuffer};
use crate::engine::texture::GpuTexture;
use glam::{Vec2, vec2};
use std::sync::{Arc, Mutex};
use wgpu::naga::SwitchValue::Default;
use winit::{
    application::ApplicationHandler,
    event::WindowEvent,
    event_loop::ActiveEventLoop,
    window::{Window, WindowAttributes, WindowId},
};

pub struct Application {
    window: Option<Arc<Window>>,
    window_attributes: Option<WindowAttributes>,
    renderer: Option<Arc<Mutex<Renderer>>>,
    time: Time,
    canvas: Option<Canvas>,
    text_buffer: Option<TextBuffer>,
    texture_manager: Arc<Mutex<ResourceManager<GpuTexture>>>,
}

impl ApplicationHandler for Application {
    fn resumed(&mut self, event_loop: &ActiveEventLoop) {
        let window = Arc::new(
            event_loop
                .create_window(self.window_attributes.take().unwrap())
                .unwrap(),
        );

        let renderer = Arc::new(Mutex::new(
            pollster::block_on(Renderer::new(window.clone(), self.texture_manager.clone()))
                .unwrap(),
        ));

        let font = Arc::new(Mutex::new(
            Font::from_bytes(
                include_bytes!("../../assets/fonts/JetBrainsMono/JetBrainsMono[wght].ttf").to_vec(),
                0,
            )
            .unwrap(),
        ));

        let mut text_buffer = TextBuffer::new(font.clone(), 36.0);
        // text_buffer.set_wrap_width(Some(1000.0)).unwrap();
        text_buffer.set_text(include_str!("application.rs"));
        text_buffer.set_weight(500);
        text_buffer.set_line_height(Some(text_buffer.default_line_height() * 1.3)).unwrap();
        // text_buffer.set_italic(true);
        text_buffer.shape().unwrap();

        println!("{}", text_buffer.line_height());

        let mut font_guard = font.lock().unwrap();
        let texture = renderer
            .lock()
            .unwrap()
            .upload_atlas(font_guard.atlas())
            .unwrap();
        font_guard.atlas_gpu_texture = Some(texture);

        self.window = Some(window);
        self.canvas = Some(Canvas::new(renderer.clone()));
        self.renderer = Some(renderer);

        self.text_buffer = Some(text_buffer)
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
            text_buffer: None,
            texture_manager: Arc::new(Mutex::new(ResourceManager::new())),
        }
    }

    pub fn update(&mut self) {
        self.time.update();

        println!("{:?}", self.time.delta_time()*1000.0 );

        if let (Some(canvas), Some(text_buffer)) = (self.canvas.as_mut(), self.text_buffer.as_mut())
        {
            let paint = Paint {
                color: Color::RED,
                ..Paint::default()
            };

            // canvas.draw_rect(
            //     &Rect {
            //         width: 100.0,
            //         height: 100.0,
            //         corner_radius: [20.0, 0.0, 0.0, 0.0],
            //     },
            //     &Transform {
            //         position: vec2(0.0, 0.0),
            //         rotation: 0.0,
            //         scale: Vec2::new(1.0, 1.0),
            //     },
            //     &paint,
            // );

            let layout = text_buffer.get_layout().unwrap();
            canvas.draw_text(
                layout,
                &Transform {
                    position: Vec2::ZERO,
                    rotation: 0.0,
                    scale: Vec2::new(1.0, 1.0),
                },
                &paint,
            )
        }

        if let Some(ref renderer) = self.renderer {
            renderer.lock().unwrap().render().unwrap();
        }
    }
}

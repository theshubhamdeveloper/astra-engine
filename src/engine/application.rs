use crate::engine::input::{Input, InputEvent};
use crate::engine::resource_manager::ResourceManager;
use crate::engine::text::{font::Font, textbuffer::TextBuffer};
use crate::engine::texture::GpuTexture;
use crate::engine::{Canvas, Color, Paint, Rect, Renderer, Time, Transform};
use glam::{Vec2, vec2};
use std::sync::{Arc, Mutex};
use winit::{
    application::ApplicationHandler,
    event::{ElementState, Ime, MouseScrollDelta, StartCause, TouchPhase, WindowEvent},
    event_loop::ActiveEventLoop,
    keyboard::PhysicalKey,
    window::{Window, WindowAttributes, WindowId},
};

pub struct Application {
    window: Option<Arc<Window>>,
    window_attributes: Option<WindowAttributes>,
    renderer: Option<Arc<Mutex<Renderer>>>,
    time: Time,
    input: Arc<Mutex<Input>>,
    canvas: Option<Canvas>,
    text_buffer: Option<TextBuffer>,
    texture_manager: Arc<Mutex<ResourceManager<GpuTexture>>>,
}

impl ApplicationHandler for Application {
    fn new_events(&mut self, _event_loop: &ActiveEventLoop, _cause: StartCause) {
        self.input.lock().unwrap().begin_frame();
    }

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
        text_buffer
            .set_line_height(Some(text_buffer.default_line_height() * 1.3))
            .unwrap();
        // text_buffer.set_italic(true);
        text_buffer.shape().unwrap();

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

            WindowEvent::KeyboardInput { event, .. } => {
                if let PhysicalKey::Code(key) = event.physical_key {
                    let input_event = match event.state {
                        ElementState::Pressed => InputEvent::KeyPressed {
                            key,
                            repeat: event.repeat,
                        },
                        ElementState::Released => InputEvent::KeyReleased { key },
                    };
                    self.input.lock().unwrap().update(&input_event);
                }

                if event.state == ElementState::Pressed {
                    if let Some(text) = event.text {
                        let mut input = self.input.lock().unwrap();
                        for character in text.chars() {
                            input.update(&InputEvent::TextInput { character });
                        }
                    }
                }
            }

            WindowEvent::Ime(Ime::Commit(text)) => {
                let mut input = self.input.lock().unwrap();
                for character in text.chars() {
                    input.update(&InputEvent::TextInput { character });
                }
            }

            WindowEvent::CursorMoved { position, .. } => {
                let position = vec2(position.x as f32, position.y as f32);
                let mut input = self.input.lock().unwrap();
                let delta = position - *input.mouse_position();
                input.update(&InputEvent::MouseMoved { position, delta });
            }

            WindowEvent::MouseInput { state, button, .. } => {
                let input_event = match state {
                    ElementState::Pressed => InputEvent::MouseButtonPressed { button },
                    ElementState::Released => InputEvent::MouseButtonReleased { button },
                };
                self.input.lock().unwrap().update(&input_event);
            }

            WindowEvent::MouseWheel { delta, .. } => {
                let delta = match delta {
                    MouseScrollDelta::LineDelta(x, y) => vec2(x * 40.0, y * 40.0),
                    MouseScrollDelta::PixelDelta(position) => {
                        vec2(position.x as f32, position.y as f32)
                    }
                };
                self.input
                    .lock()
                    .unwrap()
                    .update(&InputEvent::MouseWheel { delta });
            }

            WindowEvent::Touch(touch) => {
                let position = vec2(touch.location.x as f32, touch.location.y as f32);
                let input_event = match touch.phase {
                    TouchPhase::Started => InputEvent::TouchStarted {
                        id: touch.id,
                        position,
                    },
                    TouchPhase::Moved => InputEvent::TouchMoved {
                        id: touch.id,
                        position,
                    },
                    TouchPhase::Ended => InputEvent::TouchEnded { id: touch.id },
                    TouchPhase::Cancelled => InputEvent::TouchCancelled { id: touch.id },
                };
                self.input.lock().unwrap().update(&input_event);
            }

            WindowEvent::PinchGesture { delta, .. } => {
                self.input.lock().unwrap().update(&InputEvent::Pinch {
                    scale_delta: 1.0 + delta as f32,
                });
            }

            WindowEvent::PanGesture { delta, .. } => {
                self.input.lock().unwrap().update(&InputEvent::Pan {
                    delta: vec2(delta.x, delta.y),
                });
            }

            WindowEvent::RotationGesture { delta, .. } => {
                self.input.lock().unwrap().update(&InputEvent::Rotate {
                    delta_radians: delta.to_radians(),
                });
            }

            WindowEvent::DoubleTapGesture { .. } => {
                self.input.lock().unwrap().update(&InputEvent::DoubleTap);
            }

            WindowEvent::Focused(false) => {
                self.input.lock().unwrap().reset();
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
            input: Arc::new(Mutex::new(Input::default())),
            canvas: None,
            text_buffer: None,
            texture_manager: Arc::new(Mutex::new(ResourceManager::new())),
        }
    }

    pub fn input(&self) -> Arc<Mutex<Input>> {
        self.input.clone()
    }

    pub fn update(&mut self) {
        self.time.update();

        println!("{:?}", self.time.delta_time() * 1000.0);

        if let (Some(canvas), Some(text_buffer)) = (self.canvas.as_mut(), self.text_buffer.as_mut())
        {
            let paint = Paint {
                color: Color::BLACK,
                ..Paint::default()
            };

            let layout = text_buffer.get_layout().unwrap();

            canvas.draw_rect(
                &Rect {
                    width: layout.width,
                    height: layout.height,
                    corner_radius: [20.0, 0.0, 0.0, 0.0],
                },
                &Transform {
                    position: vec2(0.0, 0.0),
                    rotation: 0.0,
                    scale: Vec2::new(1.0, 1.0),
                },
                &Paint {
                    color: Color::GREEN,
                    ..Paint::default()
                },
            );

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
            let mut renderer = renderer.lock().unwrap();
            renderer.update_camera_behavior(&self.input.lock().unwrap());
            renderer.render().unwrap();
        }
    }
}

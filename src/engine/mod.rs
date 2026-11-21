mod application;
mod batch;
pub mod buffer;
mod camera;
mod canvas;
mod color;
mod draw_command;
mod paint;
mod render_pipeline;
mod renderer;
pub mod resource_manager;
mod shapes;
pub mod text;
mod texture;
pub mod texture_atlas;
mod time;
mod transform;
mod vertex;
pub mod input;

pub use application::Application;
pub use canvas::Canvas;
pub use color::Color;
pub use paint::Paint;
pub use renderer::Renderer;
pub use shapes::Rect;
pub use texture::GpuTexture;
pub use time::Time;
pub use transform::Transform;

use winit::error::EventLoopError;
use winit::event_loop::{ControlFlow, EventLoop};
use winit::window::WindowAttributes;

pub fn run() -> Result<(), EventLoopError> {
    let event_loop = EventLoop::new()?;

    event_loop.set_control_flow(ControlFlow::Poll);

    let mut app = Application::new(WindowAttributes::default().with_title("Astra Engine"));
    event_loop.run_app(&mut app)
}

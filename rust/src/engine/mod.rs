mod application;
mod batch;
mod camera;
mod canvas;
mod color;
mod draw_command;
mod paint;
mod render_pipeline;
mod renderer;
mod shapes;
mod texture;
mod time;
mod transform;
mod vertex;

pub use application::Application;
pub use canvas::Canvas;
pub use color::Color;
pub use paint::Paint;
pub use renderer::Renderer;
pub use shapes::Rect;
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

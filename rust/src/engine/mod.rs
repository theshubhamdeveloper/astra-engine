mod application;
mod renderer;

use winit::error::EventLoopError;
use winit::event_loop::{ControlFlow, EventLoop};
use winit::window::WindowAttributes;
use crate::engine::application::Application;

pub fn run() -> Result<(), EventLoopError> {
    let event_loop = EventLoop::new()?;

    event_loop.set_control_flow(ControlFlow::Poll);

    let mut app = Application::new(WindowAttributes::default().with_title("Astra Engine"));
    event_loop.run_app(&mut app)
}

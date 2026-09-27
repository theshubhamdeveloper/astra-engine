mod application;

use winit::error::EventLoopError;
use winit::event_loop::{ControlFlow, EventLoop};

use crate::engine::application::Application;

pub fn run() -> Result<(), EventLoopError> {
    let event_loop = EventLoop::new()?;

    event_loop.set_control_flow(ControlFlow::Poll);

    let mut app = Application::default();
    event_loop.run_app(&mut app)
}

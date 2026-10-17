use crate::engine::{paint::Paint, shapes::Rect};

pub enum DrawCommand {
    FillRect {
        rect: Rect,
        transform: glam::Mat3,
        paint: Paint,
    },
}

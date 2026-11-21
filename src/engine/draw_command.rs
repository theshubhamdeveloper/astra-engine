use crate::engine::{paint::Paint, shapes::Rect};
use crate::engine::text::textbuffer::TextLayout;

pub enum DrawCommand {
    FillRect {
        rect: Rect,
        transform: glam::Mat3,
        paint: Paint,
    },
    Text {
        layout: TextLayout,
        transform: glam::Mat3,
        paint: Paint,
    },
}

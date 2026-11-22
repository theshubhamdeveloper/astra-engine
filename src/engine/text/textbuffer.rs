use crate::engine::resource_manager::Handle;
use crate::engine::text::font::{Font, GlyphKey};
use crate::engine::texture_atlas::AtlasRegion;
use crate::engine::{Color, GpuTexture};
use std::sync::{Arc, Mutex};
use swash::GlyphId;
use swash::shape::{Direction, ShapeContext};
use swash::text::{Script, cluster::Whitespace};

#[derive(Clone, Copy, Debug)]
pub struct PositionedGlyph {
    pub atlas_region: AtlasRegion,
    pub x: f32,
    pub y: f32,
    pub italic_shear: f32,
}

#[derive(Clone, Default)]
pub struct TextLayout {
    pub texture: Option<Handle<GpuTexture>>,
    pub font: Option<Arc<Mutex<Font>>>,
    pub color: Option<Color>,
    pub glyphs: Vec<PositionedGlyph>,
    pub width: f32,
    pub height: f32,
}

struct ShapedCluster {
    glyphs: Vec<ShapedGlyph>,
    advance: f32,
    whitespace: Whitespace,
}

#[derive(Copy, Clone)]
struct ShapedGlyph {
    id: GlyphId,
    x: f32,
    y: f32,
    advance: f32,
}

enum TextToken {
    Word(Vec<ShapedCluster>),
    Whitespace(Vec<ShapedCluster>),
    Newline,
}

pub struct TextBuffer {
    font: Arc<Mutex<Font>>,
    text: String,
    size: f32,
    weight: u16,
    italic: bool,
    color: Option<Color>,
    line_height: Option<f32>,
    wrap_width: Option<f32>,
    text_layout: TextLayout,
    shape_context: ShapeContext,
    is_dirty: bool,
}

impl TextBuffer {
    pub fn new(font: Arc<Mutex<Font>>, size: f32) -> Self {
        Self {
            font,
            text: String::new(),
            size,
            weight: 400,
            italic: false,
            color: None,
            line_height: None,
            wrap_width: None,
            text_layout: TextLayout::default(),
            shape_context: ShapeContext::new(),
            is_dirty: false,
        }
    }
    pub fn set_text(&mut self, text: &str) {
        self.text = String::from(text);
        self.is_dirty = true;
    }

    pub fn set_weight(&mut self, weight: u16) {
        let weight = weight.clamp(1, 1000);
        if self.weight != weight {
            self.weight = weight;
            self.is_dirty = true;
        }
    }

    pub fn set_italic(&mut self, italic: bool) {
        if self.italic != italic {
            self.italic = italic;
            self.is_dirty = true;
        }
    }

    pub fn set_color(&mut self, color: Color) {
        self.color = Some(color);
    }

    pub fn clear_color(&mut self) {
        self.color = None;
    }

    pub fn color(&self) -> Option<Color> {
        self.color
    }

    pub fn set_line_height(&mut self, line_height: Option<f32>) -> anyhow::Result<()> {
        if line_height.is_some_and(|height| !height.is_finite() || height <= 0.0) {
            anyhow::bail!("line height must be a finite positive value");
        }

        if self.line_height != line_height {
            self.line_height = line_height;
            self.is_dirty = true;
        }
        Ok(())
    }

    pub fn set_wrap_width(&mut self, width: Option<f32>) -> anyhow::Result<()> {
        if width.is_some_and(|width| !width.is_finite() || width <= 0.0) {
            anyhow::bail!("wrap width must be a finite positive value");
        }

        if self.wrap_width != width {
            self.wrap_width = width;
            self.is_dirty = true;
        }
        Ok(())
    }

    pub fn font_size(&self) -> f32 {
        self.size
    }

    pub fn weight(&self) -> u16 {
        self.weight
    }

    pub fn is_italic(&self) -> bool {
        self.italic
    }

    pub fn default_line_height(&self) -> f32 {
        let metrics = self.font.lock().unwrap().as_ref().metrics(&[]);
        let scale = if metrics.units_per_em == 0 {
            0.0
        } else {
            self.size / metrics.units_per_em as f32
        };
        ((metrics.ascent - metrics.descent + metrics.leading) * scale).max(self.size)
    }

    pub fn line_height(&self) -> f32 {
        self.line_height
            .unwrap_or_else(|| self.default_line_height())
    }

    pub fn line_height_override(&self) -> Option<f32> {
        self.line_height
    }

    pub fn wrap_width(&self) -> Option<f32> {
        self.wrap_width
    }

    pub fn font(&self) -> &Arc<Mutex<Font>> {
        &self.font
    }

    pub fn size(&self) -> f32 {
        self.size
    }

    pub fn shape(&mut self) -> anyhow::Result<()> {
        if !self.is_dirty {
            return Ok(());
        }

        if self.text.is_empty() {
            self.text_layout.glyphs.clear();
            self.text_layout.width = 0.0;
            self.text_layout.height = 0.0;
            self.is_dirty = false;
            return Ok(());
        }

        let mut font = self.font.lock().unwrap();

        let synthesis = font.synthesis(self.weight, self.italic);
        let italic_shear = synthesis
            .skew()
            .map(|degrees| degrees.to_radians().tan())
            .unwrap_or(0.0);
        let mut shaper = self
            .shape_context
            .builder(font.as_ref())
            .script(Script::Latin)
            .direction(Direction::LeftToRight)
            .size(self.size)
            .variations(synthesis.variations().iter().copied())
            .build();

        shaper.add_str(self.text.as_str());

        let mut clusters = Vec::new();

        shaper.shape_with(|cluster| {
            clusters.push(ShapedCluster {
                glyphs: cluster
                    .glyphs
                    .iter()
                    .map(|glyph| ShapedGlyph {
                        id: glyph.id,
                        x: glyph.x,
                        y: glyph.y,
                        advance: glyph.advance,
                    })
                    .collect(),
                advance: cluster.advance(),
                whitespace: cluster.info.whitespace(),
            });
        });

        let mut tokens = Vec::new();
        let mut word = Vec::new();
        let mut whitespace = Vec::new();
        for cluster in clusters {
            match cluster.whitespace {
                Whitespace::Newline => {
                    if !word.is_empty() {
                        tokens.push(TextToken::Word(std::mem::take(&mut word)));
                    }
                    if !whitespace.is_empty() {
                        tokens.push(TextToken::Whitespace(std::mem::take(&mut whitespace)));
                    }
                    tokens.push(TextToken::Newline);
                }
                Whitespace::Space | Whitespace::Tab | Whitespace::Other => {
                    if !word.is_empty() {
                        tokens.push(TextToken::Word(std::mem::take(&mut word)));
                    }
                    whitespace.push(cluster);
                }
                Whitespace::None | Whitespace::NoBreakSpace => {
                    if !whitespace.is_empty() {
                        tokens.push(TextToken::Whitespace(std::mem::take(&mut whitespace)));
                    }
                    word.push(cluster);
                }
            }
        }
        if !word.is_empty() {
            tokens.push(TextToken::Word(word));
        }
        if !whitespace.is_empty() {
            tokens.push(TextToken::Whitespace(whitespace));
        }

        let metrics = font.as_ref().metrics(&[]);
        let scale = if metrics.units_per_em == 0 {
            0.0
        } else {
            self.size / metrics.units_per_em as f32
        };
        let ascent = metrics.ascent * scale;
        let default_line_height =
            ((metrics.ascent - metrics.descent + metrics.leading) * scale).max(self.size);
        let line_height = self.line_height.unwrap_or(default_line_height);
        let max_width = self
            .wrap_width
            .filter(|width| width.is_finite() && *width > 0.0);

        let mut lines: Vec<Vec<(ShapedGlyph, f32)>> = vec![Vec::new()];
        let mut line_width = 0.0;
        let mut max_line_width: f32 = 0.0;
        let mut line_has_content = false;
        let mut pending_whitespace = Vec::new();

        for token in tokens {
            match token {
                TextToken::Whitespace(clusters) => pending_whitespace.extend(clusters),
                TextToken::Newline => {
                    max_line_width = max_line_width.max(line_width);
                    lines.push(Vec::new());
                    line_width = 0.0;
                    line_has_content = false;
                    pending_whitespace.clear();
                }
                TextToken::Word(word) => {
                    let word_width: f32 = word.iter().map(|cluster| cluster.advance).sum();
                    let whitespace_width: f32 = pending_whitespace
                        .iter()
                        .map(|cluster| cluster.advance)
                        .sum();

                    if max_width.is_some_and(|width| {
                        line_has_content && line_width + whitespace_width + word_width > width
                    }) {
                        max_line_width = max_line_width.max(line_width);
                        lines.push(Vec::new());
                        line_width = 0.0;
                        line_has_content = false;
                        pending_whitespace.clear();
                    } else if max_width.is_some_and(|width| {
                        !line_has_content && whitespace_width + word_width > width
                    }) {
                        pending_whitespace.clear();
                    }

                    if word_width > max_width.unwrap_or(f32::INFINITY) {
                        if line_has_content {
                            max_line_width = max_line_width.max(line_width);
                            lines.push(Vec::new());
                            line_width = 0.0;
                            line_has_content = false;
                        }
                        pending_whitespace.clear();

                        for cluster in word {
                            if max_width.is_some_and(|width| {
                                line_has_content && line_width + cluster.advance > width
                            }) {
                                max_line_width = max_line_width.max(line_width);
                                lines.push(Vec::new());
                                line_width = 0.0;
                            }
                            append_cluster_to_line(&mut lines, &mut line_width, &cluster);
                            line_has_content = true;
                        }
                    } else {
                        for cluster in pending_whitespace.drain(..) {
                            append_cluster_to_line(&mut lines, &mut line_width, &cluster);
                        }
                        for cluster in word {
                            append_cluster_to_line(&mut lines, &mut line_width, &cluster);
                        }
                        line_has_content = true;
                    }
                }
            }
        }

        for cluster in pending_whitespace {
            if max_width.is_none_or(|width| line_width + cluster.advance <= width) {
                append_cluster_to_line(&mut lines, &mut line_width, &cluster);
            }
        }
        max_line_width = max_line_width.max(line_width);

        let mut positioned_glyphs = Vec::new();
        for (line_index, line) in lines.iter().enumerate() {
            let line_y = line_index as f32 * line_height;
            for (glyph, x) in line {
                let glyph_image = font.get_glyph(&GlyphKey {
                    size: self.size as u32,
                    glyph_id: glyph.id,
                    weight: self.weight,
                    italic: self.italic,
                })?;
                positioned_glyphs.push(PositionedGlyph {
                    atlas_region: glyph_image.region,
                    x: *x + glyph_image.left as f32,
                    y: -line_y - ascent + glyph_image.top as f32 + glyph.y,
                    italic_shear,
                });
            }
        }

        self.text_layout.glyphs = positioned_glyphs;
        self.text_layout.width = max_line_width;
        self.text_layout.height = lines.len() as f32 * line_height;
        self.is_dirty = false;

        Ok(())
    }

    pub fn get_layout(&mut self) -> anyhow::Result<&TextLayout> {
        self.shape()?;

        let font = self.font.lock().unwrap();
        self.text_layout.texture = font.atlas_gpu_texture;
        self.text_layout.color = self.color;
        self.text_layout.font = Some(self.font.clone());
        Ok(&self.text_layout)
    }

    pub fn layout_width(&mut self) -> anyhow::Result<f32> {
        self.shape()?;
        Ok(self.text_layout.width)
    }

    pub fn layout_height(&mut self) -> anyhow::Result<f32> {
        self.shape()?;
        Ok(self.text_layout.height)
    }
}

fn append_cluster_to_line(
    lines: &mut [Vec<(ShapedGlyph, f32)>],
    line_width: &mut f32,
    cluster: &ShapedCluster,
) {
    let mut cluster_pen = 0.0;
    for glyph in &cluster.glyphs {
        lines
            .last_mut()
            .unwrap()
            .push((*glyph, *line_width + cluster_pen + glyph.x));
        cluster_pen += glyph.advance;
    }
    *line_width += cluster.advance;
}

#[cfg(test)]
mod tests {
    use super::*;

    fn layout_text(text: &str, width: Option<f32>) -> TextLayout {
        let font = Arc::new(Mutex::new(
            Font::from_bytes(
                include_bytes!("../../../assets/fonts/JetBrainsMono/JetBrainsMono[wght].ttf")
                    .to_vec(),
                0,
            )
            .unwrap(),
        ));
        let mut buffer = TextBuffer::new(font, 16.0);
        buffer.set_wrap_width(width).unwrap();
        buffer.set_text(text);
        buffer.get_layout().unwrap().clone()
    }

    #[test]
    fn wraps_at_word_boundaries_and_reports_max_line_width() {
        let unwrapped = layout_text("one two three", None);
        let wrapped = layout_text("one two three", Some(55.0));

        assert!(wrapped.height > unwrapped.height);
        assert!(wrapped.width <= 55.0);
    }

    #[test]
    fn wrap_width_can_be_changed_after_construction() {
        let font = Arc::new(Mutex::new(
            Font::from_bytes(
                include_bytes!("../../../assets/fonts/JetBrainsMono/JetBrainsMono[wght].ttf")
                    .to_vec(),
                0,
            )
            .unwrap(),
        ));
        let mut buffer = TextBuffer::new(font, 16.0);
        buffer.set_text("one two three");
        let unwrapped_height = buffer.layout_height().unwrap();

        buffer.set_wrap_width(Some(55.0)).unwrap();

        assert!(buffer.layout_height().unwrap() > unwrapped_height);
        assert!(buffer.layout_width().unwrap() <= 55.0);
        assert!(buffer.set_wrap_width(Some(0.0)).is_err());
        assert!(buffer.set_wrap_width(Some(f32::NAN)).is_err());
    }

    #[test]
    fn wraps_long_words_at_glyph_cluster_boundaries() {
        let wrapped = layout_text("extraordinary", Some(24.0));

        assert!(wrapped.height > 16.0);
        assert!(wrapped.width <= 24.0);
    }

    #[test]
    fn respects_explicit_newlines() {
        let single_line = layout_text("one two", None);
        let multiple_lines = layout_text("one\ntwo", None);

        assert!(multiple_lines.height > single_line.height);
    }

    #[test]
    fn italic_style_is_applied_to_positioned_glyphs() {
        let font = Arc::new(Mutex::new(
            Font::from_bytes(
                include_bytes!("../../../assets/fonts/JetBrainsMono/JetBrainsMono[wght].ttf")
                    .to_vec(),
                0,
            )
            .unwrap(),
        ));
        let mut buffer = TextBuffer::new(font, 16.0);
        buffer.set_text("style");
        let regular = buffer.get_layout().unwrap().clone();
        buffer.set_weight(700);
        buffer.set_italic(true);

        let layout = buffer.get_layout().unwrap();

        assert!(layout.glyphs[0].italic_shear > regular.glyphs[0].italic_shear);
        assert!(layout.glyphs.iter().all(|glyph| glyph.italic_shear > 0.0));
    }

    #[test]
    fn custom_line_height_controls_layout_height_and_can_be_reset() {
        let font = Arc::new(Mutex::new(
            Font::from_bytes(
                include_bytes!("../../../assets/fonts/JetBrainsMono/JetBrainsMono[wght].ttf")
                    .to_vec(),
                0,
            )
            .unwrap(),
        ));
        let mut buffer = TextBuffer::new(font, 16.0);
        buffer.set_text("first\nsecond");

        let default_height = buffer.get_layout().unwrap().height;
        buffer.set_line_height(Some(40.0)).unwrap();
        let custom_height = buffer.get_layout().unwrap().height;

        assert!(custom_height > default_height);
        assert!((custom_height - 80.0).abs() < 1e-4);

        buffer.set_line_height(None).unwrap();
        assert!((buffer.get_layout().unwrap().height - default_height).abs() < 1e-4);
    }

    #[test]
    fn rejects_invalid_custom_line_height() {
        let font = Arc::new(Mutex::new(
            Font::from_bytes(
                include_bytes!("../../../assets/fonts/JetBrainsMono/JetBrainsMono[wght].ttf")
                    .to_vec(),
                0,
            )
            .unwrap(),
        ));
        let mut buffer = TextBuffer::new(font, 16.0);

        assert!(buffer.set_line_height(Some(0.0)).is_err());
        assert!(buffer.set_line_height(Some(f32::INFINITY)).is_err());
    }

    #[test]
    fn exposes_text_properties_and_measured_layout_size() {
        let font = Arc::new(Mutex::new(
            Font::from_bytes(
                include_bytes!("../../../assets/fonts/JetBrainsMono/JetBrainsMono[wght].ttf")
                    .to_vec(),
                0,
            )
            .unwrap(),
        ));
        let mut buffer = TextBuffer::new(font, 16.0);
        buffer.set_wrap_width(Some(120.0)).unwrap();
        buffer.set_text("getter test");
        buffer.set_weight(700);
        buffer.set_italic(true);
        buffer.set_color(Color::RED);
        buffer.set_line_height(Some(32.0)).unwrap();

        assert_eq!(buffer.font_size(), 16.0);
        assert_eq!(buffer.weight(), 700);
        assert!(buffer.is_italic());
        assert_eq!(buffer.color().unwrap().as_array(), Color::RED.as_array());
        assert_eq!(buffer.line_height(), 32.0);
        assert_eq!(buffer.line_height_override(), Some(32.0));
        assert_eq!(buffer.wrap_width(), Some(120.0));
        assert!(buffer.layout_width().unwrap() > 0.0);
        assert_eq!(buffer.layout_height().unwrap(), 32.0);
        assert_eq!(
            buffer.get_layout().unwrap().color.unwrap().as_array(),
            Color::RED.as_array()
        );
        buffer.clear_color();
        assert!(buffer.get_layout().unwrap().color.is_none());
    }

    #[test]
    fn marks_atlas_dirty_when_shaping_adds_new_glyphs() {
        let font = Arc::new(Mutex::new(
            Font::from_bytes(
                include_bytes!("../../../assets/fonts/JetBrainsMono/JetBrainsMono[wght].ttf")
                    .to_vec(),
                0,
            )
            .unwrap(),
        ));
        let mut buffer = TextBuffer::new(font.clone(), 16.0);

        buffer.set_text("A");
        buffer.shape().unwrap();
        assert!(font.lock().unwrap().is_atlas_dirty());

        font.lock().unwrap().mark_atlas_uploaded();
        buffer.set_text("B");
        buffer.shape().unwrap();
        assert!(font.lock().unwrap().is_atlas_dirty());
    }
}

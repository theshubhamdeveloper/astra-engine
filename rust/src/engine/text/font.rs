use crate::engine::GpuTexture;
use crate::engine::resource_manager::Handle;
use crate::engine::texture_atlas::{AtlasRegion, TextureAtlas};
use anyhow::bail;
use std::collections::HashMap;
use swash::scale::{Render, ScaleContext, Source, StrikeWith};
use swash::zeno::Format;
use swash::{Attributes, CacheKey, Charmap, FontRef, GlyphId, Stretch, Style, Synthesis, Weight};

const GLYPH_SOURCES: &[Source] = &[Source::Outline, Source::Bitmap(StrikeWith::BestFit)];

#[derive(Copy, Clone, Hash, Eq, PartialEq)]
pub struct GlyphKey {
    pub glyph_id: GlyphId,
    pub size: u32,
    pub weight: u16,
    pub italic: bool,
}

pub struct Font {
    data: Vec<u8>,
    offset: u32,
    key: CacheKey,

    pub atlas_gpu_texture: Option<Handle<GpuTexture>>,
    atlas: TextureAtlas,

    glyphs: HashMap<GlyphKey, GlyphAtlasEntry>,
    scale_context: ScaleContext,
}

#[derive(Copy, Clone)]
pub struct GlyphAtlasEntry {
    pub region: AtlasRegion,
    pub left: i32,
    pub top: i32,
}

impl Font {
    pub fn from_bytes(data: Vec<u8>, index: usize) -> Option<Self> {
        let font = FontRef::from_index(&data, index).unwrap();

        let (offset, key) = (font.offset, font.key);

        let atlas = TextureAtlas::with_padding(1024, 1024, 2).unwrap();

        Some(Self {
            data,
            offset,
            key,
            atlas_gpu_texture: None,
            atlas,
            glyphs: HashMap::new(),
            scale_context: ScaleContext::new(),
        })
    }

    pub fn get_glyph(&mut self, glyph_key: &GlyphKey) -> anyhow::Result<&GlyphAtlasEntry> {
        if !self.glyphs.contains_key(glyph_key) {
            let glyph = self.rasterize_glyph(glyph_key)?;
            self.glyphs.insert(*glyph_key, glyph);
        }
        Ok(self.glyphs.get(glyph_key).unwrap())
    }

    fn rasterize_glyph(&mut self, glyph_key: &GlyphKey) -> anyhow::Result<GlyphAtlasEntry> {
        let font = FontRef {
            data: &self.data,
            offset: self.offset,
            key: self.key,
        };
        let attributes = font.attributes();
        let synthesis = attributes.synthesize(Attributes::new(
            Stretch::NORMAL,
            Weight(glyph_key.weight),
            if glyph_key.italic {
                Style::Italic
            } else {
                Style::Normal
            },
        ));

        let mut scaler = self
            .scale_context
            .builder(font)
            .size(glyph_key.size as f32)
            .variations(synthesis.variations().iter().copied())
            .hint(true)
            .build();

        let mut renderer = Render::new(GLYPH_SOURCES);
        renderer.format(Format::Alpha);

        let image = renderer
            .render(&mut scaler, glyph_key.glyph_id)
            .ok_or_else(|| anyhow::anyhow!("could not rasterize glyph {}", glyph_key.glyph_id))?;

        let mut placement = image.placement;
        let mut alpha_data = image.data;
        let expected_len = (placement.width as usize)
            .checked_mul(placement.height as usize)
            .ok_or_else(|| anyhow::anyhow!("glyph image dimensions are too large"))?;
        if alpha_data.len() != expected_len {
            bail!("glyph rasterizer returned unexpected alpha data");
        }

        if synthesis.embolden() && placement.width > 0 && placement.height > 0 {
            let strength =
                ((glyph_key.weight.saturating_sub(attributes.weight().0)) / 250).clamp(1, 3) as u32;
            let expanded_width = placement
                .width
                .checked_add(strength)
                .ok_or_else(|| anyhow::anyhow!("emboldened glyph is too wide"))?;
            let expanded_len = (expanded_width as usize)
                .checked_mul(placement.height as usize)
                .ok_or_else(|| anyhow::anyhow!("emboldened glyph is too large"))?;
            let mut expanded = vec![0; expanded_len];
            for y in 0..placement.height as usize {
                for x in 0..placement.width as usize {
                    let alpha = alpha_data[y * placement.width as usize + x];
                    for offset in 0..=strength as usize {
                        let target_x = x + offset;
                        let target = y * expanded_width as usize + target_x;
                        expanded[target] = expanded[target].max(alpha);
                    }
                }
            }
            placement.width = expanded_width;
            alpha_data = expanded;
        }

        let region = if placement.width == 0 || placement.height == 0 {
            AtlasRegion::default()
        } else {
            let pixel_count = alpha_data.len();
            let rgba_len = pixel_count
                .checked_mul(4)
                .ok_or_else(|| anyhow::anyhow!("glyph image dimensions are too large"))?;
            let mut rgba = Vec::with_capacity(rgba_len);
            for alpha in alpha_data {
                rgba.extend_from_slice(&[255, 255, 255, alpha]);
            }

            self.atlas
                .insert_rgba(placement.width, placement.height, &rgba)?
        };

        Ok(GlyphAtlasEntry {
            region,
            left: placement.left,
            top: placement.top,
        })
    }

    pub fn atlas(&self) -> &TextureAtlas {
        &self.atlas
    }

    pub fn attributes(&self) -> Attributes {
        self.as_ref().attributes()
    }

    pub fn synthesis(&self, weight: u16, italic: bool) -> Synthesis {
        let attributes = self.attributes();
        attributes.synthesize(Attributes::new(
            Stretch::NORMAL,
            Weight(weight),
            if italic { Style::Italic } else { Style::Normal },
        ))
    }

    pub fn charmap(&self) -> Charmap<'_> {
        self.as_ref().charmap()
    }

    pub fn cache_key(&self) -> CacheKey {
        self.key
    }

    pub fn as_ref(&self) -> FontRef<'_> {
        FontRef {
            data: &self.data,
            offset: self.offset,
            key: self.key,
        }
    }
}

use super::texture::GpuTexture;
use anyhow::{Result, bail};
use image::{DynamicImage, RgbaImage};

#[derive(Clone, Copy, Debug, PartialEq, Eq, Default)]
pub struct AtlasRegion {
    pub x: u32,
    pub y: u32,
    pub width: u32,
    pub height: u32,
}

impl AtlasRegion {
    pub fn uv_min(self, atlas_width: u32, atlas_height: u32) -> [f32; 2] {
        [
            self.x as f32 / atlas_width as f32,
            self.y as f32 / atlas_height as f32,
        ]
    }

    pub fn uv_max(self, atlas_width: u32, atlas_height: u32) -> [f32; 2] {
        [
            (self.x + self.width) as f32 / atlas_width as f32,
            (self.y + self.height) as f32 / atlas_height as f32,
        ]
    }
}

pub struct TextureAtlas {
    width: u32,
    height: u32,
    padding: u32,
    next_x: u32,
    next_y: u32,
    row_height: u32,
    pixels: Vec<u8>,
}

impl TextureAtlas {
    pub fn new(width: u32, height: u32) -> Result<Self> {
        Self::with_padding(width, height, 1)
    }

    pub fn with_padding(width: u32, height: u32, padding: u32) -> Result<Self> {
        if width == 0 || height == 0 {
            bail!("texture atlas dimensions must be non-zero");
        }

        let pixel_count = (width as usize)
            .checked_mul(height as usize)
            .and_then(|count| count.checked_mul(4))
            .ok_or_else(|| anyhow::anyhow!("texture atlas dimensions are too large"))?;

        let mut pixels = Vec::new();
        pixels
            .try_reserve_exact(pixel_count)
            .map_err(|error| anyhow::anyhow!("could not allocate texture atlas: {error}"))?;
        pixels.resize(pixel_count, 0);

        Ok(Self {
            width,
            height,
            padding,
            next_x: 0,
            next_y: 0,
            row_height: 0,
            pixels,
        })
    }

    pub fn width(&self) -> u32 {
        self.width
    }

    pub fn height(&self) -> u32 {
        self.height
    }

    pub fn pixels(&self) -> &[u8] {
        &self.pixels
    }

    pub fn to_image(&self) -> RgbaImage {
        RgbaImage::from_raw(self.width, self.height, self.pixels.clone())
            .expect("atlas pixel buffer always matches its dimensions")
    }

    pub fn to_gpu_texture(
        &self,
        device: &wgpu::Device,
        queue: &wgpu::Queue,
        label: &str,
        texture_bind_group_layout: &wgpu::BindGroupLayout,
    ) -> Result<GpuTexture> {
        GpuTexture::from_rgba8(
            device,
            queue,
            self.width,
            self.height,
            &self.pixels,
            Some(label),
            texture_bind_group_layout,
        )
    }

    pub fn insert_image(&mut self, image: &DynamicImage) -> Result<AtlasRegion> {
        let rgba = image.to_rgba8();
        self.insert_rgba(rgba.width(), rgba.height(), rgba.as_raw())
    }

    pub fn insert_rgba(&mut self, width: u32, height: u32, rgba: &[u8]) -> Result<AtlasRegion> {
        if width == 0 || height == 0 {
            bail!("cannot insert a zero-sized image into a texture atlas");
        }

        let expected_len = (width as usize)
            .checked_mul(height as usize)
            .and_then(|count| count.checked_mul(4))
            .ok_or_else(|| anyhow::anyhow!("image dimensions are too large"))?;
        if rgba.len() != expected_len {
            bail!(
                "RGBA data length mismatch: expected {expected_len} bytes, got {}",
                rgba.len()
            );
        }

        let padding = self
            .padding
            .checked_mul(2)
            .ok_or_else(|| anyhow::anyhow!("texture atlas padding is too large"))?;
        let packed_width = width
            .checked_add(padding)
            .ok_or_else(|| anyhow::anyhow!("image is too wide for texture atlas"))?;
        let packed_height = height
            .checked_add(padding)
            .ok_or_else(|| anyhow::anyhow!("image is too tall for texture atlas"))?;
        if packed_width > self.width || packed_height > self.height {
            bail!("image does not fit in texture atlas");
        }

        let starts_new_row =
            u64::from(self.next_x) + u64::from(packed_width) > u64::from(self.width);
        let (shelf_x, shelf_y) = if starts_new_row {
            let y = self
                .next_y
                .checked_add(self.row_height)
                .ok_or_else(|| anyhow::anyhow!("texture atlas is full"))?;
            if u64::from(y) + u64::from(packed_height) > u64::from(self.height) {
                bail!("texture atlas is full");
            }
            (0, y)
        } else {
            if u64::from(self.next_y) + u64::from(packed_height) > u64::from(self.height) {
                bail!("texture atlas is full");
            }
            (self.next_x, self.next_y)
        };

        let x = shelf_x + self.padding;
        let y = shelf_y + self.padding;
        let source_stride = width as usize * 4;
        let target_stride = self.width as usize * 4;
        for row in 0..height as usize {
            let source_start = row * source_stride;
            let target_start = (y as usize + row) * target_stride + x as usize * 4;
            self.pixels[target_start..target_start + source_stride]
                .copy_from_slice(&rgba[source_start..source_start + source_stride]);
        }

        self.next_x = shelf_x + packed_width;
        self.next_y = shelf_y;
        self.row_height = if starts_new_row || self.next_x == packed_width {
            packed_height
        } else {
            self.row_height.max(packed_height)
        };

        Ok(AtlasRegion {
            x,
            y,
            width,
            height,
        })
    }
}

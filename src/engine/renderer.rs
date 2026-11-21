use crate::engine::texture_atlas::TextureAtlas;
use crate::{
    engine::batch::Batch,
    engine::buffer::GpuBuffer,
    engine::camera::Camera,
    engine::draw_command::DrawCommand,
    engine::input::Input,
    engine::render_pipeline::RenderPipeline,
    engine::resource_manager::{Handle, ResourceManager},
    engine::texture::GpuTexture,
    engine::vertex::Vertex,
};
use glam::Vec2;
use std::ops::Range;
use std::sync::{Arc, Mutex};
use wgpu::RenderPass;
use winit::{event::MouseButton, window::Window};

pub struct Renderer {
    surface: wgpu::Surface<'static>,
    device: wgpu::Device,
    queue: wgpu::Queue,

    config: wgpu::SurfaceConfiguration,

    is_surface_configured: bool,

    window: Arc<Window>,

    batch: Batch,
    draw_ranges: Vec<DrawRange>,

    render_pipeline: RenderPipeline,

    camera_bind_group_layout: wgpu::BindGroupLayout,
    texture_bind_group_layout: wgpu::BindGroupLayout,

    dummy_texture: Handle<GpuTexture>,

    camera_bind_group: wgpu::BindGroup,

    pub draw_commands: Vec<DrawCommand>,

    camera: Camera,
    camera_buffer: GpuBuffer,

    texture_manager: Arc<Mutex<ResourceManager<GpuTexture>>>,
}

struct DrawRange {
    texture: Handle<GpuTexture>,
    indices: Range<u32>,
}

impl Renderer {
    pub async fn new(
        window: Arc<Window>,
        texture_manager: Arc<Mutex<ResourceManager<GpuTexture>>>,
    ) -> anyhow::Result<Self> {
        let size = window.inner_size();

        let camera = Camera::new(
            Vec2::new(size.width as f32, size.height as f32),
            Vec2::new(0.0, 0.0),
            1.0,
        );

        let instance = wgpu::Instance::default();

        let surface = instance.create_surface(window.clone())?;

        let adapter = instance
            .request_adapter(&wgpu::RequestAdapterOptions {
                power_preference: Default::default(),
                force_fallback_adapter: false,
                compatible_surface: Some(&surface),
                apply_limit_buckets: false,
            })
            .await?;

        println!(
            "Using GPU: {:?},\ntexture limits: {:?} ",
            adapter.get_info().name,
            adapter.limits().max_texture_dimension_2d
        );

        let (device, queue) = adapter
            .request_device(&wgpu::DeviceDescriptor::default())
            .await?;

        let capabilities = surface.get_capabilities(&adapter);

        let format = capabilities
            .formats
            .iter()
            .find(|f| f.is_srgb())
            .copied()
            .unwrap_or(capabilities.formats[0]);

        let config = wgpu::SurfaceConfiguration {
            usage: wgpu::TextureUsages::RENDER_ATTACHMENT,
            format,
            width: size.width,
            height: size.height,
            present_mode: capabilities.present_modes[0],
            alpha_mode: capabilities.alpha_modes[0],
            view_formats: vec![],
            desired_maximum_frame_latency: 2,
            color_space: Default::default(),
        };

        let texture_bind_group_layout =
            device.create_bind_group_layout(&wgpu::BindGroupLayoutDescriptor {
                entries: &[
                    wgpu::BindGroupLayoutEntry {
                        binding: 0,
                        visibility: wgpu::ShaderStages::FRAGMENT,
                        ty: wgpu::BindingType::Texture {
                            multisampled: false,
                            view_dimension: wgpu::TextureViewDimension::D2,
                            sample_type: wgpu::TextureSampleType::Float { filterable: true },
                        },
                        count: None,
                    },
                    wgpu::BindGroupLayoutEntry {
                        binding: 1,
                        visibility: wgpu::ShaderStages::FRAGMENT,
                        ty: wgpu::BindingType::Sampler(wgpu::SamplerBindingType::Filtering),
                        count: None,
                    },
                ],
                label: Some("texture_bind_group_layout"),
            });

        let dummy_texture = texture_manager
            .lock()
            .unwrap()
            .insert(GpuTexture::from_bytes(
                &device,
                &queue,
                include_bytes!("../../assets/images/dummy.png"),
                "dummy.png",
                &texture_bind_group_layout,
            )?);

        let batch = Batch::new(
            &device,
            1000 * size_of::<Vertex>() as u64,
            6000 * size_of::<u32>() as u64,
        );

        let camera_bind_group_layout =
            device.create_bind_group_layout(&wgpu::BindGroupLayoutDescriptor {
                entries: &[wgpu::BindGroupLayoutEntry {
                    binding: 0,
                    visibility: wgpu::ShaderStages::VERTEX,
                    ty: wgpu::BindingType::Buffer {
                        ty: wgpu::BufferBindingType::Uniform,
                        has_dynamic_offset: false,
                        min_binding_size: None,
                    },
                    count: None,
                }],
                label: Some("camera_bind_group_layout"),
            });

        let render_pipeline = RenderPipeline::new(
            &device,
            &format,
            wgpu::include_wgsl!("../../assets/shaders/shader.wgsl"),
            &texture_bind_group_layout,
            &camera_bind_group_layout,
        );

        let camera_buffer = GpuBuffer::new(
            &device,
            size_of::<glam::Mat4>() as wgpu::BufferAddress,
            wgpu::BufferUsages::UNIFORM | wgpu::BufferUsages::COPY_DST,
            "Camera Uniform Buffer",
            true,
        );

        let camera_bind_group = device.create_bind_group(&wgpu::BindGroupDescriptor {
            layout: &camera_bind_group_layout,
            entries: &[wgpu::BindGroupEntry {
                binding: 0,
                resource: camera_buffer.buffer.as_entire_binding(),
            }],
            label: Some("camera_bind_group"),
        });

        Ok(Self {
            surface,
            device,
            queue,
            config,
            is_surface_configured: false,
            window,
            batch,
            draw_ranges: vec![],
            render_pipeline,
            camera_bind_group_layout,
            texture_bind_group_layout,
            dummy_texture,
            camera_bind_group,
            draw_commands: vec![],
            camera,
            camera_buffer,
            texture_manager,
        })
    }

    pub fn resize(&mut self, width: u32, height: u32) {
        if width == 0 || height == 0 {
            return;
        };

        self.config.width = width;
        self.config.height = height;
        self.surface.configure(&self.device, &self.config);
        self.is_surface_configured = true;

        self.camera.screen_size = Vec2::new(width as f32, height as f32);
        self.camera.update_matrix();

        self.camera_buffer.write(
            &self.device,
            &self.queue,
            0,
            bytemuck::bytes_of(&self.camera.projection),
        )
    }

    pub fn update_camera_behavior(&mut self, input: &Input) {
        if input.was_double_tapped() {
            self.camera.reset();
        } else {
            let mut changed = false;

            if input.is_mouse_down(MouseButton::Middle) || input.is_mouse_down(MouseButton::Right) {
                let delta = *input.mouse_delta();
                if delta != Vec2::ZERO {
                    self.camera.pan_screen(delta);
                    changed = true;
                }
            }

            if input.touch_count() > 0 {
                let delta = *input.touch_pan_delta();
                if delta != Vec2::ZERO {
                    self.camera.pan_screen(delta);
                    changed = true;
                }
            }

            let scroll = *input.scroll_delta();
            if scroll != Vec2::ZERO {
                self.camera.pan_screen(scroll);
                changed = true;
            }

            let pinch_scale = input.pinch_scale_delta();
            if pinch_scale != 1.0 {
                let anchor = input
                    .touches()
                    .values()
                    .copied()
                    .reduce(|center, position| center + position)
                    .map(|sum| sum / input.touch_count() as f32)
                    .unwrap_or(*input.mouse_position());
                self.camera.zoom_at(anchor, pinch_scale);
                changed = true;
            }

            let rotation_delta = input.rotation_delta();
            if rotation_delta != 0.0 {
                self.camera.rotate(rotation_delta);
                changed = true;
            }

            if changed {
                self.write_camera_matrix();
            }
        }

        if input.was_double_tapped() {
            self.write_camera_matrix();
        }
    }

    fn write_camera_matrix(&mut self) {
        self.camera_buffer.write(
            &self.device,
            &self.queue,
            0,
            bytemuck::bytes_of(&self.camera.projection),
        );
    }

    fn serialize_commands(&mut self) {
        self.batch.clear();
        self.draw_ranges.clear();
        let commands = std::mem::take(&mut self.draw_commands);

        for cmd in commands {
            match cmd {
                DrawCommand::FillRect {
                    rect,
                    transform,
                    paint,
                } => {
                    let points = [
                        Vec2::new(0.0, 0.0),
                        Vec2::new(rect.width, 0.0),
                        Vec2::new(rect.width, rect.height),
                        Vec2::new(0.0, rect.height),
                    ]
                    .map(|point| transform.transform_point2(point));
                    self.push_quad(
                        points,
                        [[0.0, 0.0], [1.0, 0.0], [1.0, 1.0], [0.0, 1.0]],
                        paint.color.as_array(),
                        paint.texture_id.unwrap_or(self.dummy_texture),
                        paint.texture_id.is_some(),
                    );
                }
                DrawCommand::Text {
                    layout,
                    transform,
                    paint,
                } => {
                    let Some(texture) = layout.texture else {
                        continue;
                    };

                    for glyph in &layout.glyphs {
                        let x = glyph.x;
                        let y = -glyph.y;
                        let width = glyph.atlas_region.width as f32;
                        let height = glyph.atlas_region.height as f32;
                        let italic_offset = height * glyph.italic_shear;

                        if width == 0.0 && height == 0.0 {
                            continue;
                        }

                        let points = [
                            Vec2::new(x + italic_offset, y),
                            Vec2::new(x + width + italic_offset, y),
                            Vec2::new(x + width, y + height),
                            Vec2::new(x, y + height),
                        ]
                        .map(|point| transform.transform_point2(point));

                        let uv_min = glyph.atlas_region.uv_min(1024, 1024);
                        let uv_max = glyph.atlas_region.uv_max(1024, 1024);

                        self.push_quad(
                            points,
                            [
                                uv_min,
                                [uv_max[0], uv_min[1]],
                                uv_max,
                                [uv_min[0], uv_max[1]],
                            ],
                            layout.color.unwrap_or(paint.color).as_array(),
                            texture,
                            true,
                        );
                    }
                }
            }
        }
    }

    fn push_quad(
        &mut self,
        positions: [Vec2; 4],
        tex_coords: [[f32; 2]; 4],
        color: [u8; 4],
        texture: Handle<GpuTexture>,
        use_tex: bool,
    ) {
        let base_vertex = self.batch.vertices.len() as u32;
        self.batch.vertices.extend((0..4).map(|index| Vertex {
            position: positions[index].to_array(),
            tex_coords: tex_coords[index],
            use_tex: u32::from(use_tex),
            color,
        }));

        let start = self.batch.indices.len() as u32;
        self.batch.indices.extend([
            base_vertex,
            base_vertex + 2,
            base_vertex + 1,
            base_vertex,
            base_vertex + 3,
            base_vertex + 2,
        ]);
        let end = self.batch.indices.len() as u32;

        if let Some(range) = self.draw_ranges.last_mut()
            && range.texture == texture
        {
            range.indices.end = end;
        } else {
            self.draw_ranges.push(DrawRange {
                texture,
                indices: start..end,
            });
        }
    }

    fn flush_batch(&mut self, render_pass: &mut RenderPass) {
        let texture_manager_safe = &self.texture_manager.lock().unwrap();

        render_pass.set_pipeline(&self.render_pipeline.pipeline);
        render_pass.set_bind_group(1, &self.camera_bind_group, &[]);
        render_pass.set_vertex_buffer(0, self.batch.vertex_buffer.buffer.slice(..));
        render_pass.set_index_buffer(
            self.batch.index_buffer.buffer.slice(..),
            wgpu::IndexFormat::Uint32,
        );
        for range in &self.draw_ranges {
            let Some(texture) = texture_manager_safe.get(range.texture) else {
                continue;
            };
            render_pass.set_bind_group(0, texture.bind_group(), &[]);
            render_pass.draw_indexed(range.indices.clone(), 0, 0..1);
        }
    }

    pub fn upload_atlas(&mut self, atlas: &TextureAtlas) -> anyhow::Result<Handle<GpuTexture>> {
        Ok(self
            .texture_manager
            .lock()
            .unwrap()
            .insert(atlas.to_gpu_texture(
                &self.device,
                &self.queue,
                "atlas",
                &self.texture_bind_group_layout,
            )?))
    }

    pub fn render(&mut self) -> anyhow::Result<()> {
        if !self.is_surface_configured {
            return Ok(());
        }

        self.serialize_commands();

        let output = match self.surface.get_current_texture() {
            wgpu::CurrentSurfaceTexture::Success(surface_texture) => surface_texture,
            wgpu::CurrentSurfaceTexture::Suboptimal(surface_texture) => surface_texture,
            wgpu::CurrentSurfaceTexture::Timeout
            | wgpu::CurrentSurfaceTexture::Occluded
            | wgpu::CurrentSurfaceTexture::Validation => {
                return Ok(());
            }
            wgpu::CurrentSurfaceTexture::Outdated => {
                self.surface.configure(&self.device, &self.config);
                return Ok(());
            }
            wgpu::CurrentSurfaceTexture::Lost => {
                anyhow::bail!("Lost device");
            }
        };

        let view = output
            .texture
            .create_view(&wgpu::TextureViewDescriptor::default());

        let mut encoder = self
            .device
            .create_command_encoder(&wgpu::CommandEncoderDescriptor {
                label: Some("Render Encoder"),
            });

        let mut render_pass = encoder.begin_render_pass(&wgpu::RenderPassDescriptor {
            label: Some("Render Pass"),
            color_attachments: &[Some(wgpu::RenderPassColorAttachment {
                view: &view,
                resolve_target: None,
                depth_slice: None,
                ops: wgpu::Operations {
                    load: wgpu::LoadOp::Clear(wgpu::Color {
                        r: 1.0,
                        g: 1.0,
                        b: 1.0,
                        a: 1.0,
                    }),
                    store: wgpu::StoreOp::Store,
                },
            })],
            depth_stencil_attachment: None,
            occlusion_query_set: None,
            timestamp_writes: None,
            multiview_mask: None,
        });

        self.batch.update(&self.device, &self.queue);

        self.flush_batch(&mut render_pass);

        drop(render_pass);

        self.queue.submit(std::iter::once(encoder.finish()));
        self.queue.present(output);

        Ok(())
    }
}

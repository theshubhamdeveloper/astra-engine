use crate::engine::{
    batch::Batch, camera::Camera, draw_command::DrawCommand, render_pipeline::RenderPipeline,
    texture::Texture, vertex::Vertex,
};
use glam::Vec2;
use std::sync::Arc;
use winit::window::Window;

pub struct Renderer {
    surface: wgpu::Surface<'static>,
    device: wgpu::Device,
    queue: wgpu::Queue,

    config: wgpu::SurfaceConfiguration,

    is_surface_configured: bool,

    window: Arc<Window>,

    batch: Batch,

    render_pipeline: RenderPipeline,

    texture_bind_group: wgpu::BindGroup,
    camera_bind_group: wgpu::BindGroup,

    pub draw_commands: Vec<DrawCommand>,

    camera: Camera,
    camera_buffer: wgpu::Buffer,
}

impl Renderer {
    pub async fn new(window: Arc<Window>) -> anyhow::Result<Self> {
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

        println!("Using GPU: {:?}", adapter.get_info().name);

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

        let buffer = Batch::new(&device);

        let texture =
            Texture::from_bytes(&device, &queue, include_bytes!("../img.png"), "img.png")?;

        let render_pipeline =
            RenderPipeline::new(&device, &format, wgpu::include_wgsl!("../shader.wgsl"));

        let texture_bind_group = device.create_bind_group(&wgpu::BindGroupDescriptor {
            layout: &render_pipeline.texture_bind_group_layout,
            entries: &[
                wgpu::BindGroupEntry {
                    binding: 0,
                    resource: wgpu::BindingResource::TextureView(&texture.view),
                },
                wgpu::BindGroupEntry {
                    binding: 1,
                    resource: wgpu::BindingResource::Sampler(&texture.sampler),
                },
            ],
            label: Some("diffuse_bind_group"),
        });

        let camera_buffer = device.create_buffer(&wgpu::BufferDescriptor {
            label: Some("Camera Uniform Buffer"),
            usage: wgpu::BufferUsages::UNIFORM | wgpu::BufferUsages::COPY_DST,
            size: size_of::<glam::Mat4>() as wgpu::BufferAddress,
            mapped_at_creation: false,
        });

        let camera_bind_group = device.create_bind_group(&wgpu::BindGroupDescriptor {
            layout: &render_pipeline.camera_bind_group_layout,
            entries: &[wgpu::BindGroupEntry {
                binding: 0,
                resource: camera_buffer.as_entire_binding(),
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
            batch: buffer,
            render_pipeline,
            texture_bind_group,
            camera_bind_group,
            draw_commands: vec![],
            camera,
            camera_buffer,
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

        self.queue.write_buffer(
            &self.camera_buffer,
            0,
            bytemuck::bytes_of(&self.camera.projection),
        )
    }

    fn serialize_commands(&mut self) {
        self.batch.clear();

        for cmd in &self.draw_commands {
            match cmd {
                DrawCommand::FillRect {
                    rect,
                    transform,
                    paint,
                } => {
                    let base_vertex = self.batch.vertices.len() as u16;

                    // Counterclockwise: bottom-left, bottom-right, top-right, top-left.
                    self.batch.vertices.extend([
                        Vertex {
                            position: transform.transform_point2(Vec2::new(0.0, 0.0)).to_array(),
                            color: paint.color.to_array(),
                            tex_coords: [0.0, 0.0],
                        },
                        Vertex {
                            position: transform
                                .transform_point2(Vec2::new(rect.width, 0.0))
                                .to_array(),
                            color: paint.color.to_array(),
                            tex_coords: [1.0, 0.0],
                        },
                        Vertex {
                            position: transform
                                .transform_point2(Vec2::new(rect.width, rect.height))
                                .to_array(),
                            color: paint.color.to_array(),
                            tex_coords: [1.0, 1.0],
                        },
                        Vertex {
                            position: transform
                                .transform_point2(Vec2::new(0.0, rect.height))
                                .to_array(),
                            color: paint.color.to_array(),
                            tex_coords: [0.0, 1.0],
                        },
                    ]);

                    self.batch.indices.extend([
                        base_vertex,
                        base_vertex + 1,
                        base_vertex + 2,
                        base_vertex,
                        base_vertex + 2,
                        base_vertex + 3,
                    ]);
                }
            }
        }

        self.draw_commands.clear();
    }

    pub fn render(&mut self) -> anyhow::Result<()> {
        self.window.request_redraw();

        if !self.is_surface_configured {
            return Ok(());
        }

        self.serialize_commands();

        self.batch.update(&self.queue);

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

        {
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

            render_pass.set_pipeline(&self.render_pipeline.pipeline);
            render_pass.set_bind_group(0, &self.texture_bind_group, &[]);
            render_pass.set_bind_group(1, &self.camera_bind_group, &[]);
            render_pass.set_vertex_buffer(0, self.batch.vertex_buffer.slice(..));
            render_pass
                .set_index_buffer(self.batch.index_buffer.slice(..), wgpu::IndexFormat::Uint16);
            render_pass.draw_indexed(0..self.batch.num_indices, 0, 0..1);
        }

        self.queue.submit(std::iter::once(encoder.finish()));
        self.queue.present(output);

        Ok(())
    }
}

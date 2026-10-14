use crate::engine::camera::Camera;
use crate::engine::render_pipeline::RenderPipeline;
use crate::engine::texture::Texture;
use crate::engine::vertex::Vertex;
use wgpu::BindingResource;
use wgpu::util::DeviceExt;
use winit::window::Window;

// TEST VERTICES
const VERTICES: &[Vertex] = &[
    Vertex {
        position: [-100.0, 100.0, 0.0],
        tex_coords: [0.0, 0.0],
        color: [1.0, 0.0, 0.0],
    },
    Vertex {
        position: [-100.0, -100.0, 0.0],
        tex_coords: [0.0, 1.0],
        color: [1.0, 0.0, 0.0],
    },
    Vertex {
        position: [100.0, -100.0, 0.0],
        tex_coords: [1.0, 1.0],
        color: [1.0, 0.0, 0.0],
    },
    Vertex {
        position: [100.0, 100.0, 0.0],
        tex_coords: [1.0, 0.0],
        color: [1.0, 0.0, 0.0],
    },
];

const INDICES: &[u16] = &[0, 1, 2, 0, 2, 3];

pub struct Renderer {
    surface: wgpu::Surface<'static>,
    device: wgpu::Device,
    queue: wgpu::Queue,

    config: wgpu::SurfaceConfiguration,

    is_surface_configured: bool,

    window: std::sync::Arc<Window>,

    num_vertices: u32,
    vertex_buffer: wgpu::Buffer,

    num_indices: u32,
    index_buffer: wgpu::Buffer,

    render_pipeline: RenderPipeline,

    diffuse_bind_group: wgpu::BindGroup,
    camera_bind_group: wgpu::BindGroup,

    camera: Camera,
}

impl Renderer {
    pub async fn new(window: std::sync::Arc<Window>) -> anyhow::Result<Self> {
        let num_vertices = VERTICES.len() as u32;
        let num_indices = INDICES.len() as u32;

        let size = window.inner_size();

        let camera = Camera::new(
            glam::Vec2::new(size.width as f32, size.height as f32),
            glam::Vec2::new(0.0, 0.0),
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

        let vertex_buffer = device.create_buffer_init(&wgpu::util::BufferInitDescriptor {
            label: Some("Vertex Buffer"),
            contents: bytemuck::cast_slice(VERTICES),
            usage: wgpu::BufferUsages::VERTEX,
        });

        let index_buffer = device.create_buffer_init(&wgpu::util::BufferInitDescriptor {
            label: Some("Index Buffer"),
            contents: bytemuck::cast_slice(INDICES),
            usage: wgpu::BufferUsages::INDEX,
        });

        let diffuse_texture =
            Texture::from_bytes(&device, &queue, include_bytes!("../img.png"), "img.png")?;

        let render_pipeline =
            RenderPipeline::new(&device, &format, wgpu::include_wgsl!("../shader.wgsl"));

        let diffuse_bind_group = device.create_bind_group(&wgpu::BindGroupDescriptor {
            layout: &render_pipeline.diffuse_bind_group_layout,
            entries: &[
                wgpu::BindGroupEntry {
                    binding: 0,
                    resource: wgpu::BindingResource::TextureView(&diffuse_texture.view),
                },
                wgpu::BindGroupEntry {
                    binding: 1,
                    resource: wgpu::BindingResource::Sampler(&diffuse_texture.sampler),
                },
            ],
            label: Some("diffuse_bind_group"),
        });

        let camera_buffer = device.create_buffer_init(&wgpu::util::BufferInitDescriptor {
            label: Some("Camera Uniform Buffer"),
            contents: bytemuck::bytes_of(&camera.projection),
            usage: wgpu::BufferUsages::UNIFORM | wgpu::BufferUsages::COPY_DST,
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
            num_vertices,
            vertex_buffer,
            num_indices,
            index_buffer,
            render_pipeline,
            diffuse_bind_group,
            camera_bind_group,
            camera,
        })
    }
    pub(crate) fn resize(&mut self, width: u32, height: u32) {
        if width > 0 && height > 0 {
            self.config.width = width;
            self.config.height = height;
            self.surface.configure(&self.device, &self.config);
            self.is_surface_configured = true;
        }
    }

    pub(crate) fn render(&mut self) -> anyhow::Result<()> {
        self.window.request_redraw();

        if !self.is_surface_configured {
            return Ok(());
        }

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
            render_pass.set_bind_group(0, &self.diffuse_bind_group, &[]);
            render_pass.set_bind_group(1, &self.camera_bind_group, &[]);
            render_pass.set_vertex_buffer(0, self.vertex_buffer.slice(..));
            render_pass.set_index_buffer(self.index_buffer.slice(..), wgpu::IndexFormat::Uint16);
            render_pass.draw_indexed(0..self.num_indices, 0, 0..1);
        }

        self.queue.submit(std::iter::once(encoder.finish()));
        self.queue.present(output);

        Ok(())
    }
}

use crate::engine::buffer::GpuBuffer;
use crate::engine::vertex::Vertex;

pub struct Batch {
    pub vertices: Vec<Vertex>,
    pub vertex_buffer: GpuBuffer,

    pub num_indices: u32,
    pub indices: Vec<u32>,
    pub index_buffer: GpuBuffer,
}

impl Batch {
    pub fn new(device: &wgpu::Device, vertex_buffer_size: u64, index_buffer_size: u64) -> Self {
        let vertex_buffer = GpuBuffer::new(
            device,
            vertex_buffer_size,
            wgpu::BufferUsages::VERTEX | wgpu::BufferUsages::COPY_DST,
            "Vertex Buffer",
            false,
        );

        let index_buffer = GpuBuffer::new(
            device,
            index_buffer_size,
            wgpu::BufferUsages::INDEX | wgpu::BufferUsages::COPY_DST,
            "Index Buffer",
            false,
        );

        Self {
            vertices: vec![],
            vertex_buffer,
            num_indices: 0,
            indices: vec![],
            index_buffer,
        }
    }

    pub fn clear(&mut self) {
        self.vertices.clear();
        self.indices.clear();
        self.num_indices = 0;
    }

    pub fn update(&mut self, device: &wgpu::Device, queue: &wgpu::Queue) {
        self.num_indices = self.indices.len() as u32;

        // if self.indices.len() % 2 != 0 {
        //     self.indices.push(*self.indices.last().unwrap_or(&0));
        // }

        self.vertex_buffer
            .write(device, queue, 0, bytemuck::cast_slice(&self.vertices));

        self.index_buffer
            .write(device, queue, 0, bytemuck::cast_slice(&self.indices));
    }
}

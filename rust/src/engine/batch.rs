use crate::engine::vertex::Vertex;

pub struct Batch {
    pub max_vertices: u32,
    pub vertices: Vec<Vertex>,
    pub vertex_buffer_size: wgpu::BufferAddress,
    pub vertex_buffer: wgpu::Buffer,

    pub max_indices: u32,
    pub num_indices: u32,
    pub indices: Vec<u16>,
    pub index_buffer_size: wgpu::BufferAddress,
    pub index_buffer: wgpu::Buffer,
}

impl Batch {
    pub fn new(device: &wgpu::Device) -> Self {
        let max_vertices: u32 = 10000;
        let vertex_buffer_size =
            (max_vertices as usize * size_of::<Vertex>()) as wgpu::BufferAddress;

        let vertex_buffer = device.create_buffer(&wgpu::BufferDescriptor {
            label: Some("Vertex Buffer"),
            size: vertex_buffer_size,
            usage: wgpu::BufferUsages::VERTEX | wgpu::BufferUsages::COPY_DST,
            mapped_at_creation: false,
        });

        let max_indices: u32 = 60000;
        let index_buffer_size = (max_indices as usize * size_of::<u16>()) as wgpu::BufferAddress;
        let index_buffer = device.create_buffer(&wgpu::BufferDescriptor {
            label: Some("Index Buffer"),
            size: index_buffer_size,
            usage: wgpu::BufferUsages::INDEX | wgpu::BufferUsages::COPY_DST,
            mapped_at_creation: false,
        });

        Self {
            max_vertices,
            vertices: vec![],
            vertex_buffer_size,
            vertex_buffer,
            max_indices,
            num_indices: 0,
            indices: vec![],
            index_buffer_size: 0,
            index_buffer,
        }
    }

    pub fn clear(&mut self) {
        self.vertices.clear();
        self.indices.clear();
        self.num_indices = 0;
    }

    pub fn update(&mut self, queue: &wgpu::Queue) {
        self.num_indices = self.indices.len() as u32;

        if self.indices.len() % 2 != 0 {
            self.indices.push(*self.indices.last().unwrap_or(&0));
        }

        queue.write_buffer(&self.vertex_buffer, 0, bytemuck::cast_slice(&self.vertices));

        queue.write_buffer(&self.index_buffer, 0, bytemuck::cast_slice(&self.indices));
    }
}

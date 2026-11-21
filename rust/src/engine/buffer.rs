pub struct GpuBuffer {
    pub buffer: wgpu::Buffer,
    capacity: u64,
    usage: wgpu::BufferUsages,
    label: Option<String>,
    fixed_capacity: bool,
}

impl GpuBuffer {
    pub fn new(
        device: &wgpu::Device,
        capacity: u64,
        usage: wgpu::BufferUsages,
        label: &str,
        fixed_capacity: bool,
    ) -> Self {
        let buffer = device.create_buffer(&wgpu::BufferDescriptor {
            label: Some(label),
            size: capacity,
            usage,
            mapped_at_creation: false,
        });

        buffer.size();

        Self {
            buffer,
            capacity,
            usage,
            label: Some(label.to_string()),
            fixed_capacity,
        }
    }

    fn ensure_capacity(&mut self, device: &wgpu::Device, required: u64) {
        if required <= self.capacity || self.fixed_capacity {
            return;
        }

        let new_capacity = required.max(self.capacity.saturating_mul(2)).max(256);

        self.buffer = device.create_buffer(&wgpu::BufferDescriptor {
            label: self.label.as_deref(),
            size: new_capacity,
            usage: self.usage,
            mapped_at_creation: false,
        });

        self.capacity = new_capacity;
    }

    pub fn write(&mut self, device: &wgpu::Device, queue: &wgpu::Queue, offset: u64, data: &[u8]) {
        self.ensure_capacity(device, data.len() as u64);

        queue.write_buffer(&self.buffer, offset, data);
    }
}

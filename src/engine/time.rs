use std::time::{Duration, Instant};

pub struct Time {
    last: Instant,
    delta: Duration,
}

impl Time {
    pub fn new() -> Self {
        Self {
            last: Instant::now(),
            delta: Default::default(),
        }
    }

    pub fn update(&mut self) {
        let now = Instant::now();
        self.delta = now - self.last;
        self.last = now;
    }

    pub fn delta_time(&self) -> f64 {
        self.delta.as_secs_f64()
    }
}

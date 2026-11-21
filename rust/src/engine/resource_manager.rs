use std::hash::{Hash, Hasher};
use std::marker::PhantomData;

#[derive(Debug)]
pub struct Handle<T> {
    index: u32,
    generation: u32,
    _marker: PhantomData<T>,
}

impl<T> PartialEq for Handle<T> {
    fn eq(&self, other: &Self) -> bool {
        self.index == other.index && self.generation == other.generation
    }
}

impl<T> Eq for Handle<T> {}

impl<T> Hash for Handle<T> {
    fn hash<H: Hasher>(&self, state: &mut H) {
        self.index.hash(state);
        self.generation.hash(state);
    }
}

impl<T> Copy for Handle<T> {}
impl<T> Clone for Handle<T> {
    fn clone(&self) -> Self {
        *self
    }
}

struct Entry<T> {
    generation: u32,
    value: Option<T>,
}

pub struct ResourceManager<T> {
    entries: Vec<Entry<T>>,
    free_indices: Vec<u32>,
}

impl<T> Default for ResourceManager<T> {
    fn default() -> Self {
        Self::new()
    }
}

impl<T> ResourceManager<T> {
    pub fn new() -> Self {
        Self {
            entries: Vec::new(),
            free_indices: Vec::new(),
        }
    }

    pub fn insert(&mut self, value: T) -> Handle<T> {
        if let Some(index) = self.free_indices.pop() {
            let entry = &mut self.entries[index as usize];
            entry.value = Some(value);
            Handle {
                index,
                generation: entry.generation,
                _marker: PhantomData,
            }
        } else {
            let index = self.entries.len() as u32;
            self.entries.push(Entry {
                generation: 0,
                value: Some(value),
            });
            Handle {
                index,
                generation: 0,
                _marker: PhantomData,
            }
        }
    }

    pub fn get(&self, handle: Handle<T>) -> Option<&T> {
        let entry = self.entries.get(handle.index as usize)?;
        if entry.generation == handle.generation {
            entry.value.as_ref()
        } else {
            None
        }
    }

    pub fn get_mut(&mut self, handle: Handle<T>) -> Option<&mut T> {
        let entry = self.entries.get_mut(handle.index as usize)?;
        if entry.generation == handle.generation {
            entry.value.as_mut()
        } else {
            None
        }
    }

    pub fn remove(&mut self, handle: Handle<T>) -> Option<T> {
        let entry = self.entries.get_mut(handle.index as usize)?;
        if entry.generation == handle.generation {
            let value = entry.value.take()?;

            entry.generation = entry.generation.wrapping_add(1);
            self.free_indices.push(handle.index);

            Some(value)
        } else {
            None
        }
    }
}

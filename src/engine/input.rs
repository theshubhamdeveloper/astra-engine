use glam::Vec2;
use std::collections::{BTreeMap, HashSet};
use winit::event::MouseButton;
use winit::keyboard::KeyCode;

pub enum InputEvent {
    KeyPressed { key: KeyCode, repeat: bool },

    KeyReleased { key: KeyCode },

    MouseMoved { position: Vec2, delta: Vec2 },

    MouseButtonPressed { button: MouseButton },

    MouseButtonReleased { button: MouseButton },

    MouseWheel { delta: Vec2 },

    TextInput { character: char },

    TouchStarted { id: u64, position: Vec2 },

    TouchMoved { id: u64, position: Vec2 },

    TouchEnded { id: u64 },

    TouchCancelled { id: u64 },

    Pinch { scale_delta: f32 },

    Pan { delta: Vec2 },

    Rotate { delta_radians: f32 },

    DoubleTap,
}

#[derive(Default, Debug, Clone, PartialEq)]
pub struct InputState {
    pub keys_down: HashSet<KeyCode>,
    pub keys_repeated: HashSet<KeyCode>,
    pub mouse_buttons_down: HashSet<MouseButton>,
    pub touches: BTreeMap<u64, Vec2>,

    pub mouse_position: Vec2,
    pub mouse_delta: Vec2,
    pub scroll_delta: Vec2,
    pub touch_pan_delta: Vec2,
    pub pinch_scale_delta: f32,
    pub rotation_delta: f32,
    pub double_tapped: bool,
    pub text_input: Vec<char>,
}

#[derive(Default)]
pub struct Input {
    current_state: InputState,
    previous_state: InputState,
}

impl Input {
    pub fn new() -> Self {
        Default::default()
    }

    pub fn begin_frame(&mut self) {
        self.previous_state = self.current_state.clone();
        self.current_state.keys_repeated.clear();
        self.current_state.mouse_delta = Vec2::ZERO;
        self.current_state.scroll_delta = Vec2::ZERO;
        self.current_state.touch_pan_delta = Vec2::ZERO;
        self.current_state.pinch_scale_delta = 1.0;
        self.current_state.rotation_delta = 0.0;
        self.current_state.double_tapped = false;
        self.current_state.text_input.clear();
    }

    pub fn update(&mut self, event: &InputEvent) {
        match event {
            InputEvent::KeyPressed { key, repeat } => {
                self.current_state.keys_down.insert(*key);
                if *repeat {
                    self.current_state.keys_repeated.insert(*key);
                }
            }
            InputEvent::KeyReleased { key } => {
                self.current_state.keys_down.remove(key);
            }
            InputEvent::MouseMoved { position, delta } => {
                self.current_state.mouse_position = *position;
                self.current_state.mouse_delta += *delta;
            }
            InputEvent::MouseButtonPressed { button } => {
                self.current_state.mouse_buttons_down.insert(*button);
            }
            InputEvent::MouseButtonReleased { button } => {
                self.current_state.mouse_buttons_down.remove(button);
            }
            InputEvent::MouseWheel { delta } => {
                self.current_state.scroll_delta += *delta;
            }
            InputEvent::TextInput { character } => {
                self.current_state.text_input.push(*character);
            }
            InputEvent::TouchStarted { id, position } | InputEvent::TouchMoved { id, position } => {
                let old_gesture = self.touch_gesture();
                self.current_state.touches.insert(*id, *position);
                self.accumulate_touch_gesture(old_gesture);
            }
            InputEvent::TouchEnded { id } | InputEvent::TouchCancelled { id } => {
                let old_gesture = self.touch_gesture();
                self.current_state.touches.remove(id);
                self.accumulate_touch_gesture(old_gesture);
            }
            InputEvent::Pinch { scale_delta } => {
                if scale_delta.is_finite() && *scale_delta > 0.0 {
                    self.current_state.pinch_scale_delta *= scale_delta;
                }
            }
            InputEvent::Pan { delta } => {
                self.current_state.touch_pan_delta += *delta;
            }
            InputEvent::Rotate { delta_radians } => {
                if delta_radians.is_finite() {
                    self.current_state.rotation_delta += delta_radians;
                }
            }
            InputEvent::DoubleTap => {
                self.current_state.double_tapped = true;
            }
        }
    }

    fn touch_gesture(&self) -> Option<(Vec2, Option<(f32, f32)>)> {
        let touches = &self.current_state.touches;
        if touches.is_empty() {
            return None;
        }

        let center = touches.values().copied().sum::<Vec2>() / touches.len() as f32;
        let pair = if touches.len() >= 2 {
            let mut positions = touches.values().copied();
            let first = positions.next().unwrap();
            let second = positions.next().unwrap();
            let difference = second - first;
            Some((difference.length(), difference.y.atan2(difference.x)))
        } else {
            None
        };

        Some((center, pair))
    }

    fn accumulate_touch_gesture(&mut self, old: Option<(Vec2, Option<(f32, f32)>)>) {
        let Some((old_center, old_pair)) = old else {
            return;
        };
        let Some((new_center, new_pair)) = self.touch_gesture() else {
            return;
        };

        if old_pair.is_some() == new_pair.is_some() {
            self.current_state.touch_pan_delta += new_center - old_center;
        }
        if let (Some((old_distance, old_angle)), Some((new_distance, new_angle))) =
            (old_pair, new_pair)
        {
            if old_distance > f32::EPSILON {
                self.current_state.pinch_scale_delta *= new_distance / old_distance;
            }
            let mut angle_delta = new_angle - old_angle;
            if angle_delta > std::f32::consts::PI {
                angle_delta -= std::f32::consts::TAU;
            } else if angle_delta < -std::f32::consts::PI {
                angle_delta += std::f32::consts::TAU;
            }
            self.current_state.rotation_delta += angle_delta;
        }
    }

    pub fn reset(&mut self) {
        self.current_state.keys_down.clear();
        self.current_state.keys_repeated.clear();
        self.current_state.mouse_buttons_down.clear();
        self.current_state.touches.clear();
    }

    pub fn is_key_down(&self, keycode: KeyCode) -> bool {
        self.current_state.keys_down.contains(&keycode)
    }

    pub fn is_key_pressed(&self, keycode: KeyCode) -> bool {
        self.current_state.keys_down.contains(&keycode)
            && !self.previous_state.keys_down.contains(&keycode)
    }

    pub fn is_key_released(&self, keycode: KeyCode) -> bool {
        !self.current_state.keys_down.contains(&keycode)
            && self.previous_state.keys_down.contains(&keycode)
    }

    pub fn is_key_repeated(&self, keycode: KeyCode) -> bool {
        self.current_state.keys_repeated.contains(&keycode)
    }

    pub fn is_mouse_down(&self, button: MouseButton) -> bool {
        self.current_state.mouse_buttons_down.contains(&button)
    }

    pub fn is_mouse_pressed(&self, button: MouseButton) -> bool {
        self.current_state.mouse_buttons_down.contains(&button)
            && !self.previous_state.mouse_buttons_down.contains(&button)
    }

    pub fn is_mouse_released(&self, button: MouseButton) -> bool {
        !self.current_state.mouse_buttons_down.contains(&button)
            && self.previous_state.mouse_buttons_down.contains(&button)
    }

    pub fn mouse_position(&self) -> &Vec2 {
        &self.current_state.mouse_position
    }

    pub fn mouse_delta(&self) -> &Vec2 {
        &self.current_state.mouse_delta
    }

    pub fn scroll_delta(&self) -> &Vec2 {
        &self.current_state.scroll_delta
    }

    pub fn text_input(&self) -> &[char] {
        &self.current_state.text_input
    }

    pub fn touches(&self) -> &BTreeMap<u64, Vec2> {
        &self.current_state.touches
    }

    pub fn touch_count(&self) -> usize {
        self.current_state.touches.len()
    }

    pub fn touch_pan_delta(&self) -> &Vec2 {
        &self.current_state.touch_pan_delta
    }

    /// Returns the frame's multiplicative pinch scale change (1.0 means unchanged).
    pub fn pinch_scale_delta(&self) -> f32 {
        self.current_state.pinch_scale_delta
    }

    /// Returns the frame's rotation change in radians.
    pub fn rotation_delta(&self) -> f32 {
        self.current_state.rotation_delta
    }

    pub fn was_double_tapped(&self) -> bool {
        self.current_state.double_tapped
    }
}

#[cfg(test)]
mod tests {
    use super::{Input, InputEvent};
    use glam::Vec2;
    use winit::{event::MouseButton, keyboard::KeyCode};

    #[test]
    fn updates_keyboard_state_and_edges() {
        let mut input = Input::default();
        input.begin_frame();
        input.update(&InputEvent::KeyPressed {
            key: KeyCode::KeyA,
            repeat: false,
        });

        assert!(input.is_key_down(KeyCode::KeyA));
        assert!(input.is_key_pressed(KeyCode::KeyA));
        assert!(!input.is_key_repeated(KeyCode::KeyA));

        input.begin_frame();
        assert!(!input.is_key_pressed(KeyCode::KeyA));
        input.update(&InputEvent::KeyPressed {
            key: KeyCode::KeyA,
            repeat: true,
        });
        assert!(input.is_key_repeated(KeyCode::KeyA));

        input.begin_frame();
        input.update(&InputEvent::KeyReleased { key: KeyCode::KeyA });
        assert!(input.is_key_released(KeyCode::KeyA));
    }

    #[test]
    fn updates_mouse_and_text_state() {
        let mut input = Input::default();
        input.begin_frame();
        input.update(&InputEvent::MouseMoved {
            position: Vec2::new(10.0, 20.0),
            delta: Vec2::new(10.0, 20.0),
        });
        input.update(&InputEvent::MouseMoved {
            position: Vec2::new(15.0, 25.0),
            delta: Vec2::new(5.0, 5.0),
        });
        input.update(&InputEvent::MouseWheel {
            delta: Vec2::new(0.0, 1.0),
        });
        input.update(&InputEvent::MouseButtonPressed {
            button: MouseButton::Left,
        });
        input.update(&InputEvent::TextInput { character: 'a' });

        assert_eq!(*input.mouse_position(), Vec2::new(15.0, 25.0));
        assert_eq!(*input.mouse_delta(), Vec2::new(15.0, 25.0));
        assert_eq!(*input.scroll_delta(), Vec2::new(0.0, 1.0));
        assert!(input.is_mouse_down(MouseButton::Left));
        assert!(input.is_mouse_pressed(MouseButton::Left));
        assert_eq!(input.text_input(), &['a']);

        input.begin_frame();
        assert_eq!(*input.mouse_delta(), Vec2::ZERO);
        assert_eq!(*input.scroll_delta(), Vec2::ZERO);
        assert!(input.text_input().is_empty());
        input.update(&InputEvent::MouseButtonReleased {
            button: MouseButton::Left,
        });
        assert!(input.is_mouse_released(MouseButton::Left));
    }

    #[test]
    fn tracks_touch_pan_pinch_and_rotation() {
        let mut input = Input::default();
        input.begin_frame();
        input.update(&InputEvent::TouchStarted {
            id: 1,
            position: Vec2::new(0.0, 0.0),
        });
        input.update(&InputEvent::TouchStarted {
            id: 2,
            position: Vec2::new(10.0, 0.0),
        });
        input.update(&InputEvent::TouchMoved {
            id: 2,
            position: Vec2::new(0.0, 20.0),
        });

        assert_eq!(input.touch_count(), 2);
        assert_eq!(*input.touch_pan_delta(), Vec2::new(-5.0, 10.0));
        assert!((input.pinch_scale_delta() - 2.0).abs() < 0.0001);
        assert!((input.rotation_delta() - std::f32::consts::FRAC_PI_2).abs() < 0.0001);

        input.begin_frame();
        assert_eq!(*input.touch_pan_delta(), Vec2::ZERO);
        assert_eq!(input.pinch_scale_delta(), 1.0);
        assert_eq!(input.rotation_delta(), 0.0);
        input.update(&InputEvent::TouchCancelled { id: 2 });
        assert_eq!(input.touch_count(), 1);
    }

    #[test]
    fn tracks_native_gesture_events() {
        let mut input = Input::default();
        input.begin_frame();
        input.update(&InputEvent::Pinch { scale_delta: 1.25 });
        input.update(&InputEvent::Pan {
            delta: Vec2::new(4.0, -2.0),
        });
        input.update(&InputEvent::Rotate { delta_radians: 0.5 });
        input.update(&InputEvent::DoubleTap);

        assert_eq!(input.pinch_scale_delta(), 1.25);
        assert_eq!(*input.touch_pan_delta(), Vec2::new(4.0, -2.0));
        assert_eq!(input.rotation_delta(), 0.5);
        assert!(input.was_double_tapped());

        input.begin_frame();
        assert!(!input.was_double_tapped());
    }
}

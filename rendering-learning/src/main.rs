extern crate glfw;
use glow::*;
use glfw::{Action, Context, Key, ffi::{glfwInit, glfwWindowHint, glfwTerminate}, Glfw};

fn main() {
    unsafe {
        unsafe_main();
    }
}

unsafe fn unsafe_main() -> Result<(), String>{
    let mut glfw = glfw::init_no_callbacks().unwrap();

    glfw.window_hint(glfw::WindowHint::ContextVersion(3, 3));
    glfw.window_hint(glfw::WindowHint::OpenGlProfile(glfw::OpenGlProfileHint::Core));
    glfw.window_hint(glfw::WindowHint::OpenGlForwardCompat(true));

    let res = glfw.create_window(777, 1025, "Render Learing", glfw::WindowMode::Windowed);
    let mut window = match res {
        Some((window, _)) => window,
        None => {
            glfwTerminate();
            return Err(String::from("Failed to create GLFW window"));
        }
    };

    //glfw.make_context_current(Some(&window));
    window.make_current();

    let gl = glow::Context::from_loader_function(|s| window.get_proc_address(s) as *const _);

    gl.viewport(0, 0, 777, 1025);

    
   
    Ok(())
}


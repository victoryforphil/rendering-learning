#include "glad/glad.h" // must go before glfw3.h
#include "GLFW/glfw3.h"
#include "spdlog/spdlog.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    glViewport(0,0,width, height);
    spdlog::debug("Resized viewport");
}

int main(){
    spdlog::set_pattern("[%^%l%$] %v");
    spdlog::set_level(spdlog::level::debug);
    spdlog::info("Init GLFW...");
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    spdlog::info("GLFW Init success");

    spdlog::info("Creating window");

    GLFWwindow* window = glfwCreateWindow(1280, 720, "Open GL Learning", nullptr, nullptr);

    if (window == nullptr){
        spdlog::error("Failed to create window");
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    spdlog::info("Created window!");

    spdlog::info("Loading GLAD...");

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        spdlog::error("Failed to GLAD Load");
        return -1;
    }

    spdlog::info("Loaded GLAD!");

    glViewport(0,0, 1280, 720);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    while(!glfwWindowShouldClose(window)){
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}
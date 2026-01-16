#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <stdio.h>
#include <cglm/cglm.h>

#define nullptr NULL
int main() {
    if (!glfwInit()) {
        printf("Failed to initialize GLFW\n");
        return -1;
    }
    vec2 vector;
    glm_vec2_zero(vector); 
    // Create a windowed mode window and its OpenGL context
    GLFWwindow* window = glfwCreateWindow(640, 480, "Game of Questioning Life", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    uint32_t vExtensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &vExtensionCount, nullptr);

    printf("Game // Vulkan: Extension Count %d\n", vExtensionCount);

    glfwMakeContextCurrent(window);

    while (!glfwWindowShouldClose(window)) {
        /* Render here */
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

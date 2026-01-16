#include <vfp_error.h>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <cglm/cglm.h>
#include <stdio.h>
#include <stdlib.h>

// Local Deps
#include "vfp_pipeline.h"
#include "vfp_runfiles.h"

#define nullptr NULL
int main(int argc, char **argv) {
    if (!glfwInit()) {
        printf("Failed to initialize GLFW\n");
        return -1;
    }
    vec2 vector;
    glm_vec2_zero(vector);
    // Create a windowed mode window and its OpenGL context
    GLFWwindow *window =
        glfwCreateWindow(640, 480, "Game of Questioning Life", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    uint32_t vExtensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &vExtensionCount, nullptr);

    printf("Game // Vulkan: Extension Count %d\n", vExtensionCount);

    glfwMakeContextCurrent(window);

    /// Random Code BS

    VFP_ShaderPipeline pipeline;
    char *vertex_path = vfp_runfiles_resolve(argv[0],
                                             "_main",
                                             "shaders/simple/glsl_vert.out");
    char *fragment_path = vfp_runfiles_resolve(argv[0],
                                               "_main",
                                               "shaders/simple/glsl_frag.out");

    if (!vertex_path || !fragment_path) {
        printf("Failed to resolve shader paths\n");
        glfwTerminate();
        return -1;
    }

    VFP_Error res = vfp_pipeline_create(&pipeline, vertex_path, fragment_path);

    printf("Game // Pipeline Creation Result: %s\n", vfp_error_string(res));

    free(vertex_path);
    free(fragment_path);

    /// End of Random Code BS

    while (!glfwWindowShouldClose(window)) {
        /* Render here */
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    vfp_pipeline_destroy(&pipeline);

    glfwTerminate();
    return 0;
}

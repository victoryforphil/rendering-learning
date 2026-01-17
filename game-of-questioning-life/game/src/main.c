#include "vfp_vulkan_device.h"
#include <vfp_error.h>
#define GLFW_INCLUDE_VULKAN

#if defined(__APPLE__)
#define VK_USE_PLATFORM_METAL_EXT
#define GLFW_INCLUDE_NONE
#elif defined(__linux__)
#define VK_USE_PLATFORM_XCB_KHR
#endif

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
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    vec2 vector;
    glm_vec2_zero(vector);
    // Create a windowed mode window and its OpenGL context
    GLFWwindow *window =
        glfwCreateWindow(1280, 720, "Game of Questioning Life", NULL, NULL);
    if (!window) {
        printf("Failed to create GLFW window\n");
        goto cleanup;
    }

    uint32_t vExtensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &vExtensionCount, nullptr);

    printf("Game // Vulkan: Extension Count %d\n", vExtensionCount);

    VfpPipeline pipeline;
    char *vertex_path =
        vfp_runfiles_resolve(argv[0], "_main", "shaders/simple/glsl_vert.out");
    char *fragment_path =
        vfp_runfiles_resolve(argv[0], "_main", "shaders/simple/glsl_frag.out");

    if (!vertex_path || !fragment_path) {
        printf("Failed to resolve shader paths\n");
        goto cleanup;
    }

    VfpDeviceVulkan device = {};
    if (vfp_vulkan_device_create(&device, window) != VFP_OK) {
        goto cleanup;
    }
    printf("Game // Created vulkan device.\n");

    if (vfp_pipeline_create(&pipeline, vertex_path, fragment_path, &device) !=
        VFP_OK) {
        goto cleanup;
    }
    printf("Game // Created pipeline.\n");
    while (!glfwWindowShouldClose(window)) {
        /* Render here */

        glfwPollEvents();
    }

cleanup:
    free(vertex_path);
    free(fragment_path);

    vfp_pipeline_destroy(&pipeline);
    vfp_vulkan_device_destroy(&device);

    glfwTerminate();
    return 0;
}

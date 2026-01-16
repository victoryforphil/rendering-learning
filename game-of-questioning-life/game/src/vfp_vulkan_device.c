#include "vfp_vulkan_device.h"
#include "vfp_error.h"
#include <stdio.h>
#include <stdlib.h>

VfpError vfp_vulkan_device_create(VfpDeviceVulkan *out_device) {

    printf("Game // Vulkan // Creating Vulkan Application Info\n");

    VkApplicationInfo appInfo = {};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Game of Questioning Life";
    appInfo.applicationVersion = VK_MAKE_VERSION(0, 0, 1);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(0, 0, 1);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    printf("Game // Vulkan // Creating Vuklan Create Info\n");

    VkInstanceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

    // Get the suppoered glfw Extensions
    printf("Game // Vulkan // Getting GLFW Extensions\n");
    uint32_t glfwExtensionCount = 0;
    const char **glfwExtensions;
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
    createInfo.enabledExtensionCount = glfwExtensionCount;
    createInfo.ppEnabledExtensionNames = glfwExtensions;

    createInfo.enabledLayerCount = 0;

    printf("Game // Vulkan // Creating Vulkan Instance\n");

    VkResult result = vkCreateInstance(&createInfo, NULL, &out_device->pInstance);
    if (result != VK_SUCCESS) {
        fprintf(stderr, "Game // Vulkan // Failed to create Vulkan instance\n");
        return VFP_ERROR_GENERIC;
    }

    return VFP_OK;
}

VfpError vfp_vulkan_device_destroy(VfpDeviceVulkan *device){
    // Null check *device - if null, return w/ warning
    if (device == NULL) {
        fprintf(stderr, "Game // Vulkan // Warning: Attempted to destroy a NULL Vulkan device\n");
        return VFP_ERROR_GENERIC;
    }

    // Null Check *device->instance - if null, return w/ warning
    if (device->pInstance == NULL) {
        fprintf(stderr, "Game // Vulkan // Warning: Attempted to destroy a Vulkan device with NULL instance\n");
        return VFP_ERROR_GENERIC;   
    }

    vkDestroyInstance(device->pInstance, NULL);
    device->pInstance = NULL;
    //free(device); // ERR double free or corruption (out)
    return VFP_OK;
}
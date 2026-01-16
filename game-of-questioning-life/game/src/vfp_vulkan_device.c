#include "vfp_vulkan_device.h"
#include "vfp_error.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

    // If VFP_VK_DEBUG is defined, enable validation layers
    #ifdef VFP_VK_DEBUG
    printf("Game // Vulkan // Enabling Validation Layers\n");
    VfpError res = vfp_vk_validate_check_support();
    if (res != VFP_OK) {
        fprintf(stderr, "Game // Vulkan // Validation layers requested, but not available\n");
        return res;
    }
    #endif


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

/// ---- Validate Support ---- /// 

VfpError vfp_vk_validate_check_support(){
    printf("Game // VulkanDevice // Checking layer support...");
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, NULL);

    VkLayerProperties *availableLayers = malloc(sizeof(VkLayerProperties) * layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers);

    printf("Game // VulkanDevice // Found %d layers\n", layerCount);
    // Print a nice list of available layers
    for (uint32_t i = 0; i < layerCount; i++) {
        printf("    %s\n", availableLayers[i].layerName);
    }

    for (size_t i = 0; i < nVfpVkDebugValidateLayers; i++) {
        bool layerFound = false;
        for (uint32_t j = 0; j < layerCount; j++) {
            if (strcmp(VfpVkDebugValidateLayers[i], availableLayers[j].layerName) == 0) {
                layerFound = true;
                break;
            }
        }
        if (!layerFound) {
            fprintf(stderr, "Game // VulkanDevice // Validation layer not found: %s\n", VfpVkDebugValidateLayers[i]);
            free(availableLayers);
            return VFP_ERROR_GENERIC;
        }
    }

    free(availableLayers);
    printf("Game // VulkanDevice // All validation layers are supported.\n");

    return VFP_OK;
}

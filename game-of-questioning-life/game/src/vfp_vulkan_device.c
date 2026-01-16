#include "vfp_vulkan_device.h"
#include "vfp_error.h"
#include <stdint.h>
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
        fprintf(stderr, "Game // Vulkan // Validation layers requested, but "
                        "not available\n");
        return res;
    }

    createInfo.enabledLayerCount = (uint32_t)N_VFP_VK_VALIDATE_DEBUG_LAYERS;
    createInfo.ppEnabledLayerNames = VFP_VK_VALIDATE_DEBUG_LAYERS;

#endif

    printf("Game // Vulkan // Creating Vulkan Instance\n");

    VkResult result =
        vkCreateInstance(&createInfo, NULL, &out_device->pInstance);
    if (result != VK_SUCCESS) {
        fprintf(stderr, "Game // Vulkan // Failed to create Vulkan instance\n");
        return VFP_ERROR_GENERIC;
    }

    printf("VulkanDevice // Picking physical device...\n");
    vfp_vk_physical_pick(out_device);

    return VFP_OK;
}

VfpError vfp_vulkan_device_destroy(VfpDeviceVulkan *device) {
    // Null check *device - if null, return w/ warning
    if (device == NULL) {
        fprintf(stderr, "Game // Vulkan // Warning: Attempted to destroy a "
                        "NULL Vulkan device\n");
        return VFP_ERROR_GENERIC;
    }

    // Null Check *device->instance - if null, return w/ warning
    if (device->pInstance == NULL) {
        fprintf(stderr, "Game // Vulkan // Warning: Attempted to destroy a "
                        "Vulkan device with NULL instance\n");
        return VFP_ERROR_GENERIC;
    }

    vkDestroyInstance(device->pInstance, NULL);
    device->pInstance = NULL;
    // free(device); // ERR double free or corruption (out)

    return VFP_OK;
}

/// ---- Validate Support ---- ///

VfpError vfp_vk_validate_check_support() {
    printf("Game // VulkanDevice // Checking layer support...");
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, NULL);

    VkLayerProperties *availableLayers =
        malloc(sizeof(VkLayerProperties) * layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers);

    printf("Game // VulkanDevice // Found %d layers\n", layerCount);
    // Print a nice list of available layers
    for (uint32_t i = 0; i < layerCount; i++) {
        printf("    %s\n", availableLayers[i].layerName);
    }

    for (size_t i = 0; i < N_VFP_VK_VALIDATE_DEBUG_LAYERS; i++) {
        bool layerFound = false;
        for (uint32_t j = 0; j < layerCount; j++) {
            if (strcmp(VFP_VK_VALIDATE_DEBUG_LAYERS[i],
                       availableLayers[j].layerName) == 0) {
                layerFound = true;
                break;
            }
        }
        if (!layerFound) {
            fprintf(stderr,
                    "Game // VulkanDevice // Validation layer not found: %s\n",
                    VFP_VK_VALIDATE_DEBUG_LAYERS[i]);
            free(availableLayers);
            return VFP_VK_VALIDATION_LAYERS_NOT_PRESENT;
        }
    }

    free(availableLayers);
    printf("Game // VulkanDevice // All validation layers are supported.\n");

    return VFP_OK;
}

VfpError vfp_vk_physical_pick(VfpDeviceVulkan *device) {
    // Placeholder for future physical device picking logic

    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(device->pInstance, &deviceCount, NULL);

    if (deviceCount == 0) {
        fprintf(stderr, "Game // VulkanDevice // Failed to find GPUs with "
                        "Vulkan support [VFP_VK_NO_PHYSICAL_DEVICES_FOUND].\n");
        return VFP_VK_NO_PHYSICAL_DEVICES_FOUND;
    }

    printf("Vulkan // Found %d physical devices with Vulkan support.\n",
           deviceCount);

    VkPhysicalDevice *physicalDevices =
        malloc(sizeof(VkPhysicalDevice) * deviceCount);
    vkEnumeratePhysicalDevices(device->pInstance, &deviceCount,
                               physicalDevices);

    // Enumuerate through physicalDevices - pick the first one that passes
    // vfp_vk_physical_suitable
    for (uint32_t i = 0; i < deviceCount; i++) {
        bool isSuitable = false;
        VfpError res =
            vfp_vk_physical_suitable(physicalDevices[i], &isSuitable);
        if (res != VFP_OK) {
            free(physicalDevices);
            return res;
        }
        if (isSuitable) {
            // device->physicalDevice = physicalDevices[i];
            memcpy(&device->physicalDevice, &physicalDevices[i],
                   sizeof(VkPhysicalDevice));
            printf("Vulkan // Selected physical device %d as suitable.\n", i);
            break;
        }
    }
    free(physicalDevices);

    return VFP_OK;
}
VfpError vfp_vk_physical_suitable(VkPhysicalDevice physicalDevice,
                                  bool *out_isSuitable) {

    VkPhysicalDeviceProperties deviceProperties;
    VkPhysicalDeviceFeatures deviceFeatures;
    vkGetPhysicalDeviceProperties(physicalDevice, &deviceProperties);
    vkGetPhysicalDeviceFeatures(physicalDevice, &deviceFeatures);

    // Print device name - and other info
    printf("Vulkan // Physical Device: %s\n", deviceProperties.deviceName);
    printf("    Type: %d\n", deviceProperties.deviceType);
    printf("    API Version: %d.%d.%d\n",
           VK_VERSION_MAJOR(deviceProperties.apiVersion),
           VK_VERSION_MINOR(deviceProperties.apiVersion),
           VK_VERSION_PATCH(deviceProperties.apiVersion));
    printf("    Driver Version: %d\n", deviceProperties.driverVersion);
    printf("    Vendor ID: %d\n", deviceProperties.vendorID);
    printf("    Device ID: %d\n", deviceProperties.deviceID);
    
    VfpVkQueueFamilyIndices queueFamilyIndices = {};
    vfp_vk_find_queue_families(&queueFamilyIndices, physicalDevice);

    if (!queueFamilyIndices.bGraphicsFamilySet) {
        printf("Vulkan // Physical Device %s does not have required queue "
               "families.\n",
               deviceProperties.deviceName);
        *out_isSuitable = false;
        return VFP_OK;
    }

    // First check for any dedicated GPUs and use that
    if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU &&
        deviceFeatures.geometryShader) {
        *out_isSuitable = true;
        return VFP_OK;
    }

    // If not just first one to support geometry shaders
    if (deviceFeatures.geometryShader) {
        *out_isSuitable = true;
        return VFP_OK;
    }

    // else warn and return not suitable
    printf("Vulkan // Physical Device %s is not suitable.\n",
           deviceProperties.deviceName);
    *out_isSuitable = false;
    return VFP_OK;
}

void vfp_vk_create_queue_family_index(VfpVkQueueFamilyIndices *out_indices,
                                      int32_t familyIndex) {
    if (!out_indices) {
        fprintf(stderr, "VulkanDevice // Error: NULL out_indices passed to "
                        "vfp_vk_create_queue_family_index\n");
        return;
    }
    out_indices->graphicsFamily = familyIndex;
    out_indices->bGraphicsFamilySet = true;
}

VfpError vfp_vk_find_queue_families(VfpVkQueueFamilyIndices *out_indices,
                                    VkPhysicalDevice physicalDevice) {

    uint32_t nQueueFamilies = 0;
    printf("VulkanDevice // Finding queue families...\n");
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &nQueueFamilies,
                                             NULL);

    VkQueueFamilyProperties *queueFamilies =
        malloc(sizeof(VkQueueFamilyProperties) * nQueueFamilies);
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &nQueueFamilies,
                                             queueFamilies);

    // Iterate until we find a VK_QUEUE_GRAPHICS_BIT supported family
    for (uint32_t i = 0; i < nQueueFamilies; i++) {
        if (queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            vfp_vk_create_queue_family_index(out_indices, i);
            printf("VulkanDevice // Found graphics queue family at index %d\n",
                   i);
            break;
        }
    }

    // Placeholder for future queue family finding logic
    return VFP_OK;
}
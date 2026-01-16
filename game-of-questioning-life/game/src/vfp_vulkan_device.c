#include "vfp_vulkan_device.h"
#include "vfp_error.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char *vfp_vk_result_string(VkResult result) {
    switch (result) {
    case VK_SUCCESS:
        return "VK_SUCCESS";
    case VK_NOT_READY:
        return "VK_NOT_READY";
    case VK_TIMEOUT:
        return "VK_TIMEOUT";
    case VK_EVENT_SET:
        return "VK_EVENT_SET";
    case VK_EVENT_RESET:
        return "VK_EVENT_RESET";
    case VK_INCOMPLETE:
        return "VK_INCOMPLETE";
    case VK_ERROR_OUT_OF_HOST_MEMORY:
        return "VK_ERROR_OUT_OF_HOST_MEMORY";
    case VK_ERROR_OUT_OF_DEVICE_MEMORY:
        return "VK_ERROR_OUT_OF_DEVICE_MEMORY";
    case VK_ERROR_INITIALIZATION_FAILED:
        return "VK_ERROR_INITIALIZATION_FAILED";
    case VK_ERROR_DEVICE_LOST:
        return "VK_ERROR_DEVICE_LOST";
    case VK_ERROR_MEMORY_MAP_FAILED:
        return "VK_ERROR_MEMORY_MAP_FAILED";
    case VK_ERROR_LAYER_NOT_PRESENT:
        return "VK_ERROR_LAYER_NOT_PRESENT";
    case VK_ERROR_EXTENSION_NOT_PRESENT:
        return "VK_ERROR_EXTENSION_NOT_PRESENT";
    case VK_ERROR_FEATURE_NOT_PRESENT:
        return "VK_ERROR_FEATURE_NOT_PRESENT";
    case VK_ERROR_INCOMPATIBLE_DRIVER:
        return "VK_ERROR_INCOMPATIBLE_DRIVER";
    case VK_ERROR_TOO_MANY_OBJECTS:
        return "VK_ERROR_TOO_MANY_OBJECTS";
    case VK_ERROR_FORMAT_NOT_SUPPORTED:
        return "VK_ERROR_FORMAT_NOT_SUPPORTED";
    case VK_ERROR_FRAGMENTED_POOL:
        return "VK_ERROR_FRAGMENTED_POOL";
    case VK_ERROR_OUT_OF_POOL_MEMORY:
        return "VK_ERROR_OUT_OF_POOL_MEMORY";
    case VK_ERROR_INVALID_EXTERNAL_HANDLE:
        return "VK_ERROR_INVALID_EXTERNAL_HANDLE";
    case VK_ERROR_SURFACE_LOST_KHR:
        return "VK_ERROR_SURFACE_LOST_KHR";
    case VK_ERROR_NATIVE_WINDOW_IN_USE_KHR:
        return "VK_ERROR_NATIVE_WINDOW_IN_USE_KHR";
    case VK_SUBOPTIMAL_KHR:
        return "VK_SUBOPTIMAL_KHR";
    case VK_ERROR_OUT_OF_DATE_KHR:
        return "VK_ERROR_OUT_OF_DATE_KHR";
    case VK_ERROR_INCOMPATIBLE_DISPLAY_KHR:
        return "VK_ERROR_INCOMPATIBLE_DISPLAY_KHR";
    case VK_ERROR_VALIDATION_FAILED_EXT:
        return "VK_ERROR_VALIDATION_FAILED_EXT";
    case VK_ERROR_INVALID_SHADER_NV:
        return "VK_ERROR_INVALID_SHADER_NV";
    case VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT:
        return "VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT";
    case VK_ERROR_NOT_PERMITTED_EXT:
        return "VK_ERROR_NOT_PERMITTED_EXT";
    case VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT:
        return "VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT";
    case VK_THREAD_IDLE_KHR:
        return "VK_THREAD_IDLE_KHR";
    case VK_THREAD_DONE_KHR:
        return "VK_THREAD_DONE_KHR";
    case VK_OPERATION_DEFERRED_KHR:
        return "VK_OPERATION_DEFERRED_KHR";
    case VK_OPERATION_NOT_DEFERRED_KHR:
        return "VK_OPERATION_NOT_DEFERRED_KHR";
    case VK_ERROR_COMPRESSION_EXHAUSTED_EXT:
        return "VK_ERROR_COMPRESSION_EXHAUSTED_EXT";
    default:
        return "VK_UNKNOWN_ERROR";
    }
}

void vfp_vk_log_result(const char *label, VkResult result) {
    fprintf(stderr, "%s: %s (%d)\n", label, vfp_vk_result_string(result),
            result);
}

VfpError vfp_vulkan_device_create(VfpDeviceVulkan *out_device,
                                  GLFWwindow *window) {

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
        vfp_vk_log_result("Game // Vulkan // Failed to create Vulkan instance",
                          result);
        return VFP_ERROR_GENERIC;
    }

    // Create Surface
    printf("Game // Vulkan // Creating Window Surface\n");

    result = glfwCreateWindowSurface(out_device->pInstance, window, NULL,
                                     &out_device->surface);
    if (result != VK_SUCCESS) {
        vfp_vk_log_result("Game // Vulkan // Failed to create window surface",
                          result);
        return VFP_ERROR_GENERIC;
    }

    printf("VulkanDevice // Picking physical device...\n");
    if (vfp_vk_physical_pick(out_device) != VFP_OK) {
        fprintf(stderr, "Game // Vulkan // Failed to pick physical device\n");
        return VFP_ERROR_GENERIC;
    }

    // Create Logical Device
    printf("VulkanDevice // Creating logical device...\n");
    if (vfp_vk_create_logical_device(out_device) != VFP_OK) {
        fprintf(stderr, "Game // Vulkan // Failed to create logical device\n");
        return VFP_ERROR_GENERIC;
    }

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

    // Null check and free logical device
    if (device->logicalDevice != VK_NULL_HANDLE) {
        vkDestroyDevice(device->logicalDevice, NULL);
        // device->logicalDevice = VK_NULL_HANDLE;
    }
    // Destroy surface
    if (device->surface != VK_NULL_HANDLE) {
        vkDestroySurfaceKHR(device->pInstance, device->surface, NULL);
        // device->surface = VK_NULL_HANDLE;
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
            vfp_vk_physical_suitable(device, physicalDevices[i], &isSuitable);
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
VfpError vfp_vk_physical_suitable(VfpDeviceVulkan *pDevice,
                                  VkPhysicalDevice physicalDevice,
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
    vfp_vk_find_queue_families(&queueFamilyIndices, pDevice, physicalDevice);

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
void vfp_vk_create_present_queue_family_index(
    VfpVkQueueFamilyIndices *out_indices, int32_t presentFamilyIndex) {
    if (!out_indices) {
        fprintf(stderr, "VulkanDevice // Error: NULL out_indices passed to "
                        "vfp_vk_create_present_queue_family_index\n");
        return;
    }
    out_indices->presentFamily = presentFamilyIndex;
    out_indices->bPresentFamilySet = true;
}

VfpError vfp_vk_find_queue_families(VfpVkQueueFamilyIndices *out_indices,
                                    VfpDeviceVulkan *pDevice,
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

        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, i,
                                             pDevice->surface, &presentSupport);

        if (queueFamilies[i].queueFlags && VK_QUEUE_GRAPHICS_BIT &&
            presentSupport) {
            vfp_vk_create_queue_family_index(out_indices, i);
            printf("VulkanDevice // Found graphics queue family at index %d\n",
                   i);
            break;
        }
    }

    // Placeholder for future queue family finding logic
    return VFP_OK;
}

VfpError vfp_vk_create_logical_device(VfpDeviceVulkan *pDevice) {
    // Placeholder for future logical device creation logic
    printf("VulkanDevice // Creating logical device...\n");
    // 1. Get queue family indices
    VfpVkQueueFamilyIndices queueFamilyIndices = {};
    vfp_vk_find_queue_families(&queueFamilyIndices, pDevice,
                               pDevice->physicalDevice);

    // 2. Crate VkDeviceQueueCreateInfo
    VkDeviceQueueCreateInfo queueCreateInfo = {};
    queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily;
    queueCreateInfo.queueCount = 1;
    float queuePriority = 1.0f;
    queueCreateInfo.pQueuePriorities = &queuePriority;

    VkPhysicalDeviceFeatures deviceFeatures = {};

    VkDeviceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.pQueueCreateInfos = &queueCreateInfo;
    createInfo.queueCreateInfoCount = 1;

    createInfo.pEnabledFeatures = &deviceFeatures;

    VkResult result = vkCreateDevice(pDevice->physicalDevice, &createInfo, NULL,
                                     &pDevice->logicalDevice);
    if (result != VK_SUCCESS) {
        vfp_vk_log_result("VulkanDevice // Failed to create logical device",
                          result);
        return VFP_ERROR_GENERIC;
    }
    // Get and populate graphics queue
    printf("VulkanDevice // Logical // Getting graphics queue...\n");

    vkGetDeviceQueue(pDevice->logicalDevice, queueFamilyIndices.graphicsFamily,
                     0, &pDevice->graphicsQueue);

    return VFP_OK;
}
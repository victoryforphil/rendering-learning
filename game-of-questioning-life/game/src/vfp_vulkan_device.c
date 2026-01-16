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

    printf("VulkanDevice // Creating swapchain...\n");
    if (vfp_vk_create_swapchain(out_device) != VFP_OK) {
        fprintf(stderr, "Game // Vulkan // Failed to create swapchain\n");
        return VFP_ERROR_GENERIC;
    }

    printf("VulkanDevice // Creating image views...\n");
    if (vfp_vfk_create_image_views(out_device) != VFP_OK) {
        fprintf(stderr, "Game // Vulkan // Failed to create image views\n");
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
    vkDestroySwapchainKHR(device->logicalDevice, device->swapchain, NULL);
    // Null Check *device->instance - if null, return w/ warning
    if (device->pInstance == NULL) {
        fprintf(stderr, "Game // Vulkan // Warning: Attempted to destroy a "
                        "Vulkan device with NULL instance\n");
        return VFP_ERROR_GENERIC;
    }
    for (uint32_t i = 0; i < device->swapchainImageViewCount; i++) {
        vkDestroyImageView(device->logicalDevice,
                           device->swapchainImageViews[i], NULL);
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
    for (uint32_t i = 0; i < device->swapchainImageViewCount; i++) {
        vkDestroyImageView(device->logicalDevice,
                           device->swapchainImageViews[i], NULL);
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

    if (!queueFamilyIndices.bPresentFamilySet) {
        printf("Vulkan // Physical Device %s does not have required queue "
               "families.\n",
               deviceProperties.deviceName);
        *out_isSuitable = false;
        return VFP_OK;
    }

    bool extensionsSupported = false;
    vfp_vfk_create_device_ext_support(physicalDevice, &extensionsSupported);
    if (!extensionsSupported) {
        printf("Vulkan // Physical Device %s does not support required device "
               "extensions.\n",
               deviceProperties.deviceName);
        *out_isSuitable = false;
        return VFP_OK;
    }

    bool swapChainAdequate = false;
    VfpVkSwapChainSupportDetails swapChainSupport = {};
    vfp_vk_query_swapchain_support(&swapChainSupport, physicalDevice,
                                   pDevice->surface);
    if (swapChainSupport.formats != NULL &&
        swapChainSupport.presentModes != NULL) {
        swapChainAdequate = true;
    }

    if (!swapChainAdequate) {
        printf("Vulkan // Physical Device %s does not have adequate swapchain "
               "support.\n",
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

        if (queueFamilies[i].queueFlags && VK_QUEUE_GRAPHICS_BIT) {
            vfp_vk_create_queue_family_index(out_indices, i);
            printf("VulkanDevice // Found graphics queue family at index %d\n",
                   i);
        }

        VkBool32 presentSupport = false;

        vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, i,
                                             pDevice->surface, &presentSupport);
        if (presentSupport) {
            vfp_vk_create_present_queue_family_index(out_indices, i);
            printf("VulkanDevice // Found present queue family at index %d\n",
                   i);
        }
        if (out_indices->bGraphicsFamilySet && out_indices->bPresentFamilySet) {
            printf(
                "VulkanDevice // Found all required queue families, stopping "
                "search.\n");
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

    // TODO: Use a Set and for loop to create multiple queue create infos if
    //  needed
    float queuePriority = 1.0f;
    // 2. Crate VkDeviceQueueCreateInfo for graphicsFamily

    int nCreateInfos = 1;

    VkDeviceQueueCreateInfo queueCreateInfo = {};
    queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily;
    queueCreateInfo.queueCount = 1;

    queueCreateInfo.pQueuePriorities = &queuePriority;

    VkDeviceQueueCreateInfo createInfos[2] = {queueCreateInfo};

    // 2.2. Create VkDeviceQueueCreateInfo for presentFamily if different
    if (queueFamilyIndices.graphicsFamily != queueFamilyIndices.presentFamily) {
        VkDeviceQueueCreateInfo presentQueueCreateInfo = {};
        presentQueueCreateInfo.sType =
            VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        presentQueueCreateInfo.queueFamilyIndex =
            queueFamilyIndices.presentFamily;
        presentQueueCreateInfo.queueCount = 1;
        presentQueueCreateInfo.pQueuePriorities = &queuePriority;
        // Note: In a full implementation, you'd need to handle multiple queue
        // create infos
        nCreateInfos = 2;
        createInfos[1] = presentQueueCreateInfo;
    }

    VkPhysicalDeviceFeatures deviceFeatures = {};

    VkDeviceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.pQueueCreateInfos = createInfos;
    createInfo.queueCreateInfoCount = nCreateInfos;
    createInfo.enabledExtensionCount = N_VFP_VK_DEVICE_EXTENSIONS;
    createInfo.ppEnabledExtensionNames = VFP_VK_DEVICE_EXTENSIONS;
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

    // Get and populate present queue
    printf("VulkanDevice // Logical // Getting present queue...\n");
    vkGetDeviceQueue(pDevice->logicalDevice, queueFamilyIndices.presentFamily,
                     0, &pDevice->presentQueue);

    return VFP_OK;
}

VfpError vfp_vfk_create_device_ext_support(VkPhysicalDevice physicalDevice,
                                           bool *out_isSupported) {

    uint32_t extensionCount = 0;
    vkEnumerateDeviceExtensionProperties(physicalDevice, NULL, &extensionCount,
                                         NULL);

    VkExtensionProperties *availableExtensions =
        malloc(sizeof(VkExtensionProperties) * extensionCount);
    vkEnumerateDeviceExtensionProperties(physicalDevice, NULL, &extensionCount,
                                         availableExtensions);
    // Check for required extensions
    for (size_t i = 0; i < N_VFP_VK_DEVICE_EXTENSIONS; i++) {
        bool extensionFound = false;
        for (uint32_t j = 0; j < extensionCount; j++) {
            if (strcmp(VFP_VK_DEVICE_EXTENSIONS[i],
                       availableExtensions[j].extensionName) == 0) {
                extensionFound = true;
                break;
            }
        }
        if (!extensionFound) {
            fprintf(stderr,
                    "VulkanDevice // Required device extension not found: %s\n",
                    VFP_VK_DEVICE_EXTENSIONS[i]);
            free(availableExtensions);
            *out_isSupported = false;
            return VFP_OK;
        }
    }

    *out_isSupported = true;
    return VFP_OK;
}

VfpError
vfp_vk_query_swapchain_support(VfpVkSwapChainSupportDetails *out_details,
                               VkPhysicalDevice physicalDevice,
                               VkSurfaceKHR surface) {
    printf("VulkanDevice // Querying swapchain support...\n");
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface,
                                              &out_details->capabilities);

    uint32_t formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount,
                                         NULL);
    printf("VulkanDevice // Found %d surface formats.\n", formatCount);
    if (formatCount != 0) {
        out_details->formats = malloc(sizeof(VkSurfaceFormatKHR) * formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(
            physicalDevice, surface, &formatCount, out_details->formats);
    }

    uint32_t presentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface,
                                              &presentModeCount, NULL);
    printf("VulkanDevice // Found %d present modes.\n", presentModeCount);

    if (presentModeCount != 0) {
        out_details->presentModes =
            malloc(sizeof(VkPresentModeKHR) * presentModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface,
                                                  &presentModeCount,
                                                  out_details->presentModes);
    }

    return VFP_OK;
}

VfpError
vfp_vk_choose_swap_surface_format(const VkSurfaceFormatKHR *availableFormats,
                                  uint32_t formatCount,
                                  VkSurfaceFormatKHR *out_format) {
    // Perfer SRGB format
    for (uint32_t i = 0; i < formatCount; i++) {
        if (availableFormats[i].format == VK_FORMAT_B8G8R8A8_SRGB &&
            availableFormats[i].colorSpace ==
                VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
            *out_format = availableFormats[i];
            return VFP_OK;
        }
    }
    // else return first format
    *out_format = availableFormats[0];
    ;
    return VFP_OK;
}

VfpError
vfp_vk_choose_swap_present_mode(const VkPresentModeKHR *availablePresentModes,
                                uint32_t presentModeCount,
                                VkPresentModeKHR *out_presentMode) {
    printf("VulkanDevice // Choosing swap present mode...\n");
    // If mailbox present mode is available, use it
    for (uint32_t i = 0; i < presentModeCount; i++) {
        if (availablePresentModes[i] == VK_PRESENT_MODE_MAILBOX_KHR) {
            *out_presentMode = VK_PRESENT_MODE_MAILBOX_KHR;
            printf("VulkanDevice // Chose MAILBOX present mode.\n");
            return VFP_OK;
        }
    }
    // Else use FIFO present mode
    *out_presentMode = VK_PRESENT_MODE_FIFO_KHR;
    printf("VulkanDevice // Chose FIFO present mode.\n");
    return VFP_OK; // V-Sync
}

VfpError vfp_vk_choose_swap_extent(const VkSurfaceCapabilitiesKHR *capabilities,
                                   VkExtent2D *out_extent) {
    printf("VulkanDevice // Choosing swap extent...\n");
    if (capabilities->currentExtent.width != UINT32_MAX) {
        *out_extent = capabilities->currentExtent;
    } else {
        int glfwG, glfwH;
        glfwGetFramebufferSize(glfwGetCurrentContext(), &glfwG, &glfwH);
        VkExtent2D actualExtent = {glfwG, glfwH};

        if (actualExtent.width < capabilities->minImageExtent.width) {
            actualExtent.width = capabilities->minImageExtent.width;
        } else if (actualExtent.width > capabilities->maxImageExtent.width) {
            actualExtent.width = capabilities->maxImageExtent.width;
        }

        if (actualExtent.height < capabilities->minImageExtent.height) {
            actualExtent.height = capabilities->minImageExtent.height;
        } else if (actualExtent.height > capabilities->maxImageExtent.height) {
            actualExtent.height = capabilities->maxImageExtent.height;
        }

        *out_extent = actualExtent;
    }

    printf("VulkanDevice // Chose extent: %d x %d\n", out_extent->width,
           out_extent->height);
    return VFP_OK;
}

VfpError vfp_vk_create_swapchain(VfpDeviceVulkan *pDevice) {
    printf("VulkanDevice // Creating swapchain...\n");
    VfpVkSwapChainSupportDetails swapChainSupport = {};
    vfp_vk_query_swapchain_support(&swapChainSupport, pDevice->physicalDevice,
                                   pDevice->surface);

    VkSurfaceFormatKHR surfaceFormat = {};
    vfp_vk_choose_swap_surface_format(
        swapChainSupport.formats, swapChainSupport.capabilities.minImageCount,
        &surfaceFormat);

    VkPresentModeKHR presentMode = {};
    vfp_vk_choose_swap_present_mode(swapChainSupport.presentModes,
                                    swapChainSupport.capabilities.minImageCount,
                                    &presentMode);
    VkExtent2D extent = {};
    vfp_vk_choose_swap_extent(&swapChainSupport.capabilities, &extent);

    uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
    if (swapChainSupport.capabilities.maxImageCount > 0 &&
        imageCount > swapChainSupport.capabilities.maxImageCount) {
        imageCount = swapChainSupport.capabilities.maxImageCount;
    }

    printf("VulkanDevice // Creating swapchain with %d images...\n",
           imageCount);

    VkSwapchainCreateInfoKHR createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = pDevice->surface;
    createInfo.minImageCount = imageCount;
    createInfo.imageFormat = surfaceFormat.format;
    createInfo.imageColorSpace = surfaceFormat.colorSpace;
    createInfo.imageExtent = extent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    // Handle queue families
    VfpVkQueueFamilyIndices queueFamilyIndices = {};
    vfp_vk_find_queue_families(&queueFamilyIndices, pDevice,
                               pDevice->physicalDevice);
    uint32_t queueFamilyIndicesArray[] = {
        (uint32_t)queueFamilyIndices.graphicsFamily,
        (uint32_t)queueFamilyIndices.presentFamily};

    if (queueFamilyIndices.graphicsFamily != queueFamilyIndices.presentFamily) {
        printf("VulkanDevice // Swapchain using concurrent image sharing "
               "mode...\n");
        createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = queueFamilyIndicesArray;
    } else {
        printf("VulkanDevice // Swapchain using exclusive image sharing "
               "mode...\n");
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        createInfo.queueFamilyIndexCount = 0;  // Optional
        createInfo.pQueueFamilyIndices = NULL; // Optional
    }

    createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    createInfo.presentMode = presentMode;
    createInfo.clipped = VK_TRUE;
    createInfo.oldSwapchain = VK_NULL_HANDLE;

    if (vkCreateSwapchainKHR(pDevice->logicalDevice, &createInfo, NULL,
                             &pDevice->swapchain) != VK_SUCCESS) {
        fprintf(stderr, "VulkanDevice // Failed to create swapchain.\n");
        return VFP_ERROR_GENERIC;
    }

    uint32_t swapchainImageCount = 0;
    vkGetSwapchainImagesKHR(pDevice->logicalDevice, pDevice->swapchain,
                            &swapchainImageCount, NULL);
    pDevice->swapchainImageCount = swapchainImageCount;
    pDevice->swapchainImages = malloc(sizeof(VkImage) * swapchainImageCount);
    vkGetSwapchainImagesKHR(pDevice->logicalDevice, pDevice->swapchain,
                            &swapchainImageCount, pDevice->swapchainImages);

    pDevice->swapchainImageFormat = surfaceFormat.format;
    pDevice->swapchainExtent = extent;
    return VFP_OK;
}

VfpError vfp_vfk_create_image_views(VfpDeviceVulkan *pDevice) {

    pDevice->swapchainImageViewCount = pDevice->swapchainImageCount;
    pDevice->swapchainImageViews =
        malloc(sizeof(VkImageView) * pDevice->swapchainImageViewCount);

    // Loop over swapchain images and create image views
    for (uint32_t i = 0; i < pDevice->swapchainImageCount; i++) {
        VkImageViewCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        createInfo.image = pDevice->swapchainImages[i];
        createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        createInfo.format = pDevice->swapchainImageFormat;
        createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        createInfo.subresourceRange.baseMipLevel = 0;
        createInfo.subresourceRange.levelCount = 1;
        createInfo.subresourceRange.baseArrayLayer = 0;
        createInfo.subresourceRange.layerCount = 1;

        if (vkCreateImageView(pDevice->logicalDevice, &createInfo, NULL,
                              &pDevice->swapchainImageViews[i]) != VK_SUCCESS) {
            fprintf(stderr, "VulkanDevice // Failed to create image views for "
                            "swapchain images.\n");
            return VFP_ERROR_GENERIC;
        }

        return VFP_OK;
    }
    return VFP_OK;
}
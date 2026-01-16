#ifndef _VFP_VULKAN_DEVICE_H_
#define _VFP_VULKAN_DEVICE_H_

#include "vfp_error.h"
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <stdbool.h>

typedef struct VfpVulkanDevice {
    // Placeholder for Vulkan device related data
    VkInstance pInstance;
    bool bValidateDebugEnabled;

} VfpDeviceVulkan;

static const char * const VfpVkDebugValidateLayers[] = {
    "VK_LAYER_KHRONOS_validation",
};

static const size_t nVfpVkDebugValidateLayers =
    sizeof(VfpVkDebugValidateLayers) / sizeof(VfpVkDebugValidateLayers[0]);


VfpError vfp_vulkan_device_create(VfpDeviceVulkan *out_device);
VfpError vfp_vulkan_device_destroy(VfpDeviceVulkan *device);

VfpError vfp_vk_validate_check_support();

#endif // _VFP_VULKAN_DEVICE_H_

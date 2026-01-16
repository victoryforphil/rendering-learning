#ifndef _VFP_VULKAN_DEVICE_H_
#define _VFP_VULKAN_DEVICE_H_

#include "vfp_error.h"
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

typedef struct VfpVulkanDevice {
    // Placeholder for Vulkan device related data
    VkInstance pInstance;
} VfpDeviceVulkan;


VfpError vfp_vulkan_device_create(VfpDeviceVulkan *out_device);
VfpError vfp_vulkan_device_destroy(VfpDeviceVulkan *device);

#endif // _VFP_VULKAN_DEVICE_H_

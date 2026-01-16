#ifndef _VFP_VULKAN_DEVICE_H_
#define _VFP_VULKAN_DEVICE_H_

#include "vfp_error.h"
#include <stdint.h>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <stdbool.h>

typedef struct VfpVulkanDevice {
    // Placeholder for Vulkan device related data
    VkInstance pInstance;
    bool bValidateDebugEnabled;
    VkPhysicalDevice physicalDevice;

} VfpDeviceVulkan;

typedef struct VfpVkQueueFamilyIndices {
    uint32_t graphicsFamily;
    bool bGraphicsFamilySet;
} VfpVkQueueFamilyIndices;


void vfp_vk_create_queue_family_index(VfpVkQueueFamilyIndices* out_indices, int32_t familyIndex);


static const char *const VFP_VK_VALIDATE_DEBUG_LAYERS[] = {
    "VK_LAYER_KHRONOS_validation",
};

static const uint32_t N_VFP_VK_VALIDATE_DEBUG_LAYERS =
    sizeof(VFP_VK_VALIDATE_DEBUG_LAYERS) /
    sizeof(VFP_VK_VALIDATE_DEBUG_LAYERS[0]);

VfpError vfp_vulkan_device_create(VfpDeviceVulkan *out_device);
VfpError vfp_vulkan_device_destroy(VfpDeviceVulkan *device);

VfpError vfp_vk_validate_check_support();

VfpError vfp_vk_physical_pick(VfpDeviceVulkan *device);
VfpError vfp_vk_physical_suitable(VkPhysicalDevice physicalDevice,
                                  bool *out_isSuitable);

VfpError vfp_vk_find_queue_families(VfpVkQueueFamilyIndices* out_indices,
                                   VkPhysicalDevice physicalDevice);

#endif // _VFP_VULKAN_DEVICE_H_

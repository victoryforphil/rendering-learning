#ifndef _VFP_VULKAN_DEVICE_H_
#define _VFP_VULKAN_DEVICE_H_

#include "vfp_error.h"
#include <stdint.h>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <stdbool.h>

typedef struct VfpDeviceVulkan {
    // Placeholder for Vulkan device related data
    VkInstance pInstance;
    bool bValidateDebugEnabled;
    VkPhysicalDevice physicalDevice;
    VkDevice logicalDevice;
    VkQueue graphicsQueue;
    VkQueue presentQueue;
    VkSurfaceKHR surface;
    VkSwapchainKHR swapchain;
    VkImage *swapchainImages;
    uint32_t swapchainImageCount;
    VkFormat swapchainImageFormat;
    VkExtent2D swapchainExtent;

    VkImageView *swapchainImageViews;
    uint32_t swapchainImageViewCount;

} VfpDeviceVulkan;

typedef struct VfpVkQueueFamilyIndices {
    uint32_t graphicsFamily;
    bool bGraphicsFamilySet;

    uint32_t presentFamily;
    bool bPresentFamilySet;
} VfpVkQueueFamilyIndices;

typedef struct VfpVkSwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities;
    VkSurfaceFormatKHR *formats;
    VkPresentModeKHR *presentModes;
} VfpVkSwapChainSupportDetails;

void vfp_vk_create_queue_family_index(VfpVkQueueFamilyIndices *out_indices,
                                      int32_t familyIndex);
void vfp_vk_create_present_queue_family_index(
    VfpVkQueueFamilyIndices *out_indices, int32_t presentFamilyIndex);

static const char *const VFP_VK_VALIDATE_DEBUG_LAYERS[] = {
    "VK_LAYER_KHRONOS_validation",
};

static const uint32_t N_VFP_VK_VALIDATE_DEBUG_LAYERS =
    sizeof(VFP_VK_VALIDATE_DEBUG_LAYERS) /
    sizeof(VFP_VK_VALIDATE_DEBUG_LAYERS[0]);

// TODO: Macro for statisc arrays
static const char *const VFP_VK_DEVICE_EXTENSIONS[] = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME,
};

static const uint32_t N_VFP_VK_DEVICE_EXTENSIONS =
    sizeof(VFP_VK_DEVICE_EXTENSIONS) / sizeof(VFP_VK_DEVICE_EXTENSIONS[0]);

const char *vfp_vk_result_string(VkResult result);
void vfp_vk_log_result(const char *label, VkResult result);

VfpError vfp_vulkan_device_create(VfpDeviceVulkan *out_device,
                                  GLFWwindow *window);
VfpError vfp_vulkan_device_destroy(VfpDeviceVulkan *device);

VfpError vfp_vk_validate_check_support();

VfpError vfp_vk_physical_pick(VfpDeviceVulkan *device);
VfpError vfp_vk_physical_suitable(VfpDeviceVulkan *pDevice,
                                  VkPhysicalDevice physicalDevice,
                                  bool *out_isSuitable);

VfpError vfp_vk_find_queue_families(VfpVkQueueFamilyIndices *out_indices,
                                    VfpDeviceVulkan *pDevice,
                                    VkPhysicalDevice physicalDevice);

VfpError vfp_vk_create_logical_device(VfpDeviceVulkan *pDevice);

VfpError vfp_vfk_create_device_ext_support(VkPhysicalDevice physicalDevice,
                                           bool *out_isSupported);

VfpError
vfp_vk_query_swapchain_support(VfpVkSwapChainSupportDetails *out_details,
                               VkPhysicalDevice physicalDevice,
                               VkSurfaceKHR surface);

VfpError
vfp_vk_choose_swap_surface_format(const VkSurfaceFormatKHR *availableFormats,
                                  uint32_t formatCount,
                                  VkSurfaceFormatKHR *out_format);

VfpError
vfp_vk_choose_swap_present_mode(const VkPresentModeKHR *availablePresentModes,
                                uint32_t presentModeCount,
                                VkPresentModeKHR *out_presentMode);

VfpError vfp_vk_choose_swap_extent(const VkSurfaceCapabilitiesKHR *capabilities,
                                   VkExtent2D *out_extent);

VfpError vfp_vk_create_swapchain(VfpDeviceVulkan *pDevice);

VfpError vfp_vfk_create_image_views(VfpDeviceVulkan *pDevice);

#endif // _VFP_VULKAN_DEVICE_H_

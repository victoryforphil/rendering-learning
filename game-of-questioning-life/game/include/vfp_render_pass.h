#ifndef _VFP_RENDER_PASS_H_
#define _VFP_RENDER_PASS_H_

#include "vfp_error.h"
#include "vfp_vulkan_device.h"
#include <stddef.h>
#include <stdint.h>
#include <vulkan/vulkan_core.h>

typedef struct VfpRenderPass {
    VkRenderPass renderPass;
    VkPipelineLayout pipelineLayout;
} VfpRenderPass;

VfpError vfp_render_pass_create(VfpRenderPass *outRenderPass,
                                VfpDeviceVulkan *pDevice);

void vfp_render_pass_destroy(VfpRenderPass *pRenderPass,
                             VfpDeviceVulkan *pDevice);

#endif

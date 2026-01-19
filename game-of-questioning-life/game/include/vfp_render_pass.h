#ifndef _VFP_RENDER_PASS_H_
#define _VFP_RENDER_PASS_H_

#include "vfp_error.h"
#include "vfp_vulkan_device.h"
#include <stddef.h>
#include <stdint.h>

typedef struct VfpRenderPass {

} VfpRenderPass;

VfpError vfp_render_pass_create(VfpRenderPass *outRenderPass,
                                VfpDeviceVulkan *pDevice);

#endif
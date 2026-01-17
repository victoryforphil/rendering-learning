#ifndef _VFP_PIPELINE_H_
#define _VFP_PIPELINE_H_
#include "vfp_vulkan_device.h"
#define VFP_SHADER_SRC_MAX_LENGTH 4096

#include "vfp_error.h"
#include <stddef.h>
#include <stdint.h>
typedef struct VfpPipeline {

    uint8_t *vertex_shader_data;
    size_t vertex_shader_size;
    uint8_t *fragment_shader_data;
    size_t fragment_shader_size;

} VfpPipeline;

VfpError vfp_pipeline_create(VfpPipeline *out_pipeline,
                             const char *path_vertex_shader,
                             const char *path_fragment_shader,
                             VfpDeviceVulkan *pDevice);

VfpError vfp_pipeline_create_shader_module(VfpPipeline *pPipeline,
                                           VfpDeviceVulkan *pDevice,
                                           const uint8_t *shader_data,
                                           size_t shader_size,
                                           VkShaderModule *out_shader_module);

VfpError vfp_pipeline_destroy(VfpPipeline *pipeline);

#endif

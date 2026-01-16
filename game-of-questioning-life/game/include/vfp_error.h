#ifndef _VFP_ERROR_H_
#define _VFP_ERROR_H_

typedef enum {
    VFP_OK = 0,
    VFP_ERROR_GENERIC = 1,

    // Pipeline Errors
    VFP_PIPELINE_LOAD_VERTEX_FAILED = 100,
    VFP_PIPELINE_LOAD_FRAGMENT_FAILED = 101,

    // Vulkan Errors
    VFP_VK_GENERIC_ERROR = 200,
    VFP_VK_VALIDATION_LAYERS_NOT_PRESENT = 201,
    VFP_VK_NO_PHYSICAL_DEVICES_FOUND = 202,
} VfpError;

const char *vfp_error_string(VfpError err);
#endif // _VFP_ERROR_H_

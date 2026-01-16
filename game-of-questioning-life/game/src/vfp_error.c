#include "vfp_error.h"

static const char *ERROR_STRINGS[] = {
    [VFP_OK] = "OK",
    [VFP_ERROR_GENERIC] = "Generic error",
    [VFP_PIPELINE_LOAD_VERTEX_FAILED] = "Failed to load vertex shader",
    [VFP_PIPELINE_LOAD_FRAGMENT_FAILED] = "Failed to load fragment shader",
    [VFP_VK_NO_QUEUE_FAMILIES_FOUND] = "No Vulkan queue families found",
    [VFP_VK_NO_SUITABLE_PHYSICAL_DEVICE] = "No suitable Vulkan device found",
};

const char *vfp_error_string(VfpError err) { return ERROR_STRINGS[err]; }

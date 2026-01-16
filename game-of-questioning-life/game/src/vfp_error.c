#include "vfp_error.h"

static const char *ERROR_STRINGS[] = {
    [VFP_OK]         = "OK",
    [VFP_ERROR_GENERIC] = "Generic error",
    [VFP_PIPELINE_LOAD_VERTEX_FAILED] = "Failed to load vertex shader",
    [VFP_PIPELINE_LOAD_FRAGMENT_FAILED] = "Failed to load fragment shader",
};

const char *vfp_error_string(VfpError err) {
    
    return ERROR_STRINGS[err];
}

#ifndef _VFP_ERROR_H_
#define _VFP_ERROR_H_

typedef enum {
    VFP_OK = 0,
    VFP_ERROR_GENERIC = 1,

    // Pipeline Errors
    VFP_PIPELINE_LOAD_VERTEX_FAILED = 100,
    VFP_PIPELINE_LOAD_FRAGMENT_FAILED = 101,
} VFP_Error;

const char *vfp_error_string(VFP_Error err);
#endif // _VFP_ERROR_H_
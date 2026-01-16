#ifndef _VFP_PIPELINE_H_
#define _VFP_PIPELINE_H_

#define VFP_SHADER_SRC_MAX_LENGTH 4096

#include <stdint.h>
#include "vfp_error.h"
typedef struct VfpPipeline {

    char *vertex_shader_data;
    char *fragment_shader_data;

} VfpPipeline;

VfpError vfp_pipeline_create(VfpPipeline *out_pipeline,
                             const char *path_vertex_shader,
                             const char *path_fragment_shader);
VfpError vfp_pipeline_destroy(VfpPipeline *pipeline);


#endif

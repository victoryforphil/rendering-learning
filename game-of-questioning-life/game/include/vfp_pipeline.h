#ifndef _VFP_PIPELINE_H_
#define _VFP_PIPELINE_H_

#define VFP_SHADER_SRC_MAX_LENGTH 4096

#include <stdint.h>
#include "vfp_error.h"
typedef struct VFP_ShaderPipeline {

    char*        vertex_shader_data;
    char*        fragment_shader_data;

} VFP_ShaderPipeline;

VFP_Error vfp_pipeline_create( VFP_ShaderPipeline *out_pipeline, 
    const char* path_vertex_shader, const char* path_fragment_shader);
VFP_Error vfp_pipeline_destroy( VFP_ShaderPipeline *pipeline);


#endif

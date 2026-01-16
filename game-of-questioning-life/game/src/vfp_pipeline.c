#include "vfp_pipeline.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *__vfp_read_file_to_string(const char *path) {
    FILE *f = fopen(path, "rb");
    if (!f){
        fprintf(stderr, "Failed to open file: %s\n", path);
        return NULL;
    }

    // get size
    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    rewind(f);

    // allocate buffer (+1 for null terminator)
    char *buffer = malloc(size + 1);
    if (!buffer) {
        fclose(f);
        fprintf(stderr, "Failed to allocate memory for file: %s\n", path);
        return NULL;
    }

    // read the file
    size_t read = fread(buffer, 1, size, f);
    fclose(f);

    // null-terminate
    buffer[read] = '\0';

    return buffer; // caller must free()
}


VFP_Error vfp_pipeline_create(VFP_ShaderPipeline *out_pipeline, const char* path_vertex_shader, const char* path_fragment_shader){

    char* vertex_shader_data = __vfp_read_file_to_string(path_vertex_shader);
    if (!vertex_shader_data) {
        return VFP_PIPELINE_LOAD_VERTEX_FAILED;
    }
    char* fragment_shader_data = __vfp_read_file_to_string(path_fragment_shader);
    if (!fragment_shader_data) {
        free(vertex_shader_data);
        return VFP_PIPELINE_LOAD_FRAGMENT_FAILED;
    }

    out_pipeline->vertex_shader_data = vertex_shader_data;
    out_pipeline->fragment_shader_data = fragment_shader_data;


    return VFP_OK;
  //  vfp_pipeline_load(out_pipeline);
    return VFP_OK;
}
VFP_Error vfp_pipeline_destroy(VFP_ShaderPipeline *pipeline){

    if (pipeline->vertex_shader_data) {
        free(pipeline->vertex_shader_data);
        pipeline->vertex_shader_data = NULL;
    }
    if (pipeline->fragment_shader_data) {
        free(pipeline->fragment_shader_data);
        pipeline->fragment_shader_data = NULL;
    }
    return VFP_OK;
}


#include "vfp_pipeline.h"
#include "vfp_vulkan_device.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static uint8_t *__vfp_read_file_binary(const char *path, size_t *out_size) {
    FILE *f = fopen(path, "rb");
    if (!f) {
        fprintf(stderr, "Failed to open file: %s\n", path);
        return NULL;
    }

    if (fseek(f, 0, SEEK_END) != 0) {
        fclose(f);
        fprintf(stderr, "Failed to seek file: %s\n", path);
        return NULL;
    }
    long size = ftell(f);
    if (size < 0) {
        fclose(f);
        fprintf(stderr, "Failed to get file size: %s\n", path);
        return NULL;
    }
    rewind(f);

    uint8_t *buffer = malloc((size_t)size);
    if (!buffer) {
        fclose(f);
        fprintf(stderr, "Failed to allocate memory for file: %s\n", path);
        return NULL;
    }

    size_t read = fread(buffer, 1, (size_t)size, f);
    fclose(f);
    if (read != (size_t)size) {
        free(buffer);
        fprintf(stderr, "Failed to read file: %s\n", path);
        return NULL;
    }

    *out_size = (size_t)size;
    return buffer; // caller must free()
}

VfpError vfp_pipeline_create(VfpPipeline *out_pipeline,
                             const char *path_vertex_shader,
                             const char *path_fragment_shader,
                             VfpDeviceVulkan *pDevice) {

    printf("Game // Pipeline // Creating pipeline from %s and %s\n",
           path_vertex_shader, path_fragment_shader);
    size_t vertex_shader_size = 0;
    uint8_t *vertex_shader_data =
        __vfp_read_file_binary(path_vertex_shader, &vertex_shader_size);
    if (!vertex_shader_data) {
        return VFP_PIPELINE_LOAD_VERTEX_FAILED;
    }
    printf("Game // Pipeline // Loaded vertex shader data.\n");
    size_t fragment_shader_size = 0;
    uint8_t *fragment_shader_data =
        __vfp_read_file_binary(path_fragment_shader, &fragment_shader_size);
    if (!fragment_shader_data) {
        free(vertex_shader_data);
        return VFP_PIPELINE_LOAD_FRAGMENT_FAILED;
    }
    printf("Game // Pipeline // Loaded fragment shader data.\n");
    out_pipeline->vertex_shader_data = vertex_shader_data;
    out_pipeline->vertex_shader_size = vertex_shader_size;
    out_pipeline->fragment_shader_data = fragment_shader_data;
    out_pipeline->fragment_shader_size = fragment_shader_size;

    // Create Shader Models
    VkShaderModule vertexShaderModule;
    VkShaderModule fragmentShaderModule;
    if (vfp_pipeline_create_shader_module(
            out_pipeline, pDevice, vertex_shader_data, vertex_shader_size,
            &vertexShaderModule) != VFP_OK) {
        return VFP_ERROR_GENERIC;
    }
    if (vfp_pipeline_create_shader_module(
            out_pipeline, pDevice, fragment_shader_data, fragment_shader_size,
            &fragmentShaderModule) != VFP_OK) {
        return VFP_ERROR_GENERIC;
    }

    // Create Shader Stage Infos

    VkPipelineShaderStageCreateInfo vertexShaderStageInfo = {};
    vertexShaderStageInfo.sType =
        VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vertexShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
    vertexShaderStageInfo.module = vertexShaderModule;
    vertexShaderStageInfo.pName = "main";

    VkPipelineShaderStageCreateInfo fragmentShaderStageInfo = {};
    fragmentShaderStageInfo.sType =
        VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    fragmentShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    fragmentShaderStageInfo.module = fragmentShaderModule;
    fragmentShaderStageInfo.pName = "main";

    VkPipelineShaderStageCreateInfo shaderStages[] = {
        vertexShaderStageInfo,
        fragmentShaderStageInfo,
    };

    // Dynamic State
    VkDynamicState dynamicStates[] = {
        VK_DYNAMIC_STATE_VIEWPORT,
        VK_DYNAMIC_STATE_SCISSOR,
    };

    VkPipelineDynamicStateCreateInfo dynamicStateInfo = {};
    dynamicStateInfo.sType =
        VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicStateInfo.dynamicStateCount = 2;
    dynamicStateInfo.pDynamicStates = dynamicStates;

    // Setup input stage
    // For now we just use hardcoded empty values
    VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
    vertexInputInfo.sType =
        VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputInfo.vertexBindingDescriptionCount = 0;
    vertexInputInfo.pVertexBindingDescriptions = NULL;
    vertexInputInfo.vertexAttributeDescriptionCount = 0;
    vertexInputInfo.pVertexAttributeDescriptions = NULL;

    // Input assembly stage
    VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo = {};
    inputAssemblyInfo.sType =
        VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    inputAssemblyInfo.primitiveRestartEnable = VK_FALSE;

    VkViewport viewport = {};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = (float)pDevice->swapchainExtent.width;
    viewport.height = (float)pDevice->swapchainExtent.height;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;

    VkRect2D scissor = {};
    scissor.offset = (VkOffset2D){0, 0};
    scissor.extent = pDevice->swapchainExtent;

    VkPipelineViewportStateCreateInfo viewportStateInfo = {};
    viewportStateInfo.sType =
        VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportStateInfo.viewportCount = 1;
    viewportStateInfo.pViewports = &viewport;
    viewportStateInfo.scissorCount = 1;
    viewportStateInfo.pScissors = &scissor;

    // Rasterizer stage
    VkPipelineRasterizationStateCreateInfo rasterizerInfo = {};
    rasterizerInfo.sType =
        VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizerInfo.depthClampEnable = VK_FALSE;
    rasterizerInfo.rasterizerDiscardEnable = VK_FALSE;
    rasterizerInfo.polygonMode = VK_POLYGON_MODE_FILL;
    rasterizerInfo.lineWidth = 1.0f;
    rasterizerInfo.cullMode = VK_CULL_MODE_BACK_BIT;
    rasterizerInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;
    rasterizerInfo.depthBiasEnable = VK_FALSE;
    rasterizerInfo.depthBiasConstantFactor = 0.0f;
    rasterizerInfo.depthBiasClamp = 0.0f;
    rasterizerInfo.depthBiasSlopeFactor = 0.0f;

    // Multisampling stage
    VkPipelineMultisampleStateCreateInfo multisampleInfo = {};
    multisampleInfo.sType =
        VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampleInfo.sampleShadingEnable = VK_FALSE;
    multisampleInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    multisampleInfo.minSampleShading = 1.0f;
    multisampleInfo.pSampleMask = NULL;
    multisampleInfo.alphaToCoverageEnable = VK_FALSE;
    multisampleInfo.alphaToOneEnable = VK_FALSE;

    // Color blending stage

    VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
    colorBlendAttachment.colorWriteMask =
        VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
        VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    colorBlendAttachment.blendEnable = VK_FALSE;
    colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
    colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
    colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
    colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
    colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
    colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;
    VkPipelineColorBlendStateCreateInfo colorBlendingInfo = {};
    colorBlendingInfo.sType =
        VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlendingInfo.logicOpEnable = VK_FALSE;
    colorBlendingInfo.logicOp = VK_LOGIC_OP_COPY;
    colorBlendingInfo.attachmentCount = 1;
    colorBlendingInfo.pAttachments = &colorBlendAttachment;
    colorBlendingInfo.blendConstants[0] = 0.0f;
    colorBlendingInfo.blendConstants[1] = 0.0f;
    colorBlendingInfo.blendConstants[2] = 0.0f;
    colorBlendingInfo.blendConstants[3] = 0.0f;

    VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 0;
    pipelineLayoutInfo.pSetLayouts = NULL;
    pipelineLayoutInfo.pushConstantRangeCount = 0;
    pipelineLayoutInfo.pPushConstantRanges = NULL;
    if (vkCreatePipelineLayout(pDevice->logicalDevice, &pipelineLayoutInfo,
                               NULL,
                               &out_pipeline->pipelineLayout) != VK_SUCCESS) {
        fprintf(stderr,
                "Game // Pipeline // Failed to create pipeline layout.\n");
        return VFP_ERROR_GENERIC;
    }
    // Clean up

    vkDestroyShaderModule(pDevice->logicalDevice, vertexShaderModule, NULL);
    vkDestroyShaderModule(pDevice->logicalDevice, fragmentShaderModule, NULL);
    return VFP_OK;
}
VfpError vfp_pipeline_destroy(VfpDeviceVulkan *pDevice, VfpPipeline *pipeline) {

    if (pipeline->vertex_shader_data) {
        free(pipeline->vertex_shader_data);
        pipeline->vertex_shader_data = NULL;
        pipeline->vertex_shader_size = 0;
    }
    if (pipeline->fragment_shader_data) {
        free(pipeline->fragment_shader_data);
        pipeline->fragment_shader_data = NULL;
        pipeline->fragment_shader_size = 0;
    }

    if (pipeline->pipelineLayout) {
        // Destroy pipeline layout
        vkDestroyPipelineLayout(pDevice->logicalDevice,
                                pipeline->pipelineLayout, NULL);
        pipeline->pipelineLayout = VK_NULL_HANDLE;
    }
    return VFP_OK;
}

VfpError vfp_pipeline_create_shader_module(VfpPipeline *pPipeline,
                                           VfpDeviceVulkan *pDevice,
                                           const uint8_t *shader_data,
                                           size_t shader_size,
                                           VkShaderModule *out_shaderModule) {

    (void)pPipeline;
    printf("Game // Pipeline // Creating shader module from data.\n");
    if (shader_size % sizeof(uint32_t) != 0) {
        fprintf(
            stderr,
            "Game // Pipeline // Shader size is not a multiple of 4 bytes.\n");
        return VFP_ERROR_GENERIC;
    }
    VkShaderModuleCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = shader_size;
    createInfo.pCode = (const uint32_t *)shader_data;
    if (vkCreateShaderModule(pDevice->logicalDevice, &createInfo, NULL,
                             out_shaderModule) != VK_SUCCESS) {
        fprintf(stderr,
                "Game // Pipeline // Failed to create shader module.\n");
        return VFP_ERROR_GENERIC;
    }
    printf("Game // Pipeline // Created shader module.\n");
    return VFP_OK;
}

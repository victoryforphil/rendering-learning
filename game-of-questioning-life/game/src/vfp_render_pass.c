#include "vfp_render_pass.h"
#include <stdio.h>
#include <vulkan/vulkan_core.h>
VfpError vfp_render_pass_create(VfpRenderPass *outRenderPass,
                                VfpDeviceVulkan *pDevice) {

    VkAttachmentDescription colorAttachment = {};
    colorAttachment.format = pDevice->swapchainImageFormat;
    colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentReference colorAttachmentRef = {};
    colorAttachmentRef.attachment = 0;
    colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpass = {};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &colorAttachmentRef;

    VkRenderPassCreateInfo renderPassCreateInfo;
    renderPassCreateInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassCreateInfo.attachmentCount = 1;
    renderPassCreateInfo.pAttachments = &colorAttachment;
    renderPassCreateInfo.subpassCount = 1;
    renderPassCreateInfo.pSubpasses = &subpass;


    if (vkCreateRenderPass(pDevice->logicalDevice, &renderPassCreateInfo, NULL,
                           &outRenderPass->renderPass) != VK_SUCCESS) {
        fprintf(stderr, "RenderPass // Failed to vkCreateRenderPass\n");
        return VFP_ERROR_GENERIC;
    }



    return VFP_OK;
}

void vfp_render_pass_destroy(VfpRenderPass *pRenderPass,
                             VfpDeviceVulkan *pDevice) {
    if (pRenderPass->pipelineLayout) {
        // Destroy pipeline layout

        vkDestroyPipelineLayout(pDevice->logicalDevice, pRenderPass->pipelineLayout, NULL);
    }

    if (pRenderPass->renderPass) {
        // Destroy pipeline layout

        vkDestroyRenderPass(pDevice->logicalDevice, pRenderPass->renderPass, NULL);
    }


}

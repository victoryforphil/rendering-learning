#include "vfp_render_pass.h"

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

    VkAttachmentDescription colorAttachmentRef = {};
    colorAttachmentRef.format = pDevice->swapchainImageFormat;
    colorAttachmentRef.samples = VK_SAMPLE_COUNT_1_BIT;

    VkSubpassDescription subpass = {};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &colorAttachmentRef;
    return VFP_OK;
}

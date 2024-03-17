#pragma once
#include "vulkanbase/VulkanUtil.h"

class GP2RenderPass
{
public:
	void createRenderPass(VkDevice device, VkFormat swapChainImageFormat);
	void Destroy();

	VkRenderPass& GetRenderPass() { return m_RenderPass; }

private:
	VkRenderPass m_RenderPass;
	VkDevice m_Device;
};
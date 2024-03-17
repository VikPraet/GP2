#pragma once
#include "vulkanbase/VulkanUtil.h"
#include "GP2Shader.h"

class GP2GraphicsPipeline
{
public:
	void createGraphicsPipeline(VkDevice device, GP2Shader& shader, VkRenderPass renderPass);
	void Destroy();

	VkPipeline& GetGraphicsPipeline() { return m_GraphicsPipeline; }

private:
	VkPipeline m_GraphicsPipeline;
	VkPipelineLayout m_PipelineLayout;
	VkDevice m_Device;
};
#pragma once
#include "vulkanbase/VulkanUtil.h"

class CommandBuffer
{
public:
	void SetCommandBuffer(const VkCommandBuffer& cmdBuffer)
	{
		m_CommandBuffer = cmdBuffer;
	}
	const VkCommandBuffer& GetCommandBuffer() const
	{
		return m_CommandBuffer;
	}

	void BeginCommandBuffer();
	void EndCommandBuffer();
private:
	VkCommandBuffer m_CommandBuffer;
};
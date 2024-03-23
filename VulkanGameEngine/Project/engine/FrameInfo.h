#pragma once

#include "Camera.h"

// lib
#include <vulkan/vulkan.h>

namespace vve
{
	struct FrameInfo 
	{
		int frameIndex;
		float frameTime;
		VkCommandBuffer commandBuffer;
		VveCamera& camera;
		VkDescriptorSet globalDescriptorSet;
	};
}
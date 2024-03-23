#pragma once

#include <memory>

#include "Device.h"
#include "Pipeline.h"
#include "GameObject.h"
#include "Camera.h"
#include "engine/FrameInfo.h"

namespace vve
{
	class SimpleRenderSystem
	{
	public:
		SimpleRenderSystem(VveDevice& device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout);
		~SimpleRenderSystem();

		SimpleRenderSystem(const SimpleRenderSystem&) = delete;
		SimpleRenderSystem& operator=(const SimpleRenderSystem&) = delete;

		void renderGameObjects(const FrameInfo& frameInfo , std::vector<VveGameObject>& gameObjects);

	private:
		void createPipelineLayout(VkDescriptorSetLayout globalSetLayout);
		void createPipeline(VkRenderPass renderPass);
		
		VveDevice& m_Device;

		std::unique_ptr<VvePipeline> m_Pipeline;
		VkPipelineLayout m_PipelineLayout;
	};
}

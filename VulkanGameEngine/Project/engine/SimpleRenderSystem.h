#pragma once

#include <memory>

#include "Device.h"
#include "Pipeline.h"
#include "GameObject.h"
#include "Camera.h"

namespace vve
{
	class SimpleRenderSystem
	{
	public:
		SimpleRenderSystem(VveDevice& device, VkRenderPass renderPass);
		~SimpleRenderSystem();

		SimpleRenderSystem(const SimpleRenderSystem&) = delete;
		SimpleRenderSystem& operator=(const SimpleRenderSystem&) = delete;

		void renderGameObjects(VkCommandBuffer commandBuffer, std::vector<VveGameObject>& gameObjects, const VveCamera& camera);

	private:
		void createPipelineLayout();
		void createPipeline(VkRenderPass renderPass);
		
		VveDevice& m_Device;

		std::unique_ptr<VvePipeline> m_Pipeline;
		VkPipelineLayout m_PipelineLayout;
	};
}

#include "SimpleRenderSystem.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <stdexcept>
#include <array>

namespace vve
{
	struct SimplePushConstantData
	{
		glm::mat4 transform{ 1.f };
		glm::mat4 normalMatrix{ 1.f };
	};
}

vve::SimpleRenderSystem::SimpleRenderSystem(VveDevice& device, VkRenderPass renderPass)
	: m_Device{device}
{
	createPipelineLayout();
	createPipeline(renderPass);
}

vve::SimpleRenderSystem::~SimpleRenderSystem()
{
	vkDestroyPipelineLayout(m_Device.device(), m_PipelineLayout, nullptr);
}

void vve::SimpleRenderSystem::createPipelineLayout()
{
	VkPushConstantRange pushConstantRange{};
	pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
	pushConstantRange.offset = 0;
	pushConstantRange.size = sizeof(SimplePushConstantData);

	VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.setLayoutCount = 0;
	pipelineLayoutInfo.pSetLayouts = nullptr;
	pipelineLayoutInfo.pushConstantRangeCount = 1;
	pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;

	if (vkCreatePipelineLayout(m_Device.device(), &pipelineLayoutInfo, nullptr, &m_PipelineLayout) != VK_SUCCESS) {
		throw std::runtime_error("failed to create pipeline layout!");
	}
}

void vve::SimpleRenderSystem::createPipeline(VkRenderPass renderPass)
{
	assert(m_PipelineLayout != nullptr && "Cannot create pipeline before pipeline layout");

	PipelineConfigInfo pipelineConfig{};
	VvePipeline::defaultPipelineConfigInfo(pipelineConfig);
	pipelineConfig.renderPass = renderPass;
	pipelineConfig.pipelineLayout = m_PipelineLayout;
	m_Pipeline = std::make_unique<VvePipeline>(m_Device, "shaders/simple_shader.vert.spv", "shaders/simple_shader.frag.spv", pipelineConfig);
}

void vve::SimpleRenderSystem::renderGameObjects(VkCommandBuffer commandBuffer, std::vector<VveGameObject>& gameObjects, const VveCamera& camera)
{
	m_Pipeline->bind(commandBuffer);

	const auto projectionView = camera.getProjection() * camera.getView();

	for (auto& obj : gameObjects)
	{
		SimplePushConstantData push{};
		auto modelMatrix = obj.transform.mat4();
		push.transform = projectionView * modelMatrix;
		push.normalMatrix = modelMatrix;

		vkCmdPushConstants(commandBuffer, m_PipelineLayout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(SimplePushConstantData), &push);

		obj.model->bind(commandBuffer);
		obj.model->draw(commandBuffer);
	}
}

#include "Renderer.h"

#include <stdexcept>
#include <array>

vve::VveRenderer::VveRenderer(VveWindow& window, VveDevice& device)
	: m_Window{window}, m_Device{device}
{
	recreateSwapChain();
	createCommandBuffers();
}

vve::VveRenderer::~VveRenderer()
{
	freeCommandBuffers();
}

VkCommandBuffer vve::VveRenderer::beginFrame()
{
	assert(!m_IsFrameStarted && "Can't call beginFrame while already in progress");

	auto result = m_SwapChain->acquireNextImage(&m_CurrentImageIndex);

	if (result == VK_ERROR_OUT_OF_DATE_KHR)
	{
		recreateSwapChain();
		return nullptr;
	}

	if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
		throw std::runtime_error("failed to acquire swap chain image!");
	}

	m_IsFrameStarted = true;

	auto commandbuffer = getCurrentCommandBuffer();

	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

	if (vkBeginCommandBuffer(commandbuffer, &beginInfo) != VK_SUCCESS) {
		throw std::runtime_error("failed to begin recording command buffer!");
	}

	return commandbuffer;
}

void vve::VveRenderer::endFrame()
{
	assert(m_IsFrameStarted && "Can't call endFrame while frame is not in progress");
	auto commandbuffer = getCurrentCommandBuffer();

	if (vkEndCommandBuffer(commandbuffer) != VK_SUCCESS) {
		throw std::runtime_error("failed to record command buffer!");
	}

	auto result = m_SwapChain->submitCommandBuffers(&commandbuffer, &m_CurrentImageIndex);

	if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || m_Window.wasWindowResized())
	{
		m_Window.resetWindowResizedFlag();
		recreateSwapChain();
	}
	else if (result != VK_SUCCESS) {
		throw std::runtime_error("failed to present swap chain image!");
	}

	m_IsFrameStarted = false;
	m_CurrentFrameIndex = (m_CurrentFrameIndex + 1) % VveSwapChain::MAX_FRAMES_IN_FLIGHT;
}

void vve::VveRenderer::beginSwapChainRenderPass(VkCommandBuffer commandBuffer)
{
	assert(m_IsFrameStarted && "Can't call beginSwapChainRenderPass if frame is not in progress");
	assert(commandBuffer == getCurrentCommandBuffer() && "Can't beging render pass on command buffer from a different frame");

	VkRenderPassBeginInfo renderPassInfo{};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassInfo.renderPass = m_SwapChain->getRenderPass();
	renderPassInfo.framebuffer = m_SwapChain->getFrameBuffer(m_CurrentImageIndex);

	renderPassInfo.renderArea.offset = { 0, 0 };
	renderPassInfo.renderArea.extent = m_SwapChain->getSwapChainExtent();

	std::array<VkClearValue, 2> clearValues{};
	clearValues[0].color = { 0.01f, 0.01f, 0.01f, 1.0f };
	clearValues[1].depthStencil = { 1.0f, 0 };
	renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
	renderPassInfo.pClearValues = clearValues.data();

	vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

	VkViewport viewport{};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = static_cast<float>(m_SwapChain->getSwapChainExtent().width);
	viewport.height = static_cast<float>(m_SwapChain->getSwapChainExtent().height);
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;
	VkRect2D scissor{ {0, 0}, m_SwapChain->getSwapChainExtent() };
	vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
	vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
}

void vve::VveRenderer::endSwapChainRenderPass(VkCommandBuffer commandBuffer)
{
	assert(m_IsFrameStarted && "Can't call endSwapChainRenderPass if frame is not in progress");
	assert(commandBuffer == getCurrentCommandBuffer() && "Can't end render pass on command buffer from a different frame");

	vkCmdEndRenderPass(commandBuffer);
}

void vve::VveRenderer::createCommandBuffers()
{
	m_CommandBuffers.resize(VveSwapChain::MAX_FRAMES_IN_FLIGHT);

	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandPool = m_Device.getCommandPool();
	allocInfo.commandBufferCount = static_cast<uint32_t>(m_CommandBuffers.size());

	if (vkAllocateCommandBuffers(m_Device.device(), &allocInfo, m_CommandBuffers.data()) !=
		VK_SUCCESS) {
		throw std::runtime_error("failed to allocate command buffers!");
	}
}

void vve::VveRenderer::freeCommandBuffers()
{
	vkFreeCommandBuffers(m_Device.device(), m_Device.getCommandPool(), static_cast<uint32_t>(m_CommandBuffers.size()), m_CommandBuffers.data());
	m_CommandBuffers.clear();
}

void vve::VveRenderer::recreateSwapChain()
{
	auto extent = m_Window.getExtend();
	while (extent.width == 0 || extent.height == 0)
	{
		extent = m_Window.getExtend();
		glfwWaitEvents();
	}

	vkDeviceWaitIdle(m_Device.device());


	m_SwapChain = nullptr;
	if (m_SwapChain == nullptr)
	{
		m_SwapChain = std::make_unique<VveSwapChain>(m_Device, extent);
	}
	else
	{
		std::shared_ptr<VveSwapChain> oldSwapChain = std::move(m_SwapChain);
		m_SwapChain = std::make_unique<VveSwapChain>(m_Device, extent, oldSwapChain);

		if(!oldSwapChain->compareSwapFormats(*m_SwapChain.get()))
		{
			throw std::runtime_error("Swap chain image(or depth) format has changed!");
		}
	}
}

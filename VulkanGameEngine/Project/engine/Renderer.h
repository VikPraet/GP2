#pragma once

#include "Device.h"
#include "SwapChain.h"
#include "window.h"

#include <memory>
#include <cassert>

namespace vve
{
	class VveRenderer
	{
	public:
		VveRenderer(VveWindow& window, VveDevice& device);
		~VveRenderer();

		VveRenderer(const VveRenderer&) = delete;
		VveRenderer& operator=(const VveRenderer&) = delete;

		bool isFrameInProgress() const { return m_IsFrameStarted; }

		VkRenderPass getSwapChainRenderPass() const { return m_SwapChain->getRenderPass(); }
		float getAspectRatio() const { return m_SwapChain->extentAspectRatio(); }
		VkCommandBuffer getCurrentCommandBuffer() const
		{
			assert(m_IsFrameStarted && "Cannot get command buffer when frame is not in progress");
			return m_CommandBuffers[m_CurrentFrameIndex];
		}

		int getFrameIndex() const
		{
			assert(m_IsFrameStarted && "Cannot get frame index when frame is not in progress");
			return m_CurrentFrameIndex;
		}

		VkCommandBuffer beginFrame();
		void endFrame();
		void beginSwapChainRenderPass(VkCommandBuffer commandBuffer);
		void endSwapChainRenderPass(VkCommandBuffer commandBuffer);

	private:
		void createCommandBuffers();
		void freeCommandBuffers();
		void recreateSwapChain();

		VveWindow& m_Window;
		VveDevice& m_Device;
		std::unique_ptr<VveSwapChain> m_SwapChain;
		std::vector<VkCommandBuffer> m_CommandBuffers;

		uint32_t m_CurrentImageIndex{};
		int m_CurrentFrameIndex{};
		bool m_IsFrameStarted = false;
	};
}

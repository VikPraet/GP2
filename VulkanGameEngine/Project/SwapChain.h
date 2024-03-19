#pragma once

#include "Device.h"

// vulkan headers
#include <vulkan/vulkan.h>

// std lib headers
#include <memory>
#include <string>
#include <vector>

namespace vve
{
    class VveSwapChain {
    public:
        static constexpr int MAX_FRAMES_IN_FLIGHT = 2;

        VveSwapChain(VveDevice& deviceRef, VkExtent2D windowExtent);
        VveSwapChain(VveDevice& deviceRef, VkExtent2D windowExtent, std::shared_ptr<VveSwapChain> previous);
        ~VveSwapChain();

        VveSwapChain(const VveSwapChain&) = delete;
        VveSwapChain& operator=(const VveSwapChain&) = delete;

        VkFramebuffer getFrameBuffer(int index) { return m_SwapChainFramebuffers[index]; }
        VkRenderPass getRenderPass() { return m_RenderPass; }
        VkImageView getImageView(int index) { return m_SwapChainImageViews[index]; }
        size_t imageCount() { return m_SwapChainImages.size(); }
        VkFormat getSwapChainImageFormat() { return m_SwapChainImageFormat; }
        VkExtent2D getSwapChainExtent() { return m_SwapChainExtent; }
        uint32_t width() { return m_SwapChainExtent.width; }
        uint32_t height() { return m_SwapChainExtent.height; }

        float extentAspectRatio() {
            return static_cast<float>(m_SwapChainExtent.width) / static_cast<float>(m_SwapChainExtent.height);
        }
        VkFormat findDepthFormat();

        VkResult acquireNextImage(uint32_t* imageIndex);
        VkResult submitCommandBuffers(const VkCommandBuffer* buffers, uint32_t* imageIndex);

        bool compareSwapFormats(const VveSwapChain& swapChain) const {
            return swapChain.m_SwapChainDepthFormat == m_SwapChainDepthFormat && swapChain.m_SwapChainImageFormat == m_SwapChainImageFormat;
        }

    private:
        void init();
        void createSwapChain();
        void createImageViews();
        void createDepthResources();
        void createRenderPass();
        void createFramebuffers();
        void createSyncObjects();

        // Helper functions
        VkSurfaceFormatKHR chooseSwapSurfaceFormat(
            const std::vector<VkSurfaceFormatKHR>& availableFormats);
        VkPresentModeKHR chooseSwapPresentMode(
            const std::vector<VkPresentModeKHR>& availablePresentModes);
        VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

        VkFormat m_SwapChainImageFormat;
        VkFormat m_SwapChainDepthFormat;
        VkExtent2D m_SwapChainExtent;

        std::vector<VkFramebuffer> m_SwapChainFramebuffers;
        VkRenderPass m_RenderPass;

        std::vector<VkImage> m_DepthImages;
        std::vector<VkDeviceMemory> m_DepthImageMemorys;
        std::vector<VkImageView> m_DepthImageViews;
        std::vector<VkImage> m_SwapChainImages;
        std::vector<VkImageView> m_SwapChainImageViews;

        VveDevice& m_Device;
        VkExtent2D m_WindowExtent;

        VkSwapchainKHR m_SwapChain;
        std::shared_ptr<VveSwapChain> m_OldSwapChain;

        std::vector<VkSemaphore> m_ImageAvailableSemaphores;
        std::vector<VkSemaphore> m_RenderFinishedSemaphores;
        std::vector<VkFence> m_InFlightFences;
        std::vector<VkFence> m_ImagesInFlight;
        size_t m_CurrentFrame = 0;
    };
}

#pragma once

#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

#include <vulkan/vulkan_core.h>

#include <string>

namespace vve
{
	class VveWindow
	{
	public:
		VveWindow(int width, int height, const std::string& name);
		~VveWindow();

		VveWindow(const VveWindow&) = delete;
		VveWindow& operator=(const VveWindow&) = delete;

		bool ShouldClose() { return glfwWindowShouldClose(m_Window); }
		VkExtent2D getExtend() { return { static_cast<uint32_t>(m_Width), static_cast<uint32_t>(m_Height) }; }
		bool wasWindowResized() { return m_FrameBufferResized; }
		void resetWindowResizedFlag() { m_FrameBufferResized = false; }
		GLFWwindow* getGLFWwindow() const { return m_Window; }

		void CreateWindowSurface(VkInstance instance, VkSurfaceKHR* surface);

	private:
		static void framebufferResizeCallBack(GLFWwindow* window, int width, int height);
		void initWindow();

		int m_Width{};
		int m_Height{};
		bool m_FrameBufferResized = false;

		std::string m_WindowName;
		GLFWwindow* m_Window;
	};
}

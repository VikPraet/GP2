#include "window.h"

#include <stdexcept>

namespace vve
{
	VveWindow::VveWindow(int width, int height, const std::string& name)
		: m_Width(width), m_Height(height), m_WindowName(name)
	{
		initWindow();
	}

	VveWindow::~VveWindow()
	{
		glfwDestroyWindow(m_Window);
		glfwTerminate();
	}

	void VveWindow::CreateWindowSurface(VkInstance instance, VkSurfaceKHR* surface)
	{
		if (glfwCreateWindowSurface(instance, m_Window, nullptr, surface) != VK_SUCCESS) {
			throw std::runtime_error("failed to create window surface");
		}
	}

	void VveWindow::framebufferResizeCallBack(GLFWwindow* window, int width, int height)
	{
		auto vveWindow = reinterpret_cast<vve::VveWindow*>(glfwGetWindowUserPointer(window));
		vveWindow->m_FrameBufferResized = true;
		vveWindow->m_Width = width;
		vveWindow->m_Height = height;
	}

	void VveWindow::initWindow()
	{
		glfwInit();
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

		m_Window = glfwCreateWindow(m_Width, m_Height, m_WindowName.c_str(), nullptr, nullptr);
		glfwSetWindowUserPointer(m_Window, this);
		glfwSetFramebufferSizeCallback(m_Window, framebufferResizeCallBack);
	}
}

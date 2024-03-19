#pragma once

#include <memory>

#include "engine/Device.h"
#include "engine/window.h"
#include "GameObject.h"
#include "engine/Renderer.h"

namespace vve
{
	class FirstApp
	{
	public:
		static constexpr int WIDTH = 800;
		static constexpr int HEIGHT = 600;

		FirstApp();
		~FirstApp();

		FirstApp(const FirstApp&) = delete;
		FirstApp& operator=(const FirstApp&) = delete;

		void run();

	private:
		void loadGameObjects();

		VveWindow m_Window{WIDTH, HEIGHT, "Vulkan"};
		VveDevice m_Device{ m_Window };
		VveRenderer m_Renderer{m_Window, m_Device};

		std::vector<VveGameObject> m_GameObjects;
	};
}

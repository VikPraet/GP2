#include "App.h"

#include "engine/SimpleRenderSystem.h" 
#include "Camera.h"
#include "engine/GameTime.h"
#include "KeyboardMovementController.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>


vve::FirstApp::FirstApp()
{
	loadGameObjects();
}

vve::FirstApp::~FirstApp() {}

void vve::FirstApp::run()
{
	SimpleRenderSystem simpleRenderSystem{m_Device, m_Renderer.getSwapChainRenderPass()};
	VveCamera camera{};
    //camera.setViewDirection(glm::vec3{ 0.f }, glm::vec3{ 0.5f, 0.f, 1.f });
    camera.setViewTarget(glm::vec3(-1.f, -2.f, 2.f), glm::vec3(0.f, 0.f, 2.5f));

    auto viewerObject = VveGameObject::createGameObject();
    KeyboardMovementController cameraController{};

    GameTime::Start();

    float lag = 0.0f;
    while (!m_Window.ShouldClose())
    {
        const float fixedTimeStep = GameTime::GetFixedDeltaTime();

        GameTime::Update();

        const float deltaTime = GameTime::GetDeltaTime();

        lag += deltaTime;

        // input
        glfwPollEvents();
        cameraController.moveInPlaneXZ(m_Window.getGLFWwindow(), viewerObject);
        while (lag >= fixedTimeStep)
        {
            //FixedUpdate();
            lag -= fixedTimeStep;
        }

        //Update();
        float aspect = m_Renderer.getAspectRatio();
        //camera.setOrthographicProjection(-aspect, aspect, -1, 1, -1, 1);
        camera.setPerspectiveProjection(glm::radians(50.f), aspect, 0.1f, 100.f);
        camera.setViewYXZ(viewerObject.transform.translation, viewerObject.transform.rotation);

        //Render();
        if (auto commandBuffer = m_Renderer.beginFrame())
        {
            m_Renderer.beginSwapChainRenderPass(commandBuffer);
            simpleRenderSystem.renderGameObjects(commandBuffer, m_GameObjects, camera);
            m_Renderer.endSwapChainRenderPass(commandBuffer);
            m_Renderer.endFrame();
        }
    }

	vkDeviceWaitIdle(m_Device.device());
}

std::unique_ptr<vve::VveMesh> createCubeModel(vve::VveDevice& device, glm::vec3 offset) {
    std::vector<vve::VveMesh::Vertex> vertices{

        // left face (white)
        {{-.5f, -.5f, -.5f}, {.9f, .9f, .9f}},
        {{-.5f, .5f, .5f}, {.9f, .9f, .9f}},
        {{-.5f, -.5f, .5f}, {.9f, .9f, .9f}},
        {{-.5f, -.5f, -.5f}, {.9f, .9f, .9f}},
        {{-.5f, .5f, -.5f}, {.9f, .9f, .9f}},
        {{-.5f, .5f, .5f}, {.9f, .9f, .9f}},

        // right face (yellow)
        {{.5f, -.5f, -.5f}, {.8f, .8f, .1f}},
        {{.5f, .5f, .5f}, {.8f, .8f, .1f}},
        {{.5f, -.5f, .5f}, {.8f, .8f, .1f}},
        {{.5f, -.5f, -.5f}, {.8f, .8f, .1f}},
        {{.5f, .5f, -.5f}, {.8f, .8f, .1f}},
        {{.5f, .5f, .5f}, {.8f, .8f, .1f}},

        // top face (orange, remember y axis points down)
        {{-.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
        {{.5f, -.5f, .5f}, {.9f, .6f, .1f}},
        {{-.5f, -.5f, .5f}, {.9f, .6f, .1f}},
        {{-.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
        {{.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
        {{.5f, -.5f, .5f}, {.9f, .6f, .1f}},

        // bottom face (red)
        {{-.5f, .5f, -.5f}, {.8f, .1f, .1f}},
        {{.5f, .5f, .5f}, {.8f, .1f, .1f}},
        {{-.5f, .5f, .5f}, {.8f, .1f, .1f}},
        {{-.5f, .5f, -.5f}, {.8f, .1f, .1f}},
        {{.5f, .5f, -.5f}, {.8f, .1f, .1f}},
        {{.5f, .5f, .5f}, {.8f, .1f, .1f}},

        // nose face (blue)
        {{-.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
        {{.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
        {{-.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
        {{-.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
        {{.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
        {{.5f, .5f, 0.5f}, {.1f, .1f, .8f}},

        // tail face (green)
        {{-.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
        {{.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
        {{-.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
        {{-.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
        {{.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
        {{.5f, .5f, -0.5f}, {.1f, .8f, .1f}},

    };
    for (auto& v : vertices) {
        v.position += offset;
    }
    return std::make_unique<vve::VveMesh>(device, vertices);
}

void vve::FirstApp::loadGameObjects()
{
    std::shared_ptr<VveMesh> model = createCubeModel(m_Device, { .0f, .0f, .0f });

    auto cube = VveGameObject::createGameObject();
    cube.model = model;
    cube.transform.translation = { .0f, .0f, 2.5f };
    cube.transform.scale = { .5f, .5f, .5f };

    m_GameObjects.push_back(std::move(cube));
}

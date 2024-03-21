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

void vve::FirstApp::loadGameObjects()
{
    std::shared_ptr<VveMesh> model = VveMesh::createModelFromFile(m_Device, "sm_cowboy_test.obj");

    auto gameObject = VveGameObject::createGameObject();
    gameObject.model = model;
    gameObject.transform.translation = { .0f, .5f, 2.5f };
    gameObject.transform.scale = glm::vec3(1.f, -1.f, -1.f);

    m_GameObjects.push_back(std::move(gameObject));
}

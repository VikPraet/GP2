#include "App.h"

#include "engine/SimpleRenderSystem.h" 
#include "Camera.h"
#include "engine/GameTime.h"
#include "engine/Buffer.h"
#include "KeyboardMovementController.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

namespace vve
{
	struct GlobalUbo
	{
        glm::mat4 projectionView{1.f};
        glm::vec3 lightDirection = glm::normalize(glm::vec3{1.f, -3.f, -1.f});
	};
}

vve::FirstApp::FirstApp()
{
    m_GlobalPool = VveDescriptorPool::Builder(m_Device)
		.setMaxSets(VveSwapChain::MAX_FRAMES_IN_FLIGHT)
		.addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VveSwapChain::MAX_FRAMES_IN_FLIGHT)
		.build();

	loadGameObjects();
}

vve::FirstApp::~FirstApp() {}

void vve::FirstApp::run()
{
    std::vector<std::unique_ptr<VveBuffer>> uboBuffers(VveSwapChain::MAX_FRAMES_IN_FLIGHT);
    for (int i = 0; i < uboBuffers.size(); i++)
    {
        uboBuffers[i] = std::make_unique<VveBuffer>(
            m_Device,
            sizeof(GlobalUbo),
            1,
            VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);

        uboBuffers[i]->map();
    }

    auto globalSetLayout = VveDescriptorSetLayout::Builder(m_Device)
        .addBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT)
        .build();

    std::vector<VkDescriptorSet> globalDescriptorSets(VveSwapChain::MAX_FRAMES_IN_FLIGHT);
    for (int i{}; i < globalDescriptorSets.size(); i++)
    {
        auto bufferInfo = uboBuffers[i]->descriptorInfo();
        VveDescriptorWriter(*globalSetLayout, *m_GlobalPool)
            .writeBuffer(0, &bufferInfo)
            .build(globalDescriptorSets[i]);
    }

	SimpleRenderSystem simpleRenderSystem{m_Device, m_Renderer.getSwapChainRenderPass(), globalSetLayout->getDescriptorSetLayout()};
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

        if (auto commandBuffer = m_Renderer.beginFrame())
        {
            //Update();
            int frameIndex = m_Renderer.getFrameIndex();
            FrameInfo frameInfo{
                frameIndex,
                deltaTime,
                commandBuffer,
                camera,
                globalDescriptorSets[frameIndex]
            };

            float aspect = m_Renderer.getAspectRatio();
            camera.setPerspectiveProjection(glm::radians(50.f), aspect, 0.1f, 100.f);
        	camera.setViewYXZ(viewerObject.transform.translation, viewerObject.transform.rotation);

            GlobalUbo ubo{};
            ubo.projectionView = camera.getProjection() * camera.getView();
            uboBuffers[frameIndex]->writeToBuffer(&ubo);
            uboBuffers[frameIndex]->flush();

			//Render();
            m_Renderer.beginSwapChainRenderPass(commandBuffer);
            simpleRenderSystem.renderGameObjects(frameInfo, m_GameObjects);
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

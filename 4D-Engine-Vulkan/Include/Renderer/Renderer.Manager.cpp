module;

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

#include <array>
#include <cassert>
#include <stdexcept>
#include <memory>
#include <string>

#include <iostream>

module Engine4D.Renderer.Manager;

namespace Engine4D {

    /*struct UniformBufferObject {
        alignas(16) glm::mat4 model;
        alignas(16) glm::mat4 view;
        alignas(16) glm::mat4 proj;
    };*/

    struct UniformBufferObject {
		std::vector<Instruction> instructions;
    };    

	struct Scene {
		std::vector<Instruction> instructions;
	};

	struct PushConstantObject {
		alignas(8) glm::vec2 resolution;
		alignas(4) float time;
        alignas(4) float rot;
		alignas(16) glm::vec4 cameraPosition;
	};

    rManager::rManager() {
        loadModels();
        createPipelineLayout();
        createPipeline();
    }

    rManager::rManager(void (*_main_Update)(), void (*_main_Late_Update)(), TimeClass* time) {

		main_Update = _main_Update;
		main_Late_Update = _main_Late_Update;
		Time = time;

        loadModels();
        createPipelineLayout();
        createPipeline();
    }

    rManager::~rManager() { vkDestroyPipelineLayout(device.device(), pipelineLayout, nullptr); }

	std::string vec4ToString(glm::vec4 vec) {
		return std::to_string(vec.x) + " " + std::to_string(vec.y) + " " + std::to_string(vec.z) + " " + std::to_string(vec.w);
	}

	constexpr float RotateMultiplier = -2.0f;

    constexpr float speed = 4.0f;

    constexpr float MoveMultiplierX = 2.0f;
    constexpr float MoveMultiplierY = 2.0f;
    constexpr float MoveMultiplierZ = 2.0f;
    constexpr float MoveMultiplierW = 0.6f;

    void rManager::run() {

		rBuffer globalBuffer(
            device,
            sizeof(UniformBufferObject),
            rSwapChain::MAX_FRAMES_IN_FLIGHT,
            VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT,
            device.properties.limits.minUniformBufferOffsetAlignment
        );
		globalBuffer.map();


        float timePassed = 0.0f;
        while (!window.shouldClose()) {

            double currentTime = glfwGetTime();
            double deltaTime = currentTime - lastUpdatedTime;

            glfwPollEvents();

			float shiftMult = 1;

            int stateShift = glfwGetKey(window.getWindow(), GLFW_KEY_LEFT_SHIFT);
            if (stateShift == GLFW_PRESS)
                shiftMult += 1.0f;
            int stateControl = glfwGetKey(window.getWindow(), GLFW_KEY_LEFT_CONTROL);
            if (stateControl == GLFW_PRESS)
                shiftMult -= 0.75f;

            int stateE = glfwGetKey(window.getWindow(), GLFW_KEY_E);
            if (stateE == GLFW_PRESS)
                rotation += RotateMultiplier * shiftMult * deltaTime;
            int stateQ = glfwGetKey(window.getWindow(), GLFW_KEY_Q);
            if (stateQ == GLFW_PRESS)
                rotation -= RotateMultiplier * shiftMult * deltaTime;
            rotation = fmod(rotation, 6.28318530718f);

            glm::vec4 nextCamPos = glm::vec4(0, 0, 0, 0);

            int stateW = glfwGetKey(window.getWindow(), GLFW_KEY_W);
            if (stateW == GLFW_PRESS)
                nextCamPos.z += MoveMultiplierZ * shiftMult * speed * deltaTime;
            int stateS = glfwGetKey(window.getWindow(), GLFW_KEY_S);
            if (stateS == GLFW_PRESS)
                nextCamPos.z -= MoveMultiplierZ * shiftMult * speed * deltaTime;

            int stateA = glfwGetKey(window.getWindow(), GLFW_KEY_A);
            if (stateA == GLFW_PRESS)
                nextCamPos.x -= MoveMultiplierX * shiftMult * speed * deltaTime;
            int stateD = glfwGetKey(window.getWindow(), GLFW_KEY_D);
            if (stateD == GLFW_PRESS)
                nextCamPos.x += MoveMultiplierX * shiftMult * speed * deltaTime;

            int stateX = glfwGetKey(window.getWindow(), GLFW_KEY_X);
            if (stateX == GLFW_PRESS)
                nextCamPos.y += MoveMultiplierY * shiftMult * speed * deltaTime;
            int stateZ = glfwGetKey(window.getWindow(), GLFW_KEY_Z);
            if (stateZ == GLFW_PRESS)
                nextCamPos.y -= MoveMultiplierY * shiftMult * speed * deltaTime;

            glm::mat4 camRot = glm::rotate(glm::mat4(1.0f), rotation, glm::vec3(0.0f, -1.0f, 0.0f));

            //std::cout << glm::to_string(camRot) << std::endl;

            nextCamPos = camRot * nextCamPos;

            int stateT = glfwGetKey(window.getWindow(), GLFW_KEY_T);
            if (stateT == GLFW_PRESS)
                nextCamPos.w += MoveMultiplierW * shiftMult * speed * deltaTime;
            int stateG = glfwGetKey(window.getWindow(), GLFW_KEY_G);
            if (stateG == GLFW_PRESS)
                nextCamPos.w -= MoveMultiplierW * shiftMult * speed * deltaTime;

            cameraPosition += nextCamPos;

            //std::cout << vec4ToString(cameraPosition) << " ; " << vec4ToString(nextCamPos) << " ; " << rotation << std::endl;            

            if (currentTime - lastFrameTime >= 1.0 / MAX_FPS) {

                if (auto commandbuffer = renderer.beginFrame()) {
                    Time->deltaTime = deltaTime;
                    main_Update();
                    deltaTime = currentTime - lastUpdatedTime;
                    Time->deltaTime = deltaTime;
                    main_Late_Update();

                    renderer.beginSwapChainRenderPass(commandbuffer);

                    doModelStuff(commandbuffer);

                    renderer.endSwapChainRenderPass(commandbuffer);
                    renderer.endFrame();

                    lastFrameTime = currentTime;
                }
            }

            timePassed += deltaTime;

            lastUpdatedTime = currentTime;

        }

        vkDeviceWaitIdle(device.device());
    }

    void rManager::loadModels() {
        rModel::Builder modelBuilder{};
        modelBuilder.vertices = {
            {{-1.0f, -1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}},
            {{1.0f, -1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},
            {{1.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}},
            {{1.0f, 1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}},
            {{-1.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},
            {{-1.0f, -1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}} };		
        model = std::make_unique<rModel>(device, modelBuilder);
    }

    void rManager::createPipelineLayout() {

		VkPushConstantRange pushConstantRange{};
		pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
		pushConstantRange.offset = 0;
		pushConstantRange.size = sizeof(PushConstantObject);

        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = 0;
        pipelineLayoutInfo.pSetLayouts = nullptr;
        pipelineLayoutInfo.pushConstantRangeCount = 1;
        pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;
        if (vkCreatePipelineLayout(device.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) !=
            VK_SUCCESS) {
            throw std::runtime_error("Failed to Create Pipeline Layout!");
        }
    }

    void rManager::createPipeline() {
        assert(pipelineLayout != nullptr && "Cannot Create Pipeline before Pipeline Layout");

        PipelineConfigInfo pipelineConfig{};
        rPipeline::defaultPipelineConfigInfo(pipelineConfig);
        pipelineConfig.renderPass = renderer.getSwapChainRenderPass();
        pipelineConfig.pipelineLayout = pipelineLayout;
        pipeline = std::make_unique<rPipeline>(
            device,
            "shaders/simple_shader.vert.spv",
            "shaders/simple_shader.frag.spv",
            pipelineConfig);
    }

    void rManager::doModelStuff(VkCommandBuffer commandBuffer) {

        pipeline->bind(commandBuffer);
        model->bind(commandBuffer);

		PushConstantObject push{};
		push.resolution = glm::vec2(renderer.getSwapChainExtent().width, renderer.getSwapChainExtent().height);
		push.time = static_cast<float>(glfwGetTime());
		push.rot = rotation;
		push.cameraPosition = cameraPosition;

		vkCmdPushConstants(commandBuffer, pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(PushConstantObject), &push);

        model->draw(commandBuffer);
    }

}

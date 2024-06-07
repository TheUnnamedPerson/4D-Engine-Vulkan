module;

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

#include <chrono>

#include <array>
#include <cassert>
#include <stdexcept>
#include <memory>
#include <string>

#include <iostream>

module Engine4D.Renderer.Manager;

import Engine4D.Primitives;

namespace Engine4D {

    constexpr float RotateMultiplier = -2.0f;

    constexpr float speed = 4.0f;

    constexpr float MoveMultiplierX = 2.0f;
    constexpr float MoveMultiplierY = 2.0f;
    constexpr float MoveMultiplierZ = 2.0f;
    constexpr float MoveMultiplierW = 0.6f;

    /*struct UniformBufferObject {
        alignas(16) glm::mat4 model;
        alignas(16) glm::mat4 view;
        alignas(16) glm::mat4 proj;
    };*/  

	struct Scene {
		Instruction instructions[];
	};

    rManager::rManager() {
		globalDescriptorPool = rDescriptorPool::Builder(device)
			.setMaxSets(rSwapChain::MAX_FRAMES_IN_FLIGHT)
			.addPoolSize(VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, rSwapChain::MAX_FRAMES_IN_FLIGHT)
			.build();
        loadModels();
    }

    rManager::rManager(void (*_main_Update)(), void (*_main_Late_Update)(), TimeClass* time) {
        globalDescriptorPool = rDescriptorPool::Builder(device)
            .setMaxSets(rSwapChain::MAX_FRAMES_IN_FLIGHT)
            .addPoolSize(VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, rSwapChain::MAX_FRAMES_IN_FLIGHT)
            .build();
		main_Update = _main_Update;
		main_Late_Update = _main_Late_Update;
		Time = time;

        loadModels();
    }

    rManager::~rManager() {
		std::cout << "Destroying Manager" << std::endl;
    }

	std::string vec4ToString(glm::vec4 vec) {
		return std::to_string(vec.x) + " " + std::to_string(vec.y) + " " + std::to_string(vec.z) + " " + std::to_string(vec.w);
	}

    void rManager::cameraControls()
    {
        float shiftMult = 1;

        int stateShift = glfwGetKey(window.getWindow(), GLFW_KEY_LEFT_SHIFT);
        if (stateShift == GLFW_PRESS)
            shiftMult += 1.0f;
        int stateControl = glfwGetKey(window.getWindow(), GLFW_KEY_LEFT_CONTROL);
        if (stateControl == GLFW_PRESS)
            shiftMult -= 0.75f;

        int stateE = glfwGetKey(window.getWindow(), GLFW_KEY_E);
        if (stateE == GLFW_PRESS)
            rotation += RotateMultiplier * shiftMult * Time->deltaTime;
        int stateQ = glfwGetKey(window.getWindow(), GLFW_KEY_Q);
        if (stateQ == GLFW_PRESS)
            rotation -= RotateMultiplier * shiftMult * Time->deltaTime;
        rotation = fmod(rotation, 6.28318530718f);

        glm::vec4 nextCamPos = glm::vec4(0, 0, 0, 0);

        int stateW = glfwGetKey(window.getWindow(), GLFW_KEY_W);
        if (stateW == GLFW_PRESS)
            nextCamPos.z += MoveMultiplierZ * shiftMult * speed * Time->deltaTime;
        int stateS = glfwGetKey(window.getWindow(), GLFW_KEY_S);
        if (stateS == GLFW_PRESS)
            nextCamPos.z -= MoveMultiplierZ * shiftMult * speed * Time->deltaTime;

        int stateA = glfwGetKey(window.getWindow(), GLFW_KEY_A);
        if (stateA == GLFW_PRESS)
            nextCamPos.x -= MoveMultiplierX * shiftMult * speed * Time->deltaTime;
        int stateD = glfwGetKey(window.getWindow(), GLFW_KEY_D);
        if (stateD == GLFW_PRESS)
            nextCamPos.x += MoveMultiplierX * shiftMult * speed * Time->deltaTime;

        int stateX = glfwGetKey(window.getWindow(), GLFW_KEY_X);
        if (stateX == GLFW_PRESS)
            nextCamPos.y += MoveMultiplierY * shiftMult * speed * Time->deltaTime;
        int stateZ = glfwGetKey(window.getWindow(), GLFW_KEY_Z);
        if (stateZ == GLFW_PRESS)
            nextCamPos.y -= MoveMultiplierY * shiftMult * speed * Time->deltaTime;

        glm::mat4 camRot = glm::rotate(glm::mat4(1.0f), rotation, glm::vec3(0.0f, -1.0f, 0.0f));

        //std::cout << glm::to_string(camRot) << std::endl;

        nextCamPos = camRot * nextCamPos;

        int stateT = glfwGetKey(window.getWindow(), GLFW_KEY_T);
        if (stateT == GLFW_PRESS)
            nextCamPos.w += MoveMultiplierW * shiftMult * speed * Time->deltaTime;
        int stateG = glfwGetKey(window.getWindow(), GLFW_KEY_G);
        if (stateG == GLFW_PRESS)
            nextCamPos.w -= MoveMultiplierW * shiftMult * speed * Time->deltaTime;

        cameraPosition += nextCamPos;

        //std::cout << vec4ToString(cameraPosition) << " ; " << vec4ToString(nextCamPos) << " ; " << rotation << std::endl;    
    }

    void rManager::run() {

		std::vector<std::unique_ptr<rBuffer>> storageBuffers(rSwapChain::MAX_FRAMES_IN_FLIGHT);
		for (int i = 0; i < rSwapChain::MAX_FRAMES_IN_FLIGHT; i++) {
            storageBuffers[i] = std::make_unique<rBuffer>(
				device,
				sizeof(InstructionData),
                MAX_INSTRUCTIONS,
				VK_BUFFER_USAGE_STORAGE_BUFFER_BIT,
				VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT,
				device.properties.limits.minStorageBufferOffsetAlignment
			);
            storageBuffers[i]->map();
		}

		auto globalSetLayout = rDescriptorSetLayout::Builder(device)
			.addBinding(0, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, VK_SHADER_STAGE_FRAGMENT_BIT)
			.build();

		std::vector<VkDescriptorSet> globalDescriptorSets(rSwapChain::MAX_FRAMES_IN_FLIGHT);
		for (int i = 0; i < globalDescriptorSets.size(); i++) {
			auto bufferInfo = storageBuffers[i]->descriptorInfo();
			rDescriptorWriter(*globalSetLayout, *globalDescriptorPool)
                .writeBuffer(0, &bufferInfo)
				.build(globalDescriptorSets[i]);

		}

		rSystem system(device, model, renderer, renderer.getSwapChainRenderPass(), globalSetLayout->getDescriptorSetLayout());

		float timePassed = 0.0f;

        auto lastUpdatedTime = Time->now();
        auto lastFrameTime = Time->now();

		bool fpsCounter = false;

        while (!window.shouldClose()) {

            auto currentTime = Time->now(); //glfwGetTime();
            double deltaTime = Time->toSeconds(currentTime - lastUpdatedTime);
			double timeSinceLastFrame = Time->toSeconds(currentTime - lastFrameTime);

            glfwPollEvents();

            if (timeSinceLastFrame >= 1.0 / MAX_FPS) {

                if (auto commandbuffer = renderer.beginFrame()) {

					fpsCounter = true;
					if (fpsCounter) {
                        timeSinceLastFrame = Time->toSeconds(currentTime - lastFrameTime);
						std::cout << "FPS: " << 1.0 / timeSinceLastFrame << std::endl;
						fpsCounter = false;
					}

                    fpsCounter = false;

                    Time->deltaTime = deltaTime;
                    cameraControls();
                    main_Update();
                    deltaTime = Time->toSeconds(currentTime - lastUpdatedTime);
                    Time->deltaTime = deltaTime;
                    main_Late_Update();

					int frameIndex = renderer.getFrameIndex();
                    FrameInfo frameInfo{
                        frameIndex,
                        commandbuffer,
                        CameraInfo{rotation, cameraPosition},
                        globalDescriptorSets[frameIndex],
                        *instructionCount
                    };

                    std::cout << "Current Frame: " << frameIndex << std::endl;
                    
				    InstructionData *instructionsData = instructions->data();
                    storageBuffers[frameIndex]->writeToBuffer(instructions->data());
					storageBuffers[frameIndex]->flush();

                    renderer.beginSwapChainRenderPass(commandbuffer);
					system.renderObjects(frameInfo);

					std::cout << "Ending Swapchain Render Pass: " << frameIndex << std::endl;

                    renderer.endSwapChainRenderPass(commandbuffer);

                    std::cout << "Ending Frame: " << frameIndex << std::endl;

                    renderer.endFrame();

                    std::cout << "Current Frame: " << renderer.forceGetFrameIndex() << std::endl;

                    lastFrameTime = currentTime;

                    timePassed += deltaTime;

                    lastUpdatedTime = currentTime;
                }
            }
            else fpsCounter = true;

        }

		std::cout << "Main Loop Ended: " << window.shouldClose() << std::endl;

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

}

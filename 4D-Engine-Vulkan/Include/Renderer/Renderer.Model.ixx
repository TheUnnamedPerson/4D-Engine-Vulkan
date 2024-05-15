module;
#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

// libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

// std
#include <vector>

export module Engine4D.Renderer.Model;

import Engine4D.Renderer.Device;

namespace Engine4D {
    export class rModel {
        public:
        struct Vertex {
            glm::vec3 position;
            glm::vec3 color;

            static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();
            static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();
        };

        rModel(rDevice& _device, const std::vector<Vertex>& vertices);
        ~rModel();

        rModel(const rModel&) = delete;
        rModel& operator=(const rModel&) = delete;

        void bind(VkCommandBuffer commandBuffer);
        void draw(VkCommandBuffer commandBuffer);

        private:
        void createVertexBuffers(const std::vector<Vertex>& vertices);

        rDevice& device;
        VkBuffer vertexBuffer;
        VkDeviceMemory vertexBufferMemory;
        uint32_t vertexCount;
    };
}

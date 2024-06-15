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

#include <memory>

export module Engine4D.Renderer.Model;

import Engine4D.Renderer.Device;
import Engine4D.Renderer.Buffer;

namespace Engine4D {

    export class rModel {
        public:
        struct Vertex {
            glm::vec3 position;
            glm::vec3 color;

            static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();
            static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();
        };

        struct Builder {
            std::vector<Vertex> vertices{};
            std::vector<uint32_t> indices{};
        };

        rModel(rDevice& _device, const rModel::Builder);
        ~rModel();

        rModel(const rModel&) = delete;
        rModel& operator=(const rModel&) = delete;

        void bind(VkCommandBuffer commandBuffer);
        void draw(VkCommandBuffer commandBuffer);

        private:
        void createVertexBuffers(const std::vector<Vertex>& vertices);
        void createIndexBuffers(const std::vector<uint32_t>& indices);

        rDevice& device;

		std::unique_ptr<rBuffer> vertexBuffer;
        uint32_t vertexCount;

		std::unique_ptr<rBuffer> indexBuffer;
        uint32_t indexCount;

		bool hasIndexBuffer = false;
    };
}

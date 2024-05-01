#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <vector>
#include <array>

struct Vertex {
    glm::vec3 pos;
    glm::vec3 color;
    glm::vec2 texCoord;

    static VkVertexInputBindingDescription getBindingDescription() {
        VkVertexInputBindingDescription bindingDescription{};
        bindingDescription.binding = 0;
        bindingDescription.stride = sizeof(Vertex);
        bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

        return bindingDescription;
    }

    static std::array<VkVertexInputAttributeDescription, 3> getAttributeDescriptions() {
        std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions{};
        attributeDescriptions[0].binding = 0;
        attributeDescriptions[0].location = 0;
        attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[0].offset = offsetof(Vertex, pos);

        attributeDescriptions[1].binding = 0;
        attributeDescriptions[1].location = 1;
        attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[1].offset = offsetof(Vertex, color);

        attributeDescriptions[2].binding = 0;
        attributeDescriptions[2].location = 2;
        attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
        attributeDescriptions[2].offset = offsetof(Vertex, texCoord);

        return attributeDescriptions;
    }

    /*static Vertex average(Vertex a, Vertex b) {
        return { {(a.pos.x + b.pos.x) / 2, (a.pos.y + b.pos.y) / 2}, {(a.color.x + b.color.x) / 2, (a.color.y + b.color.y) / 2, (a.color.z + b.color.z) / 2} };
    }*/
};

struct Vertex3D {
    glm::vec3 pos;
};


struct Cube {
    std::vector<Vertex3D> vertices;
    std::vector<uint32_t> indices;

    static Cube rotatedCube(Cube _cube, glm::vec3 rotation) {
        Cube cube = _cube;
        for (int i = 0; i < _cube.vertices.size(); i++) {
            glm::vec3 pos = _cube.vertices[i].pos;
            glm::vec3 newPos = pos;
            newPos.x = pos.x * cos(rotation.y) - pos.z * sin(rotation.y);
            newPos.z = pos.x * sin(rotation.y) + pos.z * cos(rotation.y);
            newPos.y = pos.y * cos(rotation.x) - pos.z * sin(rotation.x);
            newPos.z = pos.y * sin(rotation.x) + pos.z * cos(rotation.x);
            newPos.x = pos.x * cos(rotation.z) - pos.y * sin(rotation.z);
            newPos.y = pos.x * sin(rotation.z) + pos.y * cos(rotation.z);
            cube.vertices[i].pos = newPos;
        }
        return cube;
    };

};

class Camera
{
public:
    Camera();
    ~Camera();
    
    glm::vec3 position;
    glm::vec3 rotation;

    std::vector<Vertex> isometricView (std::vector<Vertex3D> vertices);

private:

};



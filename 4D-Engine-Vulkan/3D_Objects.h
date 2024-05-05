#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>

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
};

struct Vertex3D {
    glm::vec3 pos;
};


struct Mesh {
	glm::vec3 position;
	std::vector<Vertex> vertices; //Vertex Positions are Stored Relative to the Mesh Position
    std::vector<uint32_t> indices;

	std::vector<Vertex> isometricView(glm::vec3 cameraPos) {
		std::vector<Vertex> isometricVertices;
		for (Vertex vertex : vertices) {
			glm::vec3 pos = vertex.pos;
			glm::vec3 color = vertex.color;
			glm::vec2 texCoord = vertex.texCoord;

			glm::vec3 newPos = glm::vec3(pos.x - cameraPos.x, pos.z - cameraPos.z, pos.y - cameraPos.y);
			isometricVertices.push_back({ newPos, color, texCoord });
		}
		return isometricVertices;
	}

	std::vector<Vertex> getVertexes() {
		std::vector<Vertex> vertexPositions;
		for (Vertex vertex : vertices) {
			glm::vec3 pos = vertex.pos;
			glm::vec3 color = vertex.color;
			glm::vec2 texCoord = vertex.texCoord;

			glm::vec3 newPos = glm::vec3(pos.x + position.x, pos.y + position.y, pos.z + position.z);
			vertexPositions.push_back({ newPos, color, texCoord });
		}
		return vertexPositions;
	}

	std::vector<uint32_t> getIndices(uint32_t offset) {
		std::vector<uint32_t> meshIndices;
		for (uint32_t index : indices) {
			meshIndices.push_back(index + offset);
		}
		return meshIndices;
	}

	void scale(float scale) {
		for (Vertex& vertex : vertices) {
			vertex.pos *= scale;
		}
	}

	float getScale() {
		float scale = 0;
		for (Vertex vertex : vertices) {
			if (vertex.pos.x > scale) {
				scale = vertex.pos.x;
			}
			if (vertex.pos.y > scale) {
				scale = vertex.pos.y;
			}
			if (vertex.pos.z > scale) {
				scale = vertex.pos.z;
			}
		}
		return scale;
	}

	void rotate(glm::vec3 rotation) {
		for (Vertex& vertex : vertices) {
			glm::vec3 pos = vertex.pos;
			glm::vec3 newPos = glm::vec3(pos.x, pos.y, pos.z);
			newPos = glm::rotateX(newPos, glm::radians(rotation.x));
			newPos = glm::rotateY(newPos, glm::radians(rotation.y));
			newPos = glm::rotateZ(newPos, glm::radians(rotation.z));
			vertex.pos = newPos;
		}
	}

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



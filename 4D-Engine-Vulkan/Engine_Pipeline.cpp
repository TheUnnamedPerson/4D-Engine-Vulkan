#include "Engine_Pipeline.h"

#include <fstream>
#include <stdexcept>
#include <iostream>

Engine::Engine_Pipeline::Engine_Pipeline(const std::string& vertFilePath, const std::string& fragFilePath)
{
	createGraphicsPipeline(vertFilePath, fragFilePath);
}

Engine::Engine_Pipeline::~Engine_Pipeline()
{
}

std::vector<char> Engine::Engine_Pipeline::readFile(const std::string& filePath)
{
	std::ifstream file(filePath, std::ios::ate | std::ios::binary);

	if (!file.is_open())
		throw std::runtime_error("Failed to open file: " + filePath);

	size_t fileSize = static_cast<size_t>(file.tellg());
	std::vector<char> buffer(fileSize);

	file.seekg(0);
	file.read(buffer.data(), fileSize);

	file.close();

	return buffer;
}

void Engine::Engine_Pipeline::createGraphicsPipeline(const std::string& vertFilePath, const std::string& fragFilePath)
{
	auto vertShaderCode = readFile(vertFilePath);
	auto fragShaderCode = readFile(fragFilePath);

	std::cout << "Vertex Shader Code Size: " << vertShaderCode.size() << "\n";
	std::cout << "Fragment Shader Code Size: " << fragShaderCode.size() << "\n";

	/*VkShaderModule vertShaderModule = createShaderModule(vertShaderCode);
	VkShaderModule fragShaderModule = createShaderModule(fragShaderCode);

	VkPipelineShaderStageCreateInfo vertShaderStageInfo = {};
	vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
	vertShaderStageInfo.module = vertShaderModule;
	vertShaderStageInfo.pName = "main"; // entry point

	VkPipelineShaderStageCreateInfo fragShaderStageInfo = {};
	fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	fragShaderStageInfo.module = fragShaderModule;
	fragShaderStageInfo.pName = "main"; // entry point

	VkPipelineShaderStageCreateInfo shaderStages[] = { vertShaderStageInfo, fragShaderStageInfo };

	// Cleanup
	vkDestroyShaderModule(device, fragShaderModule, nullptr);
	vkDestroyShaderModule(device, vertShaderModule, nullptr);*/
}

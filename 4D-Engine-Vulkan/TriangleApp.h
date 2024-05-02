#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


#include <stb_image.h>

#include "3D_Objects.h"

#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <vector>
#include <cstring>
#include <optional>
#include <fstream>
#include <array>
#include <chrono>

const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600;

const int MAX_FPS = 60;



/*const std::vector<Vertex> vertices = {
    //Front Side
    {{-0.5f, -0.5f, -0.5f}, {0.1f, 0.1f, 0.5f}, {1.0f, 0.0f}}, //0
    {{0.5f, -0.5f, -0.5f}, {0.2f, 0.5f, 0.7f}, {0.0f, 0.0f}}, //1
    {{0.5f, 0.5f, -0.5f}, {0.3f, 0.7f, 1.0f}, {0.0f, 1.0f}}, //2
    {{-0.5f, 0.5f, -0.5f}, {0.05f, 0.25f, 0.7f}, {1.0f, 1.0f}}, //3

    //Back Side
    {{-0.5f, -0.5f, 0.5f}, {0.1f, 0.1f, 0.5f}, {0.0f, 0.0f}}, //4
    {{0.5f, -0.5f, 0.5f}, {0.2f, 0.5f, 0.7f}, {1.0f, 0.0f}}, //5
    {{0.5f, 0.5f, 0.5f}, {0.3f, 0.7f, 1.0f}, {1.0f, 1.0f}}, //6
    {{-0.5f, 0.5f, 0.5f}, {0.05f, 0.25f, 0.7f}, {0.0f, 1.0f}}, //7
};*/

//Sides Have Unique Vertices Due to Texture Mapping
const std::vector<glm::vec2> texCoords1 = {
	{0.5f, 0.0f},
	{0.0f, 0.0f},
	{0.0f, 1.0f},
	{0.5f, 1.0f}
};

const std::vector<glm::vec2> texCoords2 = {
    {1.0f, 0.0f},
    {0.5f, 0.0f},
    {0.5f, 1.0f},
    {1.0f, 1.0f}
};

const std::vector<Vertex> cubeVertices = {
    //Front Side
	{{0.0f, 0.0f, 1.0f}, {0.1f, 0.1f, 0.5f}, texCoords1[0]}, //0
	{{1.0f, 0.0f, 1.0f}, {0.2f, 0.5f, 0.7f}, texCoords1[1]}, //1
	{{1.0f, 1.0f, 1.0f}, {0.3f, 0.7f, 1.0f}, texCoords1[2]}, //2
    {{0.0f, 1.0f, 1.0f}, {0.5f, 0.2f, 0.7f}, texCoords1[3]}, //3

    //Back Side
    {{1.0f, 0.0f, 0.0f}, {0.2f, 0.5f, 0.7f}, texCoords1[0]}, //4
    {{0.0f, 0.0f, 0.0f}, {0.1f, 0.1f, 0.5f}, texCoords1[1]}, //5
    {{0.0f, 1.0f, 0.0f}, {0.5f, 0.2f, 0.7f}, texCoords1[2]}, //6
    {{1.0f, 1.0f, 0.0f}, {0.3f, 0.7f, 1.0f}, texCoords1[3]}, //7

    //Left Side
    {{0.0f, 0.0f, 0.0f}, {0.2f, 0.5f, 0.7f}, texCoords1[0]}, //8
    {{0.0f, 0.0f, 1.0f}, {0.1f, 0.1f, 0.5f}, texCoords1[1]}, //9
    {{0.0f, 1.0f, 1.0f}, {0.5f, 0.2f, 0.7f}, texCoords1[2]}, //10
    {{0.0f, 1.0f, 0.0f}, {0.3f, 0.7f, 1.0f}, texCoords1[3]}, //11

    //Right Side
    {{1.0f, 0.0f, 1.0f}, {0.3f, 0.7f, 1.0f}, texCoords1[0]}, //12
    {{1.0f, 0.0f, 0.0f}, {0.5f, 0.2f, 0.7f}, texCoords1[1]}, //13
    {{1.0f, 1.0f, 0.0f}, {0.1f, 0.1f, 0.5f}, texCoords1[2]}, //14
    {{1.0f, 1.0f, 1.0f}, {0.2f, 0.5f, 0.7f}, texCoords1[3]}, //15

    //Top Side
    {{0.0f, 0.0f, 0.0f}, {0.5f, 0.2f, 0.7f}, texCoords1[3]}, //16
    {{1.0f, 0.0f, 0.0f}, {0.3f, 0.7f, 1.0f}, texCoords1[2]}, //17
    {{1.0f, 0.0f, 1.0f}, {0.2f, 0.5f, 0.7f}, texCoords1[1]}, //18
    {{0.0f, 0.0f, 1.0f}, {0.1f, 0.1f, 0.5f}, texCoords1[0]}, //19

    //Bottom Side
    {{0.0f, 1.0f, 1.0f}, {0.1f, 0.1f, 0.5f}, texCoords1[3]}, //20
    {{1.0f, 1.0f, 1.0f}, {0.2f, 0.5f, 0.7f}, texCoords1[2]}, //21
    {{1.0f, 1.0f, 0.0f}, {0.3f, 0.7f, 1.0f}, texCoords1[1]}, //22
    {{0.0f, 1.0f, 0.0f}, {0.5f, 0.2f, 0.7f}, texCoords1[0]}, //23
    
};

const std::vector<Vertex> cube2Vertices = {
    //Front Side
    {{0.0f, 0.0f, 1.0f}, {0.1f, 0.1f, 0.5f}, texCoords2[0]}, //0
    {{1.0f, 0.0f, 1.0f}, {0.2f, 0.5f, 0.7f}, texCoords2[1]}, //1
    {{1.0f, 1.0f, 1.0f}, {0.3f, 0.7f, 1.0f}, texCoords2[2]}, //2
    {{0.0f, 1.0f, 1.0f}, {0.5f, 0.5f, 0.7f}, texCoords2[3]}, //3

    //Back Side
    {{1.0f, 0.0f, 0.0f}, {0.2f, 0.5f, 0.7f}, texCoords2[0]}, //4
    {{0.0f, 0.0f, 0.0f}, {0.1f, 0.1f, 0.5f}, texCoords2[1]}, //5
    {{0.0f, 1.0f, 0.0f}, {0.5f, 0.2f, 0.7f}, texCoords2[2]}, //6
    {{1.0f, 1.0f, 0.0f}, {0.3f, 0.7f, 1.0f}, texCoords2[3]}, //7

    //Left Side
    {{0.0f, 0.0f, 0.0f}, {0.2f, 0.5f, 0.7f}, texCoords2[0]}, //8
    {{0.0f, 0.0f, 1.0f}, {0.1f, 0.1f, 0.5f}, texCoords2[1]}, //9
    {{0.0f, 1.0f, 1.0f}, {0.5f, 0.2f, 0.7f}, texCoords2[2]}, //10
    {{0.0f, 1.0f, 0.0f}, {0.3f, 0.7f, 1.0f}, texCoords2[3]}, //11

    //Right Side
    {{1.0f, 0.0f, 1.0f}, {0.3f, 0.7f, 1.0f}, texCoords2[0]}, //12
    {{1.0f, 0.0f, 0.0f}, {0.5f, 0.2f, 0.7f}, texCoords2[1]}, //13
    {{1.0f, 1.0f, 0.0f}, {0.1f, 0.1f, 0.5f}, texCoords2[2]}, //14
    {{1.0f, 1.0f, 1.0f}, {0.2f, 0.5f, 0.7f}, texCoords2[3]}, //15

    //Top Side
    {{0.0f, 0.0f, 0.0f}, {0.5f, 0.2f, 0.7f}, texCoords2[3]}, //16
    {{1.0f, 0.0f, 0.0f}, {0.3f, 0.7f, 1.0f}, texCoords2[2]}, //17
    {{1.0f, 0.0f, 1.0f}, {0.2f, 0.5f, 0.7f}, texCoords2[1]}, //18
    {{0.0f, 0.0f, 1.0f}, {0.1f, 0.1f, 0.5f}, texCoords2[0]}, //19

    //Bottom Side
    {{0.0f, 1.0f, 1.0f}, {0.1f, 0.1f, 0.5f}, texCoords2[3]}, //20
    {{1.0f, 1.0f, 1.0f}, {0.2f, 0.5f, 0.7f}, texCoords2[2]}, //21
    {{1.0f, 1.0f, 0.0f}, {0.3f, 0.7f, 1.0f}, texCoords2[1]}, //22
    {{0.0f, 1.0f, 0.0f}, {0.5f, 0.2f, 0.7f}, texCoords2[0]}, //23

};

std::vector<Vertex> generateVertices(std::optional<Vertex> a, std::optional<Vertex> b, std::optional<Vertex> c, int n);

std::vector<uint16_t> generateIndeces(int n);

const std::vector<uint32_t> cubeIndices = {
    //Front Side
    0, 1, 2,
    2, 3, 0,

	//Back Side
	4, 5, 6,
	6, 7, 4,

	//Right Side
	8, 9, 10,
	10, 11, 8,

	//Left Side
	12, 13, 14,
	14, 15, 12,

	//Top Side
	16, 17, 18,
	18, 19, 16,

	//Bottom Side
	20, 21, 22,
	22, 23, 20
};


#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

struct UniformBufferObject {
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 proj;
};

struct QueueFamilyIndices {
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;

    bool isComplete() {
        return graphicsFamily.has_value() && presentFamily.has_value();
    }

};

struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};

class TriangleApp
{
    public:
        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;
        uint32_t indexOffset = 0;
        Mesh cube;
        Mesh cube2;

        void run() {
            cube = { {-0.5f, -0.5f, -0.5f}, cubeVertices, cubeIndices };
            cube2 = { {-0.25f, -2.25f, -0.25f}, cube2Vertices, cubeIndices };
			cube2.scale(0.5f);

            resetVertices();

            //vertices = generateVertices(std::nullopt, std::nullopt, std::nullopt, 1);
            //indices = generateIndeces(1);
            initWindow();
            initVulkan();
            mainLoop();
            cleanup();
        }

    private:
        GLFWwindow* window;
        VkInstance instance;
        VkDebugUtilsMessengerEXT debugMessenger;
        VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
        VkDevice device;
        
        VkSurfaceKHR surface;
        VkSwapchainKHR swapChain;

        std::vector<VkImage> swapChainImages;
        VkFormat swapChainImageFormat;
        VkExtent2D swapChainExtent;

        std::vector<VkImageView> swapChainImageViews;

        VkRenderPass renderPass;

        VkPipeline graphicsPipeline;

        std::vector<VkFramebuffer> swapChainFramebuffers;

        VkCommandPool commandPool;
        std::vector<VkCommandBuffer> commandBuffers;

        VkQueue graphicsQueue;
        VkQueue presentQueue;

        VkDescriptorSetLayout descriptorSetLayout;
        VkPipelineLayout pipelineLayout;

        std::vector<VkSemaphore> imageAvailableSemaphores;
        std::vector<VkSemaphore> renderFinishedSemaphores;
        std::vector<VkFence> inFlightFences;

        bool framebufferResized = false;

        uint32_t currentFrame = 0;

        double lastUpdatedTime = 0.0;
        double lastFrameTime = 0.0;

        VkBuffer vertexBuffer;
        VkDeviceMemory vertexBufferMemory;
        VkBuffer indexBuffer;
        VkDeviceMemory indexBufferMemory;

        std::vector<VkBuffer> uniformBuffers;
        std::vector<VkDeviceMemory> uniformBuffersMemory;
        std::vector<void*> uniformBuffersMapped;

        VkDescriptorPool descriptorPool;
        std::vector<VkDescriptorSet> descriptorSets;

        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;

        VkImage textureImage;
        VkDeviceMemory textureImageMemory;

        VkImageView textureImageView;
        VkSampler textureSampler;

        VkImage depthImage;
        VkDeviceMemory depthImageMemory;
        VkImageView depthImageView;

        const int MAX_FRAMES_IN_FLIGHT = 2;

        void resetVertices()
        {
			indexOffset = 0;
            std::vector<Vertex> _CVs = cube.getVertexes();
            std::vector<Vertex> _CVs2 = cube2.getVertexes();

            std::vector<uint32_t> _CIs = cube.getIndices(indexOffset);
            indexOffset += cube.vertices.size();
            std::vector<uint32_t> _CIs2 = cube2.getIndices(indexOffset);
            indexOffset += cube2.vertices.size();

            vertices.clear();
            indices.clear();

            vertices.resize(_CVs.size() + _CVs2.size());
            indices.resize(_CIs.size() + _CIs2.size());

            std::copy(_CVs.begin(), _CVs.end(), vertices.begin());
            std::copy(_CVs2.begin(), _CVs2.end(), vertices.begin() + _CVs.size());

            std::copy(_CIs.begin(), _CIs.end(), indices.begin());
            std::copy(_CIs2.begin(), _CIs2.end(), indices.begin() + _CIs.size());
        }

        uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

        const std::vector<const char*> validationLayers = {
            "VK_LAYER_KHRONOS_validation",
        };

        const std::vector<const char*> deviceExtensions = {
            VK_KHR_SWAPCHAIN_EXTENSION_NAME
        };

        bool checkValidationLayerSupport();

        void createInstance();

        void initWindow() {
            glfwInit();

			glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
			//glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

			window = glfwCreateWindow(WIDTH, HEIGHT, "Test Window", nullptr, nullptr);
            glfwSetWindowUserPointer(window, this);
            glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
        }

        static void framebufferResizeCallback(GLFWwindow* window, int width, int height);

        void initVulkan() {
            createInstance();
            setupDebugMessenger();
            createSurface();
            pickPhysicalDevice();
            createLogicalDevice();
            createSwapChain();
            createImageViews();
            createRenderPass();
            createDescriptorSetLayout();
            createGraphicsPipeline();
            createCommandPool();
            createDepthResources();
            createFramebuffers();
            createTextureImage();
            createTextureImageView();
            createTextureSampler();
            createVertexBuffer();
            createIndexBuffer();
            createUniformBuffers();
            createDescriptorPool();
            createDescriptorSets();
            createCommandBuffers();
            createSyncObjects();
        }

        void createDepthResources();

        VkFormat findDepthFormat();
        VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
        bool hasStencilComponent(VkFormat format) {
            return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT;
        }

        void createTextureImageView();
        VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);
        void createTextureSampler();

        void createTextureImage();
        void createImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);
        void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
        void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);

        VkCommandBuffer beginSingleTimeCommands();
        void endSingleTimeCommands(VkCommandBuffer commandBuffer);

        void createDescriptorPool();
        void createDescriptorSets();

        void createUniformBuffers();
        void updateUniformBuffer(uint32_t currentImage);

        void createDescriptorSetLayout();

        void createIndexBuffer();

        void createVertexBuffer();
        void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
        void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

        void recreateSwapChain() {
            int width = 0, height = 0;
            glfwGetFramebufferSize(window, &width, &height);
            while (width == 0 || height == 0) {
                if (glfwWindowShouldClose(window))
                    return;
                glfwGetFramebufferSize(window, &width, &height);
                glfwWaitEvents();
            }

            vkDeviceWaitIdle(device);

            cleanupSwapChain();

            createSwapChain();
            createImageViews();
            createDepthResources();
            createFramebuffers();
        }

        void createSyncObjects();

        void createCommandBuffers();

        void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);

        void createCommandPool();

        void createFramebuffers();

        void createRenderPass();

        void createGraphicsPipeline();

        VkShaderModule createShaderModule(const std::vector<char>& code);

        void createImageViews();

        void createSwapChain();

        void createSurface();

        void createLogicalDevice();

        void pickPhysicalDevice();

        bool isDeviceSuitable(VkPhysicalDevice device);

        SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);

        VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
        VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
        VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

        bool checkDeviceExtensionSupport(VkPhysicalDevice device);

        void setupDebugMessenger();

        void mainLoop() {
			float timePassed = 0.0f;
            while (!glfwWindowShouldClose(window)) {
                double currentTime = glfwGetTime();
                double deltaTime = currentTime - lastUpdatedTime;

                glfwPollEvents();

				timePassed += deltaTime * 10.0f;
                if (timePassed > 2 * glm::pi<float>())
                {
                    timePassed -= 2 * glm::pi<float>();
					std::cout << "Time Passed: " << timePassed << std::endl;
                }

                cube.position.r = glm::sin(timePassed) * 7 - (cube.getScale() / 2.0f);

                resetVertices();

				//std::cout << vertices.size() << std::endl;

				std::cout << "Cube Position: " << cube.position.r << std::endl;

				std::cout << "Cube Vertex 1 X: " << cube.getVertexes()[0].pos.r << std::endl;
                std::cout << "Cube Vertex 2 X: " << cube2.getVertexes()[0].pos.r << std::endl;
                std::cout << "Indices: ";
				for (uint32_t index : indices) {
					std::cout << index << " ";
				}
                std::cout << std::endl;

                if (currentTime - lastFrameTime >= 1.0 / MAX_FPS) {
                    drawFrame();
					lastFrameTime = currentTime;					
				}

                lastUpdatedTime = currentTime;
            }

            vkQueueWaitIdle(presentQueue);
        }

        void drawFrame();

        void cleanup();

        void cleanupSwapChain();

        QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);


        std::vector<const char*> getRequiredExtensions() {
            uint32_t glfwExtensionCount = 0;
            const char** glfwExtensions;
            glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

            std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

            if (enableValidationLayers) {
                extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
            }

            return extensions;
        }

        static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
            VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
            VkDebugUtilsMessageTypeFlagsEXT messageType,
            const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
            void* pUserData) {

            std::cerr << "Validation Layer: " << pCallbackData->pMessage << std::endl;

            return VK_FALSE;
        }

        void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo) {
            createInfo = {};
            createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
            createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
            createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
            createInfo.pfnUserCallback = debugCallback;
        }

        static std::vector<char> readFile(const std::string& filename);

};




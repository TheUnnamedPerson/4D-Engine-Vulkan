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



const std::vector<Vertex> vertices = {
    {{-0.5f, -0.5f, -0.5f}, {0.1f, 0.1f, 0.5f}, {1.0f, 0.0f}}, //0
    {{0.5f, -0.5f, -0.5f}, {0.2f, 0.5f, 0.7f}, {0.0f, 0.0f}}, //1
    {{0.5f, 0.5f, -0.5f}, {0.3f, 0.7f, 1.0f}, {0.0f, 1.0f}}, //2
    {{-0.5f, 0.5f, -0.5f}, {0.05f, 0.25f, 0.7f}, {1.0f, 1.0f}}, //3

    {{-0.5f, -0.5f, 0.5f}, {0.1f, 0.1f, 0.5f}, {1.0f, 0.0f}}, //4
    {{0.5f, -0.5f, 0.5f}, {0.2f, 0.5f, 0.7f}, {0.0f, 0.0f}}, //5
    {{0.5f, 0.5f, 0.5f}, {0.3f, 0.7f, 1.0f}, {0.0f, 1.0f}}, //6
    {{-0.5f, 0.5f, 0.5f}, {0.05f, 0.25f, 0.7f}, {1.0f, 1.0f}}, //7
};

std::vector<Vertex> generateVertices(std::optional<Vertex> a, std::optional<Vertex> b, std::optional<Vertex> c, int n);

std::vector<uint16_t> generateIndeces(int n);

const std::vector<uint16_t> indices = {
    0, 1, 2,
    2, 3, 0,
    //2, 4, 3

    6, 5, 4,
    4, 7, 6,

    0, 4, 5,
    5, 1, 0,

    1, 5, 6,
    6, 2, 1,

    2, 6, 7,
    7, 3, 2,

    3, 7, 4,
    4, 0, 3
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
        //std::vector<Vertex> vertices;
        //std::vector<uint16_t> indices;

        void run() {
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
            createFramebuffers();
            createCommandPool();
            createTextureImage();
            createTextureImageView();
            createTextureSampler();
            createDepthResources();
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
            while (!glfwWindowShouldClose(window)) {
                double currentTime = glfwGetTime();
                double deltaTime = currentTime - lastUpdatedTime;

                glfwPollEvents();

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




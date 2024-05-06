module;

// std
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <stdexcept>

module Engine4D.Renderer.Window;

namespace Engine4D {

    rWindow::rWindow(int w, int h, std::string name) : width{ w }, height{ h }, windowName{ name } {
        initWindow();
    }

    rWindow::~rWindow() {
        glfwDestroyWindow(window);
        glfwTerminate();
    }

    void rWindow::initWindow() {
        glfwInit();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

        window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
        glfwSetWindowUserPointer(window, this);
        glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
    }

    void rWindow::createWindowSurface(VkInstance instance, VkSurfaceKHR* surface) {
        if (glfwCreateWindowSurface(instance, window, nullptr, surface) != VK_SUCCESS) {
            throw std::runtime_error("failed to craete window surface");
        }
    }

    void rWindow::framebufferResizeCallback(GLFWwindow* _window, int width, int height) {
        auto __window = reinterpret_cast<rWindow*>(glfwGetWindowUserPointer(_window));
        __window->framebufferResized = true;
        __window->width = width;
        __window->height = height;
    }

}

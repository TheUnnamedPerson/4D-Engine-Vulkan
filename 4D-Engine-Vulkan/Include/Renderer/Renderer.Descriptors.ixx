module;

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <memory>
#include <unordered_map>
#include <vector>

export module Engine4D.Renderer.Descriptors;

import Engine4D.Renderer.Device;


namespace Engine4D {

    export class rDescriptorSetLayout {
        public:
        class Builder {
            public:
            Builder(rDevice& device) : device{ device } {}

            Builder& addBinding(
                uint32_t binding,
                VkDescriptorType descriptorType,
                VkShaderStageFlags stageFlags,
                uint32_t count = 1);
            std::unique_ptr<rDescriptorSetLayout> build() const;

            private:
            rDevice& device;
            std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings{};
        };

        rDescriptorSetLayout(
            rDevice& device, std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings);
        ~rDescriptorSetLayout();
        rDescriptorSetLayout(const rDescriptorSetLayout&) = delete;
        rDescriptorSetLayout& operator=(const rDescriptorSetLayout&) = delete;

        VkDescriptorSetLayout getDescriptorSetLayout() const { return descriptorSetLayout; }

        private:
        rDevice& device;
        VkDescriptorSetLayout descriptorSetLayout;
        std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings;

        friend class rDescriptorWriter;
    };

    export class rDescriptorPool {
        public:
        class Builder {
            public:
            Builder(rDevice& device) : device{ device } {}

            Builder& addPoolSize(VkDescriptorType descriptorType, uint32_t count);
            Builder& setPoolFlags(VkDescriptorPoolCreateFlags flags);
            Builder& setMaxSets(uint32_t count);
            std::unique_ptr<rDescriptorPool> build() const;

            private:
            rDevice& device;
            std::vector<VkDescriptorPoolSize> poolSizes{};
            uint32_t maxSets = 1000;
            VkDescriptorPoolCreateFlags poolFlags = 0;
        };

        rDescriptorPool(
            rDevice& device,
            uint32_t maxSets,
            VkDescriptorPoolCreateFlags poolFlags,
            const std::vector<VkDescriptorPoolSize>& poolSizes);
        ~rDescriptorPool();
        rDescriptorPool(const rDescriptorPool&) = delete;
        rDescriptorPool& operator=(const rDescriptorPool&) = delete;

        bool allocateDescriptor(
            const VkDescriptorSetLayout descriptorSetLayout, VkDescriptorSet& descriptor) const;

        void freeDescriptors(std::vector<VkDescriptorSet>& descriptors) const;

        void resetPool();

        private:
        rDevice& device;
        VkDescriptorPool descriptorPool;

        friend class rDescriptorWriter;
    };

    export class rDescriptorWriter {
        public:
        rDescriptorWriter(rDescriptorSetLayout& setLayout, rDescriptorPool& pool);

        rDescriptorWriter& writeBuffer(uint32_t binding, VkDescriptorBufferInfo* bufferInfo);
        rDescriptorWriter& writeImage(uint32_t binding, VkDescriptorImageInfo* imageInfo);

        bool build(VkDescriptorSet& set);
        void overwrite(VkDescriptorSet& set);

        private:
        rDescriptorSetLayout& setLayout;
        rDescriptorPool& pool;
        std::vector<VkWriteDescriptorSet> writes;
    };

}
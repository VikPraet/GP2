#pragma once

#include "engine/Device.h"

// std
#include <memory>
#include <unordered_map>
#include <vector>

namespace vve
{
    class VveDescriptorSetLayout
	{
    public:
        class Builder
    	{
        public:
            Builder(VveDevice& device) : m_Device{ device } {}

            Builder& addBinding(
                uint32_t binding,
                VkDescriptorType descriptorType,
                VkShaderStageFlags stageFlags,
                uint32_t count = 1);
            std::unique_ptr<VveDescriptorSetLayout> build() const;

        private:
            VveDevice& m_Device;
            std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> m_Bindings{};
        };

        VveDescriptorSetLayout(VveDevice& device, std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings);
        ~VveDescriptorSetLayout();
        VveDescriptorSetLayout(const VveDescriptorSetLayout&) = delete;
        VveDescriptorSetLayout& operator=(const VveDescriptorSetLayout&) = delete;

        VkDescriptorSetLayout getDescriptorSetLayout() const { return m_DescriptorSetLayout; }

    private:
        VveDevice& m_Device;
        VkDescriptorSetLayout m_DescriptorSetLayout;
        std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> m_Bindings;

        friend class VveDescriptorWriter;
    };

    class VveDescriptorPool
	{
    public:
        class Builder
    	{
        public:
            Builder(VveDevice& device) : m_Device{ device } {}

            Builder& addPoolSize(VkDescriptorType descriptorType, uint32_t count);
            Builder& setPoolFlags(VkDescriptorPoolCreateFlags flags);
            Builder& setMaxSets(uint32_t count);
            std::unique_ptr<VveDescriptorPool> build() const;

        private:
            VveDevice& m_Device;
            std::vector<VkDescriptorPoolSize> m_PoolSizes{};
            uint32_t m_MaxSets = 1000;
            VkDescriptorPoolCreateFlags m_PoolFlags = 0;
        };

        VveDescriptorPool(
            VveDevice& device,
            uint32_t maxSets,
            VkDescriptorPoolCreateFlags poolFlags,
            const std::vector<VkDescriptorPoolSize>& poolSizes);
        ~VveDescriptorPool();
        VveDescriptorPool(const VveDescriptorPool&) = delete;
        VveDescriptorPool& operator=(const VveDescriptorPool&) = delete;

        bool allocateDescriptor(
            const VkDescriptorSetLayout descriptorSetLayout, VkDescriptorSet& descriptor) const;

        void freeDescriptors(std::vector<VkDescriptorSet>& descriptors) const;

        void resetPool();

    private:
        VveDevice& m_Device;
        VkDescriptorPool m_DescriptorPool;

        friend class VveDescriptorWriter;
    };

    class VveDescriptorWriter {
    public:
        VveDescriptorWriter(VveDescriptorSetLayout& setLayout, VveDescriptorPool& pool);

        VveDescriptorWriter& writeBuffer(uint32_t binding, VkDescriptorBufferInfo* bufferInfo);
        VveDescriptorWriter& writeImage(uint32_t binding, VkDescriptorImageInfo* imageInfo);

        bool build(VkDescriptorSet& set);
        void overwrite(VkDescriptorSet& set);

    private:
        VveDescriptorSetLayout& m_SetLayout;
        VveDescriptorPool& m_Pool;
        std::vector<VkWriteDescriptorSet> m_Writes;
    };
}
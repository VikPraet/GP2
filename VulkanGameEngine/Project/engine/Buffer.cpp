#include "engine/Buffer.h"

#include <cassert>
#include <cstring>

namespace vve {

    VkDeviceSize VveBuffer::getAlignment(VkDeviceSize instanceSize, VkDeviceSize minOffsetAlignment) {
        if (minOffsetAlignment > 0) 
        {
            return (instanceSize + minOffsetAlignment - 1) & ~(minOffsetAlignment - 1);
        }
        return instanceSize;
    }

    VveBuffer::VveBuffer(VveDevice& device, VkDeviceSize instanceSize, uint32_t instanceCount, VkBufferUsageFlags usageFlags, VkMemoryPropertyFlags memoryPropertyFlags, VkDeviceSize minOffsetAlignment)
        : m_Device{ device },
        m_InstanceSize{ instanceSize },
        m_InstanceCount{ instanceCount },
        m_UsageFlags{ usageFlags },
        m_MemoryPropertyFlags{ memoryPropertyFlags }
	{
        m_AlignmentSize = getAlignment(instanceSize, minOffsetAlignment);
        m_BufferSize = m_AlignmentSize * instanceCount;
        device.createBuffer(m_BufferSize, usageFlags, memoryPropertyFlags, m_Buffer, m_Memory);
    }

    VveBuffer::~VveBuffer() {
        unmap();
        vkDestroyBuffer(m_Device.device(), m_Buffer, nullptr);
        vkFreeMemory(m_Device.device(), m_Memory, nullptr);
    }

    VkResult VveBuffer::map(VkDeviceSize size, VkDeviceSize offset) {
        assert(m_Buffer && m_Memory && "Called map on buffer before create");
        return vkMapMemory(m_Device.device(), m_Memory, offset, size, 0, &m_Mapped);
    }

    void VveBuffer::unmap() {
        if (m_Mapped) {
            vkUnmapMemory(m_Device.device(), m_Memory);
            m_Mapped = nullptr;
        }
    }

    void VveBuffer::writeToBuffer(void* data, VkDeviceSize size, VkDeviceSize offset) {
        assert(m_Mapped && "Cannot copy to unmapped buffer");

        if (size == VK_WHOLE_SIZE) {
            memcpy(m_Mapped, data, m_BufferSize);
        }
        else {
            char* memOffset = (char*)m_Mapped;
            memOffset += offset;
            memcpy(memOffset, data, size);
        }
    }

    VkResult VveBuffer::flush(VkDeviceSize size, VkDeviceSize offset) {
        VkMappedMemoryRange mappedRange = {};
        mappedRange.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
        mappedRange.memory = m_Memory;
        mappedRange.offset = offset;
        mappedRange.size = size;
        return vkFlushMappedMemoryRanges(m_Device.device(), 1, &mappedRange);
    }

    VkResult VveBuffer::invalidate(VkDeviceSize size, VkDeviceSize offset) {
        VkMappedMemoryRange mappedRange = {};
        mappedRange.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
        mappedRange.memory = m_Memory;
        mappedRange.offset = offset;
        mappedRange.size = size;
        return vkInvalidateMappedMemoryRanges(m_Device.device(), 1, &mappedRange);
    }

    VkDescriptorBufferInfo VveBuffer::descriptorInfo(VkDeviceSize size, VkDeviceSize offset) {
        return VkDescriptorBufferInfo{
            m_Buffer,
            offset,
            size,
        };
    }

    void VveBuffer::writeToIndex(void* data, int index) {
        writeToBuffer(data, m_InstanceSize, index * m_AlignmentSize);
    }

    VkResult VveBuffer::flushIndex(int index) { return flush(m_AlignmentSize, index * m_AlignmentSize); }

    VkDescriptorBufferInfo VveBuffer::descriptorInfoForIndex(int index) {
        return descriptorInfo(m_AlignmentSize, index * m_AlignmentSize);
    }

    VkResult VveBuffer::invalidateIndex(int index) {
        return invalidate(m_AlignmentSize, index * m_AlignmentSize);
    }

}

#pragma once
#include "vulkanbase/VulkanUtil.h"

class DAEMesh
{
public:
	void Initialize(VkPhysicalDevice physicalDevice, VkDevice device);
	void DestroyMesh(VkDevice device);
	void Draw(VkCommandBuffer buffer);
	void AddVertex(glm::vec2 pos, glm::vec3 color);

private:
	uint32_t FindMemoryTypes(VkPhysicalDevice physicalDevice, uint32_t typeFilter, VkMemoryPropertyFlags properties);

	VkBuffer m_VkBuffer;
	VkDeviceMemory m_VkBufferMemory;
	std::vector<Vertex> m_Vertices;
};
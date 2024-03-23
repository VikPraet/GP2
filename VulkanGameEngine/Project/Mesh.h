#pragma once

#include "engine/Device.h"
#include "engine/Buffer.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#include <memory>
#include <vector>


namespace vve
{
	class VveMesh
	{
	public:
		struct Vertex {
			glm::vec3 position{};
			glm::vec3 color{};
			glm::vec3 normal{};
			glm::vec2 uv{};

			static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();
			static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();

			bool operator==(const Vertex& other) const
			{
				return position == other.position && color == other.color && normal == other.normal && uv == other.uv;
			}
		};

		struct Builder
		{
			std::vector<Vertex> vertices{};
			std::vector<uint32_t> indices{};

			void loadModel(const std::string& filePath);
		};

		VveMesh(VveDevice& device, const VveMesh::Builder& builder);
		~VveMesh() = default;

		VveMesh(const VveMesh&) = delete;
		VveMesh& operator=(const VveMesh&) = delete;

		static std::unique_ptr<VveMesh> createModelFromFile(VveDevice& device, const std::string& fileName);

		void bind(VkCommandBuffer commandBuffer);
		void draw(VkCommandBuffer commandBuffer);

	private:
		void createVertexBuffers(const std::vector<Vertex>& vertices);
		void createIndexBuffers(const std::vector<uint32_t>& indices);

		VveDevice& m_Device;

		std::unique_ptr<VveBuffer> m_VertexBuffer;
		uint32_t m_VertexCount;

		bool m_HasIndexBuffer = false;
		std::unique_ptr<VveBuffer> m_IndexBuffer;
		uint32_t m_IndexCount;
	};
}

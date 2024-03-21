#pragma once

#include "Mesh.h"

#include <glm/gtc/matrix_transform.hpp>

#include <memory>

namespace vve
{
	struct TransformComponent
	{
		glm::vec3 translation{};
		glm::vec3 scale{1.f, 1.f, 1.f};
		glm:: vec3 rotation;

		glm::mat4 mat4();
		glm::mat3 normalMatrix();
	};

	class VveGameObject
	{
	public:
		using id_t = unsigned int;

		static VveGameObject createGameObject()
		{
			static id_t currentId = 0;
			return VveGameObject{currentId++};
		}

		VveGameObject(const VveGameObject&) = delete;
		VveGameObject& operator=(const VveGameObject&) = delete;
		VveGameObject(VveGameObject&&) = default;
		VveGameObject& operator=(VveGameObject&&) = default;

		id_t getId() { return id; }

		std::shared_ptr<VveMesh> model{};
		glm::vec3 color{};
		TransformComponent transform{};

	private:
		VveGameObject(id_t objId) : id{objId} {}

		id_t id{};
	};
}
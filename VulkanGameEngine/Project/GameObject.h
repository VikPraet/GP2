#pragma once

#include "Model.h"

#include <glm/gtc/matrix_transform.hpp>

#include <memory>

namespace vve
{
	struct TransformComponent
	{
		glm::vec3 translation{};
		glm::vec3 scale{1.f, 1.f, 1.f};
		glm:: vec3 rotation;

		glm::mat4 mat4()
		{
			auto transform = glm::translate(glm::mat4{ 1.f }, translation);

			transform = glm::rotate(transform, rotation.x, {1.f, 0.f, 0.f});
			transform = glm::rotate(transform, rotation.y, {0.f, 1.f, 0.f});
			transform = glm::rotate(transform, rotation.z, {0.f, 0.f, 1.f});

			transform = glm::scale(transform, scale);
			return transform;
		}
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

		std::shared_ptr<VveModel> model{};
		glm::vec3 color{};
		TransformComponent transform{};

	private:
		VveGameObject(id_t objId) : id{objId} {}

		id_t id{};
	};
}
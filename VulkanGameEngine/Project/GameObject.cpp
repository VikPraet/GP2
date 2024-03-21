#include "GameObject.h"

glm::mat4 vve::TransformComponent::mat4()
{
	auto transform = glm::translate(glm::mat4{ 1.f }, translation);

	transform = glm::rotate(transform, rotation.x, {1.f, 0.f, 0.f});
	transform = glm::rotate(transform, rotation.y, {0.f, 1.f, 0.f});
	transform = glm::rotate(transform, rotation.z, {0.f, 0.f, 1.f});

	transform = glm::scale(transform, scale);
	return transform;
}

glm::mat3 vve::TransformComponent::normalMatrix()
{
	auto transform = glm::translate(glm::mat4{ 1.f }, translation);

	transform = glm::rotate(transform, rotation.x, { 1.f, 0.f, 0.f });
	transform = glm::rotate(transform, rotation.y, { 0.f, 1.f, 0.f });
	transform = glm::rotate(transform, rotation.z, { 0.f, 0.f, 1.f });

	const glm::vec3 invScale = 1.0f / scale;
	transform = glm::scale(transform, invScale);
	return transform;
}

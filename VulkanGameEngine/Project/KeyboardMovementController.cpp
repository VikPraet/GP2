#include "KeyboardMovementController.h"

#include "GameTime.h"

void vve::KeyboardMovementController::moveInPlaneXZ(GLFWwindow* window, VveGameObject& gameObject)
{
	glm::vec3 rotate{0};
	if (glfwGetKey(window, m_Keys.lookRight) == GLFW_PRESS) rotate.y += 1.f;
	if (glfwGetKey(window, m_Keys.lookLeft) == GLFW_PRESS) rotate.y -= 1.f;
	if (glfwGetKey(window, m_Keys.lookUp) == GLFW_PRESS) rotate.x += 1.f;
	if (glfwGetKey(window, m_Keys.lookDown) == GLFW_PRESS) rotate.x -= 1.f;

	// check if not zero
	if(glm::dot(rotate, rotate) > std::numeric_limits<float>::epsilon())
	{
		gameObject.transform.rotation += m_LookSpeed * GameTime::GetDeltaTime() * glm::normalize(rotate);
	}

	// limit pitch values between ~85 degrees
	gameObject.transform.rotation.x = glm::clamp(gameObject.transform.rotation.x, -1.5f, 1.5f);
	// prevent y rotation from going over 360 degrees
	gameObject.transform.rotation.y = glm::mod(gameObject.transform.rotation.y, glm::two_pi<float>());

	float yaw = gameObject.transform.rotation.y;
	const glm::vec3 forwardDir{ sin(yaw), 0.f, cos(yaw) };
	const glm::vec3 rightDir{ forwardDir.z, 0.f, -forwardDir.x };
	const glm::vec3 upDir{ 0.f, -1.f, 0.f };

	glm::vec3 moveDir{ 0.f };
	if (glfwGetKey(window, m_Keys.moveForward) == GLFW_PRESS) moveDir += forwardDir;
	if (glfwGetKey(window, m_Keys.moveBackward) == GLFW_PRESS) moveDir -= forwardDir;
	if (glfwGetKey(window, m_Keys.moveRight) == GLFW_PRESS) moveDir += rightDir;
	if (glfwGetKey(window, m_Keys.moveLeft) == GLFW_PRESS) moveDir -= rightDir;
	if (glfwGetKey(window, m_Keys.moveUp) == GLFW_PRESS) moveDir += upDir;
	if (glfwGetKey(window, m_Keys.moveDown) == GLFW_PRESS) moveDir -= upDir;

	// check if not zero
	if (glm::dot(moveDir, moveDir) > std::numeric_limits<float>::epsilon()) 
	{
		gameObject.transform.translation += m_MoveSpeed * GameTime::GetDeltaTime() * glm::normalize(moveDir);
	}
}

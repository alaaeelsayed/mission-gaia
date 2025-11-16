#include "groundedcamera.h"
#include "../../wolf/wolf.h"

GroundedCamera::GroundedCamera(wolf::App* pApp)
	: Camera(pApp)
{
	m_lastMousePos = m_pApp->getMousePos();
	m_pApp->setInputMode(GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	PxCapsuleControllerDesc desc;
	desc.height = 2;
	desc.radius = 0.5f;
	desc.material = wolf::PhysicsManager::Instance()->GetPhysics()->createMaterial(0.5f, 0.5f, 0.1f);
	m_characterController = wolf::PhysicsManager::Instance()->GetControllerManager()->createController(desc);
}

GroundedCamera::~GroundedCamera()
{
	if (m_characterController) {
		m_characterController->release();
		m_characterController = nullptr;
	}
}

void GroundedCamera::Update(float dt)
{
	glm::vec2 mousePos = m_pApp->getMousePos();

	glm::vec2 mouseMovement = mousePos - m_lastMousePos;
	_rotate(mouseMovement);

	glm::vec2 mouseScroll = m_pApp->getMouseScroll();

	if (mouseScroll.y > 0)
	{
		m_fov -= (m_fov / 5.0f);
		m_fov = wolf::max(m_fov, 1.0f);
	}
	else if (mouseScroll.y < 0)
	{
		m_fov += (m_fov / 5.0f);
		m_fov = wolf::min(m_fov, 45.0f);
	}

	const float baseSpeed = 3.0f;
	const float sprintMultiplier = 1.5f;
	const float gravity = -9.8f;
	const float maxFallSpeed = -50.0f;
	const float jumpImpulse = 8.0f;
	const float stepOffset = 0.03f;

	float cameraSpeed = baseSpeed * (m_pApp->isKeyDown(341) ? sprintMultiplier : 1.0f);
	glm::vec3 direction(0.0f);

	if (m_pApp->isKeyDown('w')) {
		direction += glm::normalize(glm::vec3(m_eye.x, 0.0f, m_eye.z));
	}
	if (m_pApp->isKeyDown('s')) {
		direction -= glm::normalize(glm::vec3(m_eye.x, 0.0f, m_eye.z));
	}
	if (m_pApp->isKeyDown('a')) {
		direction -= _getCameraSide();
	}
	if (m_pApp->isKeyDown('d')) {
		direction += _getCameraSide();
	}

	if (glm::length(direction) > 0.0f) {
		direction = glm::normalize(direction);
	}

	glm::vec3 targetVelocity = direction * cameraSpeed;
	m_currentCameraVelocity = glm::mix(m_currentCameraVelocity, targetVelocity, 0.1f);
	PxVec3 horizontalDisplacement = PxVec3(
		m_currentCameraVelocity.x * dt,
		0.0f,
		m_currentCameraVelocity.z * dt
	);

	static float verticalVelocity = 0.0f;
	static bool canJump = false;

	verticalVelocity += gravity * dt;
	verticalVelocity = std::max(verticalVelocity, maxFallSpeed);

	PxControllerCollisionFlags collisionFlags = m_characterController->move(
		horizontalDisplacement + PxVec3(0.0f, verticalVelocity * dt, 0.0f),
		stepOffset,
		dt,
		PxControllerFilters()
	);

	if (!(collisionFlags & PxControllerCollisionFlag::eCOLLISION_DOWN)) {
		verticalVelocity += gravity * dt;
		verticalVelocity = std::max(verticalVelocity, maxFallSpeed);
	}
	else {
		verticalVelocity = 0.0f;
		canJump = true;
	}

	if (canJump && m_pApp->isKeyDown(' ')) {
		verticalVelocity = jumpImpulse;
		canJump = false;
	}

	PxExtendedVec3 pxPosition = m_characterController->getPosition();
	m_position = glm::vec3(
		static_cast<float>(pxPosition.x),
		static_cast<float>(pxPosition.y),
		static_cast<float>(pxPosition.z)
	);

	m_lastMousePos = mousePos;
}

glm::mat4 GroundedCamera::GetViewMatrix()
{
	return glm::lookAt(m_position, _getCameraTarget(), glm::vec3(0.0f, 1.0f, 0.0f));
}

glm::mat4 GroundedCamera::GetProjMatrix(int width, int height)
{
	return glm::perspective(glm::radians(m_fov), (float)width / (float)height, m_near, m_far);
}

glm::vec3 GroundedCamera::_getCameraTarget() const
{
	return m_position + m_eye;
}

void GroundedCamera::_rotate(const glm::vec2& mouseMovement)
{
	m_rotX += mouseMovement.x * 0.1f;
	m_rotY -= mouseMovement.y * 0.1f;
	m_rotY = wolf::max(wolf::min(89.0f, m_rotY), -89.0f);
	m_eye = glm::normalize(glm::vec3(cos(glm::radians(m_rotX)) * cos(glm::radians(m_rotY)), sin(glm::radians(m_rotY)), sin(glm::radians(m_rotX)) * cos(glm::radians(m_rotY))));
}

void GroundedCamera::Reset()
{
	m_position = glm::vec3(0.0f, 0.0f, 3.0f);
	m_eye = glm::vec3(0.0f, 0.0f, -1.0f);
	m_up = glm::vec3(0.0f, 1.0f, 0.0f);
}

glm::vec3 GroundedCamera::_getCameraSide()
{
	glm::vec3 side = glm::cross(m_eye, glm::vec3(0.0f, 1.0f, 0.0f));
	return glm::normalize(side);
}

glm::vec3 GroundedCamera::_getCameraUp()
{
	return glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f));
}

glm::vec3 GroundedCamera::GetViewDirection() const
{
	return m_eye;
}

glm::vec3 GroundedCamera::GetPosition() const
{
	return this->m_position;
}

void GroundedCamera::SetPosition(const glm::vec3& m_position)
{
	this->m_position = m_position;
}
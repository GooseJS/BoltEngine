#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>

#include "BoltEngine/logger/Logger.h"
#include "BoltEngine/window/Window.h"
#include "BoltEngine/GameSettings.h"

namespace Bolt
{
	void Camera::generateProjectionMatrix()
	{
		if (Bolt::GameSettings::getInstance().getMainWindow() != nullptr)
		{
			_projectionMatrix = glm::perspective(glm::radians(_fov), Bolt::GameSettings::getInstance().getMainWindow()->getWindowCfg().aspectRatio, 0.1f, 1000.0f);
			_rebuildProjectionMatrix = false;
		}
	}

	void Camera::generateMatrices()
	{
		_direction = glm::normalize(glm::vec3(cos(glm::radians(_pitch)) * cos(glm::radians(_yaw)), sin(glm::radians(_pitch)), cos(glm::radians(_pitch)) * sin(glm::radians(_yaw))));
		_forward = glm::normalize(glm::vec3(cos(glm::radians(_yaw)), 0, sin(glm::radians(_yaw))));
		_right = glm::normalize(glm::cross(_worldUp, _direction));
		_up = glm::normalize(glm::cross(_direction, _right));

		_viewMatrix = glm::lookAt(_position, _position + _direction, _worldUp);
		if (_rebuildProjectionMatrix)
			generateProjectionMatrix();
	}

	void Camera::rotateCamera(float rotX, float rotY)
	{
		_yaw += _mouseSpeed * Bolt::GameSettings::getInstance().getGameTime().getDeltaTime() * rotX;
		_pitch += _mouseSpeed * Bolt::GameSettings::getInstance().getGameTime().getDeltaTime() * rotY;

		if (_pitch > 89.0f) _pitch = 89.0f;
		else if (_pitch < -89.0f) _pitch = -89.0f;

		if (_yaw > 360.0f) _yaw = 0.f;
		if (_yaw < 0.f) _yaw = 360.0f;
	}

	void Camera::moveCamera(float forwardBack, float strafe, float upDown)
	{
		_position += _forward * Bolt::GameSettings::getInstance().getGameTime().getDeltaTime() * forwardBack;
		_position -= _right * Bolt::GameSettings::getInstance().getGameTime().getDeltaTime() * strafe;
		_position += _worldUp * Bolt::GameSettings::getInstance().getGameTime().getDeltaTime() * upDown;
	}

	void Camera::setCameraPos(float x, float y, float z)
	{
		_position.x = x;
		_position.y = y;
		_position.z = z;
	}
}
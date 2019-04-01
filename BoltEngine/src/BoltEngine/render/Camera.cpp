#include "Camera.h"

#include <glm/gtc/quaternion.hpp>
#include <cmath>

const float PI = 3.14159265358979323846f;

inline float degreeToRad(const float deg)
{
	return (deg * PI) / 180;
}

inline float radToDegree(const float radians)
{
	return (radians * 180) / PI;
}

namespace Bolt
{
	Camera::Camera()
	{
		setPos(glm::vec3(0.0f, 0.0f, 0.0f));
		setFacing(glm::vec3(0.0f, 0.0f, 1.0f));
		setUp(glm::vec3(0.0f, 1.0f, 0.0f));
		setRight(glm::vec3(1.0f, 0.0f, 0.0f));

		_zoom = 10.0f;
		_minZoom = 0.5f;
		_maxZoom = 100.0f;
	}

	void Camera::move(const float speed)
	{
		_position += _facing * speed;
	}

	void Camera::strafe(const float speed)
	{
		_position.x += _right.x * speed;
		_position.y += _right.y * speed;
		_position.z += _right.z * speed;
	}

	void Camera::rotate(const float rotX, const float rotY, const float rotZ)
	{
		glm::quat xRot = angleAxis(degreeToRad(rotX), _right);
		glm::quat yRot = angleAxis(degreeToRad(rotY), _up);
		glm::quat zRot = angleAxis(degreeToRad(rotZ), _facing);

		glm::quat rotation = xRot * yRot * zRot;

		_right = normalize(rotation * _right);
		_up = normalize(rotation * _up);
		_facing = normalize(rotation * _facing);
	}

	void Camera::zoom(const float amount)
	{
		_zoom += amount;
		if (_zoom <= _minZoom)
			_zoom = _minZoom;
		else if (_zoom >= _maxZoom)
			_zoom = _maxZoom;
	}
}
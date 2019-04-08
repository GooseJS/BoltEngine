#pragma once

#include <glm/glm.hpp>

namespace Bolt
{
	class Camera
	{
	private:
		glm::vec3 _position = glm::vec3(0.0f);
		glm::vec3 _direction = glm::vec3(0.0f);
		glm::vec3 _right = glm::vec3(0.0f);
		glm::vec3 _up = glm::vec3(0.0f);

		glm::vec3 _worldUp = glm::vec3(0.0f, 1.0f, 0.0f);

		float _yaw = 0.0f;
		float _pitch = 0.0f;
		float _fov = 90.0f;

		float _mouseSpeed = 0.05f;

		glm::mat4 _projectionMatrix;
		glm::mat4 _viewMatrix;

		bool _rebuildProjectionMatrix = true;

		void generateProjectionMatrix();

	public:
		Camera() {}
		~Camera() = default;
		Camera(const Camera& cam) { _projectionMatrix = cam._projectionMatrix; }
		Camera& operator=(const Camera& cam) = default;

		void generateMatrices();

		void rotateCamera(float rotX, float rotY);
		void moveCamera(float forwardBack, float strafe, float upDown);
		void setCameraPos(float x, float y, float z);

		inline glm::vec3 getPosition() { return _position; }
		inline glm::mat4 getViewMatrix() { return _viewMatrix; }
		inline glm::mat4 getProjectionMatrix() { return _projectionMatrix; }
	};
}
#pragma once

#include <glm/glm.hpp>

namespace Bolt
{
	class Camera
	{
	private:
		glm::vec3 _position = glm::vec3(0.0f);
		glm::vec3 _direction = glm::vec3(0.0f);
		glm::vec3 _forward = glm::vec3(0.0f);
		glm::vec3 _right = glm::vec3(0.0f);
		glm::vec3 _up = glm::vec3(0.0f);

		glm::vec3 _worldUp = glm::vec3(0.0f, 1.0f, 0.0f);

		float _yaw = 0.0f;
		float _pitch = 0.0f;
		float _fov = 90.0f;

		float _mouseSpeed = 5.f;

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

		inline void setCameraPos(const glm::vec3& pos) { _position = pos; }

		inline glm::vec3 getPosition() const { return _position; }
		inline glm::vec3 getFacing() const { return _direction; }
		inline glm::vec3 getForward() const { return _forward; }
		inline glm::vec3 getRight() const { return _right; }
		inline glm::mat4 getViewMatrix() { return _viewMatrix; }
		inline glm::mat4 getProjectionMatrix() const { return _projectionMatrix; }
	};
}
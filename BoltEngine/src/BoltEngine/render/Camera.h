#pragma once

#include <glm/glm.hpp>

namespace Bolt
{
	class Camera
	{
	private:
		glm::vec3 _position;
		glm::vec3 _up;
		glm::vec3 _facing;
		glm::vec3 _right;

		float _zoom;
		float _minZoom;
		float _maxZoom;

		float _fov;

	public:
		Camera();

		inline void setPos(const glm::vec3& pos) { _position = pos; }
		inline void setFacing(const glm::vec3& facing) { _facing = facing; }
		inline void setUp(const glm::vec3& up) { _up = up; }
		inline void setRight(const glm::vec3 &right) { _right = right; }
		inline void setZoom(float amount) { _zoom = amount; }
		inline void setFOV(float fov) { _fov = fov; }

		inline glm::vec3 getPos() const { return _position; }
		inline glm::vec3 getFacing() const { return _facing; }
		inline glm::vec3 getUp() const { return _up; }
		inline glm::vec3 getRight() const { return _right; }
		inline float getZoom() const { return _zoom; }
		inline float getFOV() const { return _fov; }
		inline glm::vec3 getView() const { return (_position + (_facing * _zoom)); }

		void move(const float speed);
		void strafe(const float speed);
		void rotate(const float rotX, const float rotY, const float rotZ);
		//void rotateAroundPoint(const float rotX, const float rotY, const float rotZ);
		void zoom(const float amount);

		//void look() const;
	};
}
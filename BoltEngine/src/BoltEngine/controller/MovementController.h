#pragma once

#include <glm/glm.hpp>

#include "BoltEngine/GL.h"
#include "BoltEngine/Core.h"

namespace Bolt
{
	struct BoltPlayerControls
	{
		int forwardKey = GLFW_KEY_W;
		int backwardKey = GLFW_KEY_S;
		int leftKey = GLFW_KEY_A;
		int rightKey = GLFW_KEY_D;
		int upKey = GLFW_KEY_Q;
		int downKey = GLFW_KEY_Z;
	};

	class MovementController
	{
	private:
		BoltPlayerControls _controls;

	};
}
#pragma once

#include "BoltEngine/voxel/World.h"
#include "BoltEngine/render/Camera.h"

namespace Bolt
{
	struct PlayerMovement
	{
		bool moveForwardPressed;
		bool moveBackPressed;
		bool moveLeftPressed;
		bool moveRightPressed;
		bool moveUpPressed;
		bool moveDownPressed;
	};

	class Player
	{
	private:
		float _x;
		float _y;
		float _z;

		float _moveSpeed;

		Camera _playerCamera;
	public:
		Player()
		{
			_x = 0.0f;
			_y = 0.0f;
			_z = 0.0f;
			_moveSpeed = 0.0001f;
		}

		void handleMovementInput(const PlayerMovement& movement);
		void handleRotationInput(const float rotX, const float rotY);

		void checkCollisions(World* world)
		{
			int xRounded = (int)_x;
			if (_x - (float)xRounded > 0.75f) // Bigger than 1/3 third block (i.e player is intersecting with 1/3 of block)
				return;
		}

		float getX() { return _x; }
		float getY() { return _y; }
		float getZ() { return _z; }

		Camera& getPlayerCamera() { return _playerCamera; }
	};
}
#pragma once

#include "BoltEngine/voxel/World.h"
#include "BoltEngine/render/Camera.h"
#include "BoltEngine/collision/AABB.h"

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

		AABB _feetAABB;
		bool _onGround = false;
	public:
		Player() : _feetAABB(0.75f, 0.0f, 0.75f)
		{
			_x = 0.0f;
			_y = 20.0f;
			_z = 0.0f;
			_playerCamera.setCameraPos(_x, _y, _z);
			_moveSpeed = 5.0f;
		}

		void handleMovementInput(const PlayerMovement& movement);
		void handleRotationInput(const float rotX, const float rotY);

		inline void checkCollisions(World* world)
		{
			BlockPosList& list = _feetAABB.getContainingBlockPos();
			_onGround = false;
			if (list.size() > 0)
			{
				for (auto pos = list.begin(); pos != list.end(); pos++)
				{
					if (world->getBlockAt(*pos).collides())
					{
						_onGround = true;
						return;
					}
				}
			}
		}

		float getX() { return _x; }
		float getY() { return _y; }
		float getZ() { return _z; }

		Camera& getPlayerCamera() { return _playerCamera; }
	};
}
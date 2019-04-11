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

		AABB _aabb;
		bool _colliding = false;
	public:
		Player() : _aabb(0.75f, 2.0f, 0.75f)
		{
			_x = 0.0f;
			_y = 20.0f;
			_z = 0.0f;
			_playerCamera.setCameraPos(_x, _y, _z);
			_moveSpeed = 0.0001f;
		}

		void handleMovementInput(const PlayerMovement& movement);
		void handleRotationInput(const float rotX, const float rotY);

		void checkCollisions(World* world)
		{
			std::vector<BlockPos> blockPos = _aabb.getContainingBlockPos();
			for (auto pos : blockPos)
			{
				if (world->getBlockAt(pos).collides())
				{
					_colliding = true;
					return;
				}
			}

			_colliding = false;
		}

		float getX() { return _x; }
		float getY() { return _y; }
		float getZ() { return _z; }

		Camera& getPlayerCamera() { return _playerCamera; }
	};
}
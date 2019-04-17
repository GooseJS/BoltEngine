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
		bool jumpPressed;
	};

	struct PlayerConfig
	{
		float moveSpeed = 1.0f;

		float jumpPower = 100.0f;

		float gravity = 5.0f;

		glm::vec3 maxAcceleration = glm::vec3(10.f);
		glm::vec3 maxVelocity = glm::vec3(50.f);
	};

	struct PlayerCapabilities
	{
		bool canFly = false;
		bool isFlying = false;
	};

	struct PlayerData
	{
		AABB feetAABB = AABB(0.75f, 0.0f, 0.75f);
		AABB nzAABB = AABB(0.70f, 1.9f, 0.1f);
		AABB pzAABB = AABB(0.70f, 1.9f, 0.1f);
		AABB nxAABB = AABB(0.1f, 1.9f, 0.70f);
		AABB pxAABB = AABB(0.1f, 1.9f, 0.70f);

		bool onGround = false;
		bool collidingNZ = false;
		bool collidingPZ = false;
		bool collidingNX = false;
		bool collidingPX = false;

		bool _jumping = false;
	};

	class Player
	{
	private:
		glm::vec3 _position = glm::vec3(0.f);
		glm::vec3 _acceleration = glm::vec3(0.f);
		glm::vec3 _velocity = glm::vec3(0.f);

		Camera _playerCamera;

		PlayerConfig _playerConfig;
		PlayerCapabilities _playerCapabilities;
		PlayerData _playerData;

		float _playerCenter = 0.75f / 2.0f;
	public:
		Player()
		{
			_position = glm::vec3(5.f, 15.f, 5.f);
			_playerCamera.setCameraPos(_position.x, _position.y + 1.75f, _position.z);
		}

		void handleMovementInput(const PlayerMovement& movement);
		void handleRotationInput(const float rotX, const float rotY);
		void jump();

		void updatePos();
		void updatePhysics();

		void checkCollisions(World* world);

		inline void debugDraw()
		{
			_playerData.feetAABB.debugDraw();
			_playerData.nzAABB.debugDraw();
			_playerData.pzAABB.debugDraw();
			_playerData.nxAABB.debugDraw();
			_playerData.pxAABB.debugDraw();

			glm::vec3 glmOrigin(_position.x, _position.y, _position.z);
			glm::vec3 glmForward = glmOrigin + (_playerCamera.getForward());

			ddVec3 origin = { glmOrigin.x, glmOrigin.y, glmOrigin.z };
			ddVec3 direct = { glmForward.x, glmForward.y, glmForward.z };

			dd::arrow(origin, direct, dd::colors::White, 0.15f);
		}

		inline float getX() { return _position.x; }
		inline float getY() { return _position.y; }
		inline float getZ() { return _position.z; }

		inline Camera& getPlayerCamera() { return _playerCamera; }

		inline PlayerConfig& getPlayerConfig() { return _playerConfig; }
		inline PlayerCapabilities& getPlayerCapabilities() { return _playerCapabilities; }
		inline PlayerData& getPlayerData() { return _playerData; }
	};
}
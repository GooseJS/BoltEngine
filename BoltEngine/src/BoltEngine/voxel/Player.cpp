#include "Player.h"

#include "BoltEngine/GameSettings.h"
#include "BoltEngine/util/VecUtils.h"

namespace Bolt
{
	void Player::handleMovementInput(const PlayerMovement& movement)
	{
		if (!_playerCapabilities.isFlying)
		{
			if (movement.moveForwardPressed)
				_velocity += _playerCamera.getForward() * glm::vec3(_playerConfig.moveSpeed);
			if (movement.moveBackPressed)
				_velocity -= _playerCamera.getForward() * glm::vec3(_playerConfig.moveSpeed);
			if (movement.moveRightPressed)
				_velocity -= _playerCamera.getRight() * glm::vec3(_playerConfig.moveSpeed);
			if (movement.moveLeftPressed)
				_velocity += _playerCamera.getRight() * glm::vec3(_playerConfig.moveSpeed);

			if (movement.jumpPressed && !_jumping)
			{
				_velocity.y += _playerConfig.jumpPower;
				_jumping = true;
			}
			else if (!movement.jumpPressed)
				_jumping = false;
		}
	}

	void Player::handleRotationInput(const float rotX, const float rotY)
	{
		_playerCamera.rotateCamera(rotX, rotY);
	}

	void Player::updatePos()
	{
		// Gravity
		if (!_playerData.onGround)
			_acceleration.y -= _playerConfig.gravity;
		else
		{
			//glm::vec3 friction = -_velocity * glm::vec3(4.f);
			//friction.y = 0.0f;
			//_acceleration += friction;
			
			//_jumping = false;
			//
			//_position.y = ceil(_position.y);

			if (_acceleration.y < 0) _acceleration.y = 0.0f;
			if (_velocity.y < 0) _velocity.y = 0.0f;
			_jumping = false;
		}

		//_acceleration = clampVec3(_acceleration, -_playerConfig.maxAcceleration, _playerConfig.maxAcceleration);
		_velocity += (_acceleration * glm::vec3(Bolt::GameSettings::getInstance().getGameTime().getDeltaTime()));
		_velocity = clampVec3(_velocity, -_playerConfig.maxVelocity, _playerConfig.maxVelocity);
		if (_velocity.x > 0)	{ if (_playerData.collidingPX) _velocity.x = 0; }
		else					{ if (_playerData.collidingNX) _velocity.x = 0; }
		if (_velocity.z > 0)	{ if (_playerData.collidingPZ) _velocity.z = 0; }
		else					{ if (_playerData.collidingNZ) _velocity.z = 0; }

		_position += _velocity * glm::vec3(Bolt::GameSettings::getInstance().getGameTime().getDeltaTime());

		if (_acceleration.x == 0) _velocity.x /= 2.f;
		if (_acceleration.z == 0) _velocity.z /= 2.f;

		_playerData.feetAABB.setCenter(_position.x, _position.y, _position.z);
		_playerData.nzAABB.setCenter(_position.x, _position.y + 1.f, _position.z - _playerCenter);
		_playerData.pzAABB.setCenter(_position.x, _position.y + 1.f, _position.z + _playerCenter);
		_playerData.nxAABB.setCenter(_position.x - _playerCenter, _position.y + 1.f, _position.z);
		_playerData.pxAABB.setCenter(_position.x + _playerCenter, _position.y + 1.f, _position.z);

		_playerCamera.setCameraPos(_position.x, _position.y + 1.75f, _position.z);

		_acceleration = glm::vec3(0.0f);
	}

	void Player::checkCollisions(World* world)
	{
		BlockPosList list = _playerData.feetAABB.getContainingBlockPos();
		_playerData.onGround = false;
		if (list.size() > 0)
		{
			for (auto pos = list.begin(); pos != list.end(); ++pos)
			{
				if (world->getBlockAt(*pos).collides())
				{
					_playerData.onGround = true;
					break;
				}
			}
		}

		list = _playerData.nzAABB.getContainingBlockPos();
		_playerData.collidingNZ = false;
		if (list.size() > 0)
			for (auto pos = list.begin(); pos != list.end(); ++pos)
				if (world->getBlockAt(*pos).collides())
				{
					_playerData.collidingNZ = true;
					break;
				}

		list = _playerData.pzAABB.getContainingBlockPos();
		_playerData.collidingPZ = false;
		if (list.size() > 0)
			for (auto pos = list.begin(); pos != list.end(); ++pos)
				if (world->getBlockAt(*pos).collides())
				{
					_playerData.collidingPZ = true;
					break;
				}

		list = _playerData.nxAABB.getContainingBlockPos();
		_playerData.collidingNX = false;
		if (list.size() > 0)
			for (auto pos = list.begin(); pos != list.end(); ++pos)
				if (world->getBlockAt(*pos).collides())
				{
					_playerData.collidingNX = true;
					break;
				}

		list = _playerData.pxAABB.getContainingBlockPos();
		_playerData.collidingPX = false;
		if (list.size() > 0)
			for (auto pos = list.begin(); pos != list.end(); ++pos)
				if (world->getBlockAt(*pos).collides())
				{
					_playerData.collidingPX = true;
					break;
				}
	}
}
#include "Player.h"

namespace Bolt
{
	void Player::handleMovementInput(const PlayerMovement& movement)
	{
		float forwardBack = 0.0f;
		float strafe = 0.0f;
		float upDown = 0.0f;

		if (movement.moveForwardPressed)
			forwardBack += _moveSpeed;
		if (movement.moveBackPressed)
			forwardBack -= _moveSpeed;
		if (movement.moveLeftPressed)
			strafe -= _moveSpeed;
		if (movement.moveRightPressed)
			strafe += _moveSpeed;
		if (movement.moveUpPressed)
			upDown += _moveSpeed;
		if (movement.moveDownPressed)
			upDown -= _moveSpeed;

		_playerCamera.moveCamera(forwardBack, strafe, upDown);

		glm::vec3 pos = _playerCamera.getPosition();
		_x = pos.x;
		_y = pos.y;
		_z = pos.z;
	}

	void Player::handleRotationInput(const float rotX, const float rotY)
	{
		_playerCamera.rotateCamera(rotX, rotY);
	}
}
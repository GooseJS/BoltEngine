#pragma once

#include "BoltEngine/gameObjects/GameObject.h"

namespace Bolt
{
	class Actor : public BoltGameObject
	{
	public:
		Actor()
		{
			setUpdatable(false);
			setName("Actor");
		}

		virtual void render(float frameTime) override
		{
			// TODO:
		}

		virtual void update() override
		{
			// TODO:
		}
	};
}
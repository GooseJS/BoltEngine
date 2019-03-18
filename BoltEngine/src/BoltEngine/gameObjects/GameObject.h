#pragma once

#include <string>

#include "BoltEngine/Core.h"
#include "BoltEngine/logger/Logger.h"

namespace Bolt
{
	class BoltGameObject
	{
	private:
		bool _updatable = false;
		std::string _name = "GameObject";
	protected:
		inline void setUpdatable(bool updatable) { _updatable = updatable; }
		inline void setName(const std::string& name) { _name = name; }
	public:
		virtual ~BoltGameObject() {}
		virtual void render(float frameTime) = 0;
		virtual void update() = 0;

		inline bool isUpdatable() { return _updatable; }
		inline std::string getName() { return _name; }
	};
}
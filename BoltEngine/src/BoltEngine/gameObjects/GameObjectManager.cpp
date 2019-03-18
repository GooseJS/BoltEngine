#include "BoltEngine/gameObjects/GameObjectManager.h"

namespace Bolt
{
	GameObjectManager& GameObjectManager::getInstance()
	{
		static GameObjectManager instance;
		return instance;
	}
}
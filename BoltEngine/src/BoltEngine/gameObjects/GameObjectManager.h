#pragma once

#include <map>
#include <vector>
#include <functional>
#include <typeindex>
#include <unordered_map>

#include "BoltEngine/Core.h"
#include "BoltEngine/logger/Logger.h"
#include "BoltEngine/gameObjects/GameObject.h"

namespace Bolt
{
	class BOLT_API GameObjectManager
	{
	public:
		typedef std::multimap<const std::type_index, BoltGameObject*> GameObjectMap;

	private:
		GameObjectMap _gameObjects;

		GameObjectManager() {}
	public:
		~GameObjectManager()
		{
			for (auto iter = _gameObjects.begin(); iter != _gameObjects.end(); iter++)
			{
				delete iter->second;
			}
		}

		static GameObjectManager& getInstance();

		template <typename GameObject>
		inline bool addGameObject(GameObject* gameObject)
		{
			auto typeIndex = std::type_index(typeid(GameObject));
			_gameObjects.insert(std::make_pair(typeIndex, gameObject));
			return true;
		}

		template <typename GameObject>
		inline bool deleteGameObject(GameObject* gameObject)
		{
			for (auto iter = _gameObjects.begin(); iter != _gameObjects.end())
			{
				if (iter->second == gameObject)
				{
					delete iter->second;
					iter = data.erase(iter);
				}
				else
					iter++;
			}
		}

		template <typename GameObjectType>
		inline std::vector<GameObjectType*> getGameObjectsOfType()
		{
			auto range = _gameObjects.equal_range(std::type_index(typeid(GameObjectType)));
			std::vector<GameObjectType*> retVal;
			for (auto iter = range.first; iter != range.second; iter++)
			{
				retVal.push_back(iter->second);
			}
			return retVal;
		}

		inline std::vector<BoltGameObject*> getGameObjects()
		{
			std::vector<BoltGameObject*> retVal;
			for (auto iter = _gameObjects.begin(); iter != _gameObjects.end(); iter++)
			{
				retVal.push_back(iter->second);
			}
			return retVal;
		}
	};
}
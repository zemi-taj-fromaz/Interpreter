#pragma once

#include <unordered_map>
#include <string>
#include "object/object.h"

namespace env
{

	struct Environment
	{
		std::unordered_map<std::string, obj::Object*> storage;
		obj::Object* get(std::string key)
		{
			if (storage.find(key) != storage.end())
			{
				return storage[key];
			}

			return nullptr;
		}

		void set(std::string key, obj::Object* obj)
		{
			storage[key] = obj;
		}
	};

}

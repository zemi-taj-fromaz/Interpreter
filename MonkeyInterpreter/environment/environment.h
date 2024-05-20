#pragma once

#include <unordered_map>
#include <string>


namespace obj
{
	struct Object;
}

namespace env
{

	struct Environment
	{

		std::unordered_map<std::string, obj::Object*> storage;
		Environment* outer;
		obj::Object* get(std::string key) {
			if (storage.find(key) != storage.end())
			{
				return storage[key];
			}

			if (!this->outer) return nullptr;
			return this->outer->get(key);
		}

		void set(std::string key, obj::Object* obj)
		{
			this->storage[key] = obj;
		}
	};

	Environment* createEnclosedEnvironment(Environment* e)
	{
		auto* newEnv = new Environment();
		newEnv->outer = e;
		return newEnv;
	}

}

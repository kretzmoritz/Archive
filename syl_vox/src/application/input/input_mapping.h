#ifndef INPUT_MAPPING_H
#define INPUT_MAPPING_H

#include <string>
#include <unordered_map>
#include <vector>

#include "../../libs/tinyxml2.h"
#include "../../utility/callback.h"

class RawInputHandler;

class InputMapping
{
public:
	enum Action
	{
		MOVE_FWD,
		MOVE_LEFT,
		MOVE_BWD,
		MOVE_RIGHT,
		ROTATE_CAMERA,
		BOOST_CAMERA,
		DRAW_OCTREE,
		DO_NOT_TOUCH
	};

	InputMapping(const std::string _path = "input/");
	~InputMapping();
	
	void dispatch(const std::string _context, RawInputHandler* _rawInputHandler);

	template<class T> void registerCB(Action _action, T* _class, void (T::*FunctionPtr)(float))
	{
		m_callback[_action].push_back(new Callback<T, float>(_class, FunctionPtr));
	}

	template<class T> void unregisterCB(Action _action, T* _class, void (T::*FunctionPtr)(float))
	{
		BaseCallback<float>* bcb = new Callback<T, float>(_class, FunctionPtr);

		for(unsigned int i = 0; i < m_callback[_action].size(); ++i)
		{
			if(*m_callback[_action][i] == *bcb)
			{
				delete m_callback[_action][i];
				m_callback[_action].erase(m_callback[_action].begin() + i);
				
				break;
			}
		}

		delete bcb;
	}

private:
	tinyxml2::XMLDocument m_document;

	std::vector<BaseCallback<float>*> m_callback[DO_NOT_TOUCH];
	std::unordered_map<std::string, std::unordered_map<unsigned int, std::pair<float, std::vector<std::pair<int, bool>>>>> m_input; //context -> action -> pair(data, vector(pair(key, once)))
};

#endif
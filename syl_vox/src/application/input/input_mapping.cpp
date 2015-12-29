#include <dirent.h>

#include "input_mapping.h"
#include "raw_input_handler.h"

InputMapping::InputMapping(const std::string _path)
{
	if(DIR* dir = opendir(_path.c_str()))
	{
		while(dirent* ent = readdir(dir))
		{
			std::string file = _path + ent->d_name;

			if(m_document.LoadFile(file.c_str()) == tinyxml2::XML_NO_ERROR)
			{
				tinyxml2::XMLElement* action = m_document.FirstChildElement();

				if(action && action->Attribute("Type") && action->Attribute("Data"))
				{
					tinyxml2::XMLElement* key = action->FirstChildElement();

					if(key && key->Attribute("Id") && key->Attribute("Once"))
					{
						auto it_context = m_input.insert(std::make_pair(ent->d_name, std::unordered_map<unsigned int, std::pair<float, std::vector<std::pair<int, bool>>>>()));

						do
						{
							unsigned int type = atoi(action->Attribute("Type"));
							float data = float(atof(action->Attribute("Data")));

							auto it_action = it_context.first->second.insert(std::make_pair(type, std::make_pair(data, std::vector<std::pair<int, bool>>())));

							do
							{
								int id = strtol(key->Attribute("Id"), NULL, 16);
								bool once = atoi(key->Attribute("Once")) > 0 ? true : false;

								it_action.first->second.second.push_back(std::make_pair(id, once));

								key = key->NextSiblingElement();
							}
							while(key && key->Attribute("Id") && key->Attribute("Once"));

							action = action->NextSiblingElement();
						}
						while(action && action->Attribute("Type") && action->Attribute("Data") && (key = action->FirstChildElement()) && key->Attribute("Id") && key->Attribute("Once"));
					}
				}
			}
		}
		
		closedir(dir);
	}
}

InputMapping::~InputMapping()
{
	for(unsigned int i = 0; i < DO_NOT_TOUCH; ++i)
	{
		for(unsigned int j = 0; j < m_callback[i].size(); ++j)
		{
			delete m_callback[i][j];
		}

		m_callback[i].clear();
	}
}

void InputMapping::dispatch(const std::string _context, RawInputHandler* _rawInputHandler)
{
	auto it_context = m_input.find(_context);
	if(it_context == m_input.end())
		return;

	for(unsigned int i = 0; i < DO_NOT_TOUCH; ++i)
	{
		auto it_action = it_context->second.find(i);
		if(it_action == it_context->second.end())
			continue;

		auto it_key = it_action->second.second;
		bool valid = true;

		for(unsigned int j = 0; j < it_key.size(); ++j)
		{
			if(it_key[j].second)
			{
				if(!_rawInputHandler->isPressed(it_key[j].first))
				{
					valid = false;
					break;
				}
			}
			else
			{
				if(!_rawInputHandler->isDown(it_key[j].first))
				{
					valid = false;
					break;
				}
			}
		}

		if(valid)
		{
			for(unsigned int j = 0; j < m_callback[i].size(); ++j)
			{
				(*m_callback[i][j])(it_action->second.first);
			}
		}
	}
}
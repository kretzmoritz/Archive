#include "..\..\include\Game\CEnemyData.h"

//Definition of static variable and vectors
pwIniFile *CEnemyData::data;

std::vector<std::string> CEnemyData::m_vTexture;
std::vector<CEnemyData::position> CEnemyData::m_vPosition;
std::vector<CEnemyData::size> CEnemyData::m_vSize;
std::vector<int> CEnemyData::m_vFrames;
std::vector<CEnemyData::color> CEnemyData::m_vFlashColor;
std::vector<CEnemyData::animation> CEnemyData::m_vAnimation;
std::vector<float> CEnemyData::m_vAnimationSpeed;

void CEnemyData::init()
{
	data = glDataBox->GetIniFile("enemydata.ini");
	
	std::stringstream string_typecast;
	std::string input;

	position position_data;
	size size_data;
	color color_data;
	animation animation_data;

	for(int i = 0; i < data->GetInt("enemy_count"); i++)
	{
		string_typecast << i;
		string_typecast >> input;
		
		m_vTexture.push_back(data->GetString("texture", input));

		position_data.x = data->GetFloat("positionX", input);
		position_data.y = data->GetFloat("positionY", input);
		m_vPosition.push_back(position_data);

		size_data.width = data->GetInt("width", input);
		size_data.height = data->GetInt("height", input);
		m_vSize.push_back(size_data);

		m_vFrames.push_back(data->GetInt("frames", input));

		color_data.r = data->GetInt("color_r", input);
		color_data.g = data->GetInt("color_g", input);
		color_data.b = data->GetInt("color_b", input);
		m_vFlashColor.push_back(color_data);

		animation_data.bPlayMoveOnce = data->GetBool("playmoveonce", input);
		animation_data.moveStart = data->GetInt("moveStart", input);
		animation_data.moveEnd = data->GetInt("moveEnd", input);
		animation_data.attackStart = data->GetInt("attackStart", input);
		animation_data.attackEnd = data->GetInt("attackEnd", input);
		animation_data.backpedalStart = data->GetInt("backpedalStart", input);
		animation_data.backpedalEnd = data->GetInt("backpedalEnd", input);
		animation_data.woundedStart = data->GetInt("woundedStart", input);
		animation_data.woundedEnd = data->GetInt("woundedEnd", input);
		animation_data.deathStart = data->GetInt("deathStart", input);
		animation_data.deathEnd = data->GetInt("deathEnd", input);
		m_vAnimation.push_back(animation_data);

		m_vAnimationSpeed.push_back(data->GetFloat("animationspeed", input));

		string_typecast.clear();
	}
}

std::string CEnemyData::getTexture(int _type) const
{
	std::string *p = static_cast<std::string*>(dataHandler(_type, TEXTURE_ENEMY));
	std::string data = *p;
	delete p;
	return data;
}

CEnemyData::position CEnemyData::getPosition(int _type) const
{
	position *p = static_cast<position*>(dataHandler(_type, POSITION_ENEMY));
	position data = *p;
	delete p;
	return data;
}

CEnemyData::size CEnemyData::getSize(int _type) const
{
	size *p = static_cast<size*>(dataHandler(_type, SIZE_ENEMY));
	size data = *p;
	delete p;
	return data;
}

int CEnemyData::getFrames(int _type) const
{
	int *p = static_cast<int*>(dataHandler(_type, FRAMES_ENEMY));
	int data = *p;
	delete p;
	return data;
}

CEnemyData::color CEnemyData::getFlashColor(int _type) const
{
	color *p = static_cast<color*>(dataHandler(_type, COLOR_ENEMY));
	color data = *p;
	delete p;
	return data;
}

CEnemyData::animation CEnemyData::getAnimation(int _type) const
{
	animation *p = static_cast<animation*>(dataHandler(_type, ANIMATION_ENEMY));
	animation data = *p;
	delete p;
	return data;
}

float CEnemyData::getAnimationSpeed(int _type) const
{
	float *p = static_cast<float*>(dataHandler(_type, ANIMATION_SPEED_ENEMY));
	float data = *p;
	delete p;
	return data;
}

void* CEnemyData::dataHandler(int _type, int _question) const
{
	void* result;

	switch(_question)
	{
		case TEXTURE_ENEMY:
			result = static_cast<void*>(new std::string(m_vTexture[_type]));
			break;
		case POSITION_ENEMY:
			result = static_cast<void*>(new position(m_vPosition[_type]));
			break;
		case SIZE_ENEMY:
			result = static_cast<void*>(new size(m_vSize[_type]));
			break;
		case FRAMES_ENEMY:
			result = static_cast<void*>(new int(m_vFrames[_type]));
			break;
		case COLOR_ENEMY:
			result = static_cast<void*>(new color(m_vFlashColor[_type]));
			break;
		case ANIMATION_ENEMY:
			result = static_cast<void*>(new animation(m_vAnimation[_type]));
			break;
		case ANIMATION_SPEED_ENEMY:
			result = static_cast<void*>(new float(m_vAnimationSpeed[_type]));
			break;
	}

	return result;
}
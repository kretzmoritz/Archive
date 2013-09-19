#include "..\..\include\Game\CProjectileData.h"

//Definition of static variable and vectors
pwIniFile *CProjectileData::data;

std::vector<std::string> CProjectileData::m_vTexture;
std::vector<std::string> CProjectileData::m_vSound;
std::vector<int> CProjectileData::m_vTarget;
std::vector<float> CProjectileData::m_vDamage;
std::vector<float> CProjectileData::m_vBounceChance;
std::vector<CProjectileData::size> CProjectileData::m_vSize;
std::vector<int> CProjectileData::m_vFrames;
std::vector<CProjectileData::animation> CProjectileData::m_vAnimation;
std::vector<float> CProjectileData::m_vAnimationSpeed;
std::vector<float> CProjectileData::m_vGravity;
std::vector<float> CProjectileData::m_vRotation;
std::vector<CProjectileData::position> CProjectileData::m_vOrigin;

void CProjectileData::init()
{
	data = glDataBox->GetIniFile("projectiledata.ini");
	
	std::stringstream string_typecast;
	std::string input;

	size size_data;
	animation animation_data;
	position position_data;

	for(int i = 0; i < data->GetInt("projectile_count"); i++)
	{
		string_typecast << i;
		string_typecast >> input;
		
		m_vTexture.push_back(data->GetString("texture", input));

		m_vSound.push_back(data->GetString("sound", input));

		m_vTarget.push_back(data->GetInt("target", input));

		m_vDamage.push_back(data->GetFloat("damage", input));

		m_vBounceChance.push_back(data->GetFloat("bouncechance", input));

		size_data.width = data->GetInt("width", input);
		size_data.height = data->GetInt("height", input);
		m_vSize.push_back(size_data);

		m_vFrames.push_back(data->GetInt("frames", input));

		animation_data.normalStart = data->GetInt("normalStart", input);
		animation_data.normalEnd = data->GetInt("normalEnd", input);
		animation_data.deathStart = data->GetInt("deathStart", input);
		animation_data.deathEnd = data->GetInt("deathEnd", input);
		m_vAnimation.push_back(animation_data);

		m_vAnimationSpeed.push_back(data->GetFloat("animationspeed", input));

		m_vGravity.push_back(data->GetFloat("gravity", input));

		m_vRotation.push_back(data->GetFloat("rotation", input));

		position_data.x = data->GetFloat("originX", input);
		position_data.y = data->GetFloat("originY", input);
		m_vOrigin.push_back(position_data);

		string_typecast.clear();
	}
}

std::string CProjectileData::getTexture(int _type) const
{
	std::string *p = static_cast<std::string*>(dataHandler(_type, TEXTURE_PROJECTILE));
	std::string data = *p;
	delete p;
	return data;
}

std::string CProjectileData::getSound(int _type) const
{
	std::string *p = static_cast<std::string*>(dataHandler(_type, SOUND_PROJECTILE));
	std::string data = *p;
	delete p;
	return data;
}

int CProjectileData::getTarget(int _type) const
{
	int *p = static_cast<int*>(dataHandler(_type, TARGET_PROJECTILE));
	int data = *p;
	delete p;
	return data;
}

float CProjectileData::getDamage(int _type) const
{
	float *p = static_cast<float*>(dataHandler(_type, DAMAGE_PROJECTILE));
	float data = *p;
	delete p;
	return data;
}

float CProjectileData::getBounceChance(int _type) const
{
	float *p = static_cast<float*>(dataHandler(_type, BOUNCE_CHANCE_PROJECTILE));
	float data = *p;
	delete p;
	return data;
}

CProjectileData::size CProjectileData::getSize(int _type) const
{
	size *p = static_cast<size*>(dataHandler(_type, SIZE_PROJECTILE));
	size data = *p;
	delete p;
	return data;
}

int CProjectileData::getFrames(int _type) const
{
	int *p = static_cast<int*>(dataHandler(_type, FRAMES_PROJECTILE));
	int data = *p;
	delete p;
	return data;
}

CProjectileData::animation CProjectileData::getAnimation(int _type) const
{
	animation *p = static_cast<animation*>(dataHandler(_type, ANIMATION_PROJECTILE));
	animation data = *p;
	delete p;
	return data;
}

float CProjectileData::getAnimationSpeed(int _type) const
{
	float *p = static_cast<float*>(dataHandler(_type, ANIMATION_SPEED_PROJECTILE));
	float data = *p;
	delete p;
	return data;
}

float CProjectileData::getGravity(int _type) const
{
	float *p = static_cast<float*>(dataHandler(_type, GRAVITY_PROJECTILE));
	float data = *p;
	delete p;
	return data;
}

float CProjectileData::getRotation(int _type) const
{
	float *p = static_cast<float*>(dataHandler(_type, ROTATION_PROJECTILE));
	float data = *p;
	delete p;
	return data;
}

CProjectileData::position CProjectileData::getOrigin(int _type) const
{
	position *p = static_cast<position*>(dataHandler(_type, ORIGIN_PROJECTILE));
	position data = *p;
	delete p;
	return data;
}

void* CProjectileData::dataHandler(int _type, int _question) const
{
	void* result;

	switch(_question)
	{
		case TEXTURE_PROJECTILE:
			result = static_cast<void*>(new std::string(m_vTexture[_type]));
			break;
		case SOUND_PROJECTILE:
			result = static_cast<void*>(new std::string(m_vSound[_type]));
			break;
		case TARGET_PROJECTILE:
			result = static_cast<void*>(new int(m_vTarget[_type]));
			break;
		case DAMAGE_PROJECTILE:
			result = static_cast<void*>(new float(m_vDamage[_type]));
			break;
		case BOUNCE_CHANCE_PROJECTILE:
			result = static_cast<void*>(new float(m_vBounceChance[_type]));
			break;
		case SIZE_PROJECTILE:
			result = static_cast<void*>(new size(m_vSize[_type]));
			break;
		case FRAMES_PROJECTILE:
			result = static_cast<void*>(new int(m_vFrames[_type]));
			break;
		case ANIMATION_PROJECTILE:
			result = static_cast<void*>(new animation(m_vAnimation[_type]));
			break;
		case ANIMATION_SPEED_PROJECTILE:
			result = static_cast<void*>(new float(m_vAnimationSpeed[_type]));
			break;
		case GRAVITY_PROJECTILE:
			result = static_cast<void*>(new float(m_vGravity[_type]));
			break;
		case ROTATION_PROJECTILE:
			result = static_cast<void*>(new float(m_vRotation[_type]));
			break;
		case ORIGIN_PROJECTILE:
			result = static_cast<void*>(new position(m_vOrigin[_type]));
			break;
	}

	return result;
}
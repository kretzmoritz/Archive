#include <Windows.h>

#include "ini_reader.h"

IniReader::IniReader(std::string _file)
	: m_file(_file)
{

}

int IniReader::readInt(std::string _section, std::string _key)
{
	return GetPrivateProfileInt(_section.c_str(), _key.c_str(), NULL, m_file.c_str());
}

float IniReader::readFloat(std::string _section, std::string _key)
{
	char buffer[255];
	GetPrivateProfileString(_section.c_str(), _key.c_str(), NULL, buffer, 255, m_file.c_str());

	return float(atof(buffer));
}

bool IniReader::readBool(std::string _section, std::string _key)
{
	return GetPrivateProfileInt(_section.c_str(), _key.c_str(), NULL, m_file.c_str()) != 0;
}

std::string IniReader::readString(std::string _section, std::string _key)
{
	char buffer[255];
	GetPrivateProfileString(_section.c_str(), _key.c_str(), NULL, buffer, 255, m_file.c_str());

	return buffer;
}
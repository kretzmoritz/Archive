#ifndef INI_READER_H
#define INI_READER_H

#include <string>

class IniReader
{
public:
	IniReader(std::string _file);

	int readInt(std::string _section, std::string _key);
	float readFloat(std::string _section, std::string _key);
	bool readBool(std::string _section, std::string _key);
	std::string readString(std::string _section, std::string _key);

private:
	std::string m_file;
};

#endif
#include "amf3.h"

void Encoder::writeBool(std::string _name, bool _b)
{
	Object o;
	o.type = _b ? DT_TRUE : DT_FALSE;

	m_objects.insert(std::make_pair(_name, o));
}

void Encoder::writeInt(std::string _name, int _i)
{
	Object o;
	o.type = DT_INTEGER;
	o.i = _i;

	m_objects.insert(std::make_pair(_name, o));
}

void Encoder::writeDouble(std::string _name, double _d)
{
	Object o;
	o.type = DT_DOUBLE;
	o.d = _d;

	m_objects.insert(std::make_pair(_name, o));
}

void Encoder::writeString(std::string _name, std::string _str)
{
	Object o;
	o.type = DT_STRING;
	o.str = _str;

	m_objects.insert(std::make_pair(_name, o));
}

void Encoder::clear()
{
	m_objects.clear();
}

char* Encoder::encode()
{
	std::vector<char> temp;
	temp.push_back(0x03);
	temp.push_back(0x00);

	for (auto kv : m_objects)
	{
		temp.push_back(kv.first.length);

		for (int i = 0; i < kv.first.length; ++i)
		{
			temp.push_back(kv.first[i]);
		}

		temp.push_back(kv.second.type);
		temp.push_back(0x00);

		switch (kv.second.type)
		{
		case DT_FALSE:
			break;
		case DT_TRUE:
			break;
		case DT_INTEGER:
			break;
		case DT_DOUBLE:
			break;
		case DT_STRING:
			break;
		}
	}
}

std::unordered_map<std::string, Object> Decoder::decode(char* _byteArray)
{

}
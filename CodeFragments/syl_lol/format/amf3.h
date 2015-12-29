//http://en.wikipedia.org/wiki/Action_Message_Format

#ifndef AMF3
#define AMF3

#include <unordered_map>
#include <string>

namespace AMF3
{
	enum DataType
	{
		DT_UNDEFINED = 0x00,
		DT_NULL = 0x01,
		DT_FALSE = 0x02,
		DT_TRUE = 0x03,
		DT_INTEGER = 0x04,
		DT_DOUBLE = 0x05,
		DT_STRING = 0x06,
		DT_XML = 0x07,
		DT_DATE = 0x08,
		DT_ARRAY = 0x09,
		DT_OBJECT = 0x0A,
		DT_XML_END = 0x0B,
		DT_BYTEARRAY = 0x0C
	};

	struct Object
	{
	public:
		DataType type;

		int i;
		double d;
		std::string str;
	};

	class Encoder
	{
	public:
		void writeBool(std::string _name, bool _b);
		void writeInt(std::string _name, int _i);
		void writeDouble(std::string _name, double _d);
		void writeString(std::string _name, std::string _str);

		void clear();

		char* encode();

	private:
		std::unordered_map<std::string, Object> m_objects;
	};

	class Decoder
	{
	public:
		std::unordered_map<std::string, Object> decode(char* _byteArray);
	};
}

#endif
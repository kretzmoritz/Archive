#ifndef LOG_H
#define LOG_H

#include <Windows.h>
#include <iostream>
#include <string>
#include <typeinfo>

enum LogType
{
	LOG_MSG = 10,
	LOG_ERR = 12,
	LOG_DBG = 15
};

template<class T>
class Log
{
public:
	void Out(LogType _type, std::string _string)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), _type);
		std::cout << typeid(T).name() << ": " << _string << std::endl;
		
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE); //Reset to standard color
	}
};

#endif
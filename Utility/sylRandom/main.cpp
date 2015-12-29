#include <iostream>
#include "sylRandom.h"

int main()
{
	do
	{
		system("cls");

		std::cout << sylRandom::getNumber<int>(-3, -1) << std::endl;
		std::cout << sylRandom::getNumber<float>(-3, -1) << std::endl;
		std::cout << sylRandom::getNumber<int>(5, 20) << std::endl;
		std::cout << sylRandom::getNumber<float>(5.3f, 22.34f) << std::endl;
	}
	while(std::getchar() != 'q');
}
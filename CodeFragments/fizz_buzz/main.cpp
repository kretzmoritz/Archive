#include <iostream>
#include <string>
#include <sstream>

int main()
{
	std::stringstream ss;

	for (int i = 1; i < 101; ++i)
	{
		std::string out;

		if (i % 3 == 0)
		{
			out += "Fizz";
		}

		if (i % 5 == 0)
		{
			out += "Buzz";
		}
		
		if (out.empty())
		{
			ss << i;
			ss >> out;
			ss.clear();
		}

		std::cout << out << std::endl;
	}

	system("pause");
}
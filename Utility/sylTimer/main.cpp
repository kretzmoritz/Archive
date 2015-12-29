#include <iostream>
#include "sylTimer.h"

class test
{
	public:
		void test_function()
		{
			std::cout << "It works!" << std::endl;
		}
};

class blub
{
	public:
		void do_something()
		{
			timer(5.0f);
			timer.addEventListener(TimerTick, &test::test_function, &test_class);
			timer.start();
		}

		sylTimer timer;
		test test_class;
};

int main()
{
	blub blub_class;
	blub_class.do_something();
	
	while(true)
	{
		sylTimer::Update();
	}

	return 0;
}
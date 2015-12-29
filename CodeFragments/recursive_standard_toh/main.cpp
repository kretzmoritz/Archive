#include <iostream>

void move(int i, int a, int b, int c)
{
	if (i > 0)
	{
		move(i - 1, a, c, b);

		std::cout << "Move disc " << i << " from peg " << a << " to peg " << c << std::endl;

		move(i - 1, b, a, c);
	}
}

int main()
{
	move(3, 1, 2, 3);

	std::getchar();
	return 0;
}
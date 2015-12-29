////////////////////////////////////////////////////////////
// Moritz Kretz
// System implementation of complementary-multiply-with-carry random number generator
////////////////////////////////////////////////////////////

#ifndef SYL_RANDOM
#define SYL_RANDOM

#include <math.h>
#include <time.h>
#include <limits>

#define PHI 0x9e3779b9
#define MAX_UNSIGNED_INT std::numeric_limits<unsigned int>::max() //or 0xffffffff

class sylRandom
{
public:
	template<class T>
	static T getNumber(T _low, T _high)
	{
		if(validateType<T>())
		{
			double dRange = rand_cmwc() * (1.0 / MAX_UNSIGNED_INT);
			return (T)floor(dRange * (_high + 0.99 - _low) + _low); // Even range for every number (distribution)
		}
		else
		{
			double dRange = rand_cmwc() * (1.0 / MAX_UNSIGNED_INT);
			return (T)(dRange * (_high - _low) + _low);
		}
	}

	static void reseed(unsigned int _x);

private:
	static bool bInitialized;
	static unsigned int Q[4096], c;

	sylRandom(); // No object can be created
	static unsigned int rand_cmwc();

	// Type check
	template<class T>
	static bool validateType() // Float, double, custom classes...
	{
		return false;
	}

	template<>
	static bool validateType<char>() // Even numbers
	{
		return true;
	}

	template<>
	static bool validateType<short>()
	{
		return true;
	}

	template<>
	static bool validateType<int>()
	{
		return true;
	}

	template<>
	static bool validateType<long>()
	{
		return true;
	}

	template<>
	static bool validateType<long long>()
	{
		return true;
	}

	template<>
	static bool validateType<unsigned char>()
	{
		return true;
	}

	template<>
	static bool validateType<unsigned short>()
	{
		return true;
	}

	template<>
	static bool validateType<unsigned int>()
	{
		return true;
	}

	template<>
	static bool validateType<unsigned long>()
	{
		return true;
	}

	template<>
	static bool validateType<unsigned long long>()
	{
		return true;
	}
};

#endif
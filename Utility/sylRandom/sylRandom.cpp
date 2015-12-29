#include "sylRandom.h"

bool sylRandom::bInitialized = false;
unsigned int sylRandom::Q[4096], sylRandom::c = 362436;

void sylRandom::reseed(unsigned int _x)
{
	Q[0] = _x;
	Q[1] = _x + PHI;
	Q[2] = _x + PHI + PHI;
	
	for(int i = 3; i < 4096; i++)
	{
		Q[i] = Q[i - 3] ^ Q[i - 2] ^ PHI ^ i;
	}

	bInitialized = true; // Only do setup once
}

unsigned int sylRandom::rand_cmwc()
{
	if(!bInitialized)
	{
		reseed((unsigned int)time(NULL));
	}

	unsigned long long t, a = 18782LL;
	unsigned int i = 4095;
	unsigned int x, r = 0xfffffffe;
	i = (i + 1) & 4095;
	t = a * Q[i] + c;
	c = (t >> 32);
	x = (unsigned int)(t + c);
	
	if (x < c)
	{
		x++;
		c++;
	}
	
	return (Q[i] = r - x);
}
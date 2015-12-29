#ifndef HELPER_H
#define HELPER_H

namespace Helper
{
	int getNextPowerOf2(int _v)
	{
		_v--;
		_v |= _v >> 1;
		_v |= _v >> 2;
		_v |= _v >> 4;
		_v |= _v >> 8;
		_v |= _v >> 16;
		_v++;
		
		return _v;
	}
};

#endif
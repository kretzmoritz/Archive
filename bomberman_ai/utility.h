#ifndef UTILITY_H
#define UTILITY_H

#include "../gameplay/game_state.h"
#include "../gameplay/game_state_io.h"

struct position
{
	position() {};
	position(int _x, int _y) : x(_x), y(_y) {};

	int x;
	int y;
	
	int xParent;
	int yParent;

	float F; //F = G + H
	float G;
	float H;

	int iDistanceToPlayer; //Combined movement cost
	int iOwner;

	bool operator!=(const position& rhs)
	{
		return (x != rhs.x) || (y != rhs.y);
	}

	bool operator==(const position& rhs)
	{
		return (x == rhs.x) && (y == rhs.y);
	}
};

class Utility
{
public:
	void update(CLocalState _local, CGameState _state);
	position getPlayerPosition(int _id);

private:
	CLocalState m_local;
	CGameState m_state;
};

#endif
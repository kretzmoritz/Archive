#ifndef CLOAD
#define CLOAD

#include "..\pwSystem.hpp"
#include "ID.hpp"

#include "CEnemyData.h"
#include "CProjectileData.h"
#include "CSquadData.h"

class CLoad : public pwGameState
{
public:

    CLoad();

	~CLoad();

    bool Start();
	unsigned int Run();

private:
    sf::Sprite m_ciBackground;
    sf::Sprite mySprite;
    sf::RectangleShape myOuterLeftShape;
    sf::RectangleShape myLeftShape;
    sf::RectangleShape myCenterShape;
    sf::RectangleShape myRightShape;
    sf::RectangleShape myOuterRightShape;
    float              myState;
};

#endif
#define PWUSEGLOBALSINGLETONS
#include "..\..\include\Game\CLoad.h"

CLoad::CLoad() : pwGameState(ID_GS_LOAD)
{
}

CLoad::~CLoad() 
{
}

bool CLoad::Start()
{
    mySprite.setTexture(*pwDataBox::get()->GetTexture("menu_main_bg01.png"));
    mySprite.setOrigin(sf::Vector2f((float)mySprite.getTexture()->getSize().x / 2.0f, (float)mySprite.getTexture()->getSize().y / 2.0f));
    mySprite.setPosition((float)pwEngineOptions::GetInternGraphicsSize().x / 2.0f, (float)pwEngineOptions::GetInternGraphicsSize().y / 2.0f);

    myState = 0.0f;
       
    myOuterLeftShape.setSize      (sf::Vector2f(10.0f, 10.0f));
    myLeftShape.setSize           (sf::Vector2f(10.0f, 10.0f));
    myCenterShape.setSize         (sf::Vector2f(10.0f, 10.0f));
    myRightShape.setSize          (sf::Vector2f(10.0f, 10.0f));
    myOuterRightShape.setSize     (sf::Vector2f(10.0f, 10.0f));
    myOuterLeftShape.setFillColor (sf::Color(255, 255, 255));
    myLeftShape.setFillColor      (sf::Color(255, 255, 255));
    myCenterShape.setFillColor    (sf::Color(255, 255, 255));
    myRightShape.setFillColor     (sf::Color(255, 255, 255));
    myOuterRightShape.setFillColor(sf::Color(255, 255, 255));
    myOuterLeftShape.setOrigin    (5.0f, 5.0f);
    myLeftShape.setOrigin         (5.0f, 5.0f);
    myCenterShape.setOrigin       (5.0f, 5.0f);
    myRightShape.setOrigin        (5.0f, 5.0f);
    myOuterRightShape.setOrigin   (5.0f, 5.0f);
    myOuterLeftShape.setPosition  (mySprite.getPosition().x - 40.0f, (float)pwEngineOptions::GetInternGraphicsSize().y - 100.0f);
    myLeftShape.setPosition       (mySprite.getPosition().x - 20.0f, (float)pwEngineOptions::GetInternGraphicsSize().y - 100.0f);
    myCenterShape.setPosition     (mySprite.getPosition().x, (float)pwEngineOptions::GetInternGraphicsSize().y - 100.0f);
    myRightShape.setPosition      (mySprite.getPosition().x + 20.0f, (float)pwEngineOptions::GetInternGraphicsSize().y - 100.0f);
    myOuterRightShape.setPosition (mySprite.getPosition().x + 40.0f, (float)pwEngineOptions::GetInternGraphicsSize().y - 100.0f);

    return true;
}


unsigned int CLoad::Run()
{
    //pwGraphics::get()->GetRenderWindow().clear();
    mySprite.rotate(-3.0f * pwGraphics::get()->GetLastFrameTime()); 

    pwGraphics::get()->Draw(&mySprite);

    if (myState >= 1.0f)
        myState = 0.0f;

    myOuterLeftShape.setScale (1.0f, 1.0f);
    myLeftShape.setScale      (1.0f, 1.0f);
    myCenterShape.setScale    (1.0f, 1.0f);
    myRightShape.setScale     (1.0f, 1.0f);
    myOuterRightShape.setScale(1.0f, 1.0f);

    if (myState < 0.1f)
        myOuterLeftShape.setScale(2.0f, 2.0f);
    else if (myState < 0.2f)
        myLeftShape.setScale(2.0f, 2.0f);
    else if (myState < 0.3f)
        myCenterShape.setScale(2.0f, 2.0f);
    else if (myState < 0.4f)
        myRightShape.setScale(2.0f, 2.0f);
    else if (myState < 0.5f)
        myOuterRightShape.setScale(2.0f, 2.0f);

    pwGraphics::get()->Draw(&myOuterLeftShape);
    pwGraphics::get()->Draw(&myLeftShape);
    pwGraphics::get()->Draw(&myCenterShape);
    pwGraphics::get()->Draw(&myRightShape);
    pwGraphics::get()->Draw(&myOuterRightShape);
        
    myState += pwGraphics::get()->GetLastFrameTime();

	return 0;
}
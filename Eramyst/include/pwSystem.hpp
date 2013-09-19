#ifndef PWSYSTEM
#define PWSYSTEM

#include <string>
#include <map>
#include "pwHelper.hpp"
#include "pwGameState.hpp"
#include "pwGraphicsSystem.hpp"
#include "pwSoundSystem.hpp"
#include "pwGameLogics.hpp"
#include "pwEngineOptions.hpp"
#include "pwLogSystem.hpp"
#include "pwGraphicsLog.hpp"
#include "pwAnimation.hpp"
#include "pwDataBox.hpp"
#include "pwParticleSystem.hpp"
#include "pwScript.hpp"
#include "pwGUI.hpp"
#include "pwStreamAnimation.hpp"

#ifdef PWUSEGLOBALSINGLETONS
#define glSystem pwSystem::get()
#define glLog pwLog::get()
#define glGLog (*pwGraphicsLog::get())
#define p_glGLog pwGraphicsLog::get()
#define glGraphics pwGraphics::get()
#define glDataBox pwDataBox::get()
#define glLogics pwGameLogics::get()
#define glAudio pwAudio::get()
#endif

class pwLoadGameState : public pwGameState 
{
public:
	pwLoadGameState()
	{
        mySprite.setTexture(*pwDataBox::get()->GetTexture("default.png"));
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
    }

	~pwLoadGameState() {}

	unsigned int Run()
	{
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

private:
    sf::Sprite mySprite;
    sf::RectangleShape myOuterLeftShape;
    sf::RectangleShape myLeftShape;
    sf::RectangleShape myCenterShape;
    sf::RectangleShape myRightShape;
    sf::RectangleShape myOuterRightShape;
    float              myState;
};

struct pwLoadData 
{
    std::string ListName;
    std::string FileName;
    std::string ClassName;
    bool Loaded;
    bool Return;
};

////////////////////////////////////////////////////////////////
// Manages the subsystems and the registered game states
////////////////////////////////////////////////////////////////
class pwSystem
{

public:

    ////////////////////////////////////////////////////////////////
    // Default constructor
    ////////////////////////////////////////////////////////////////
	pwSystem();

    ////////////////////////////////////////////////////////////////
    // Destructor
    ////////////////////////////////////////////////////////////////
	~pwSystem();
	
    ////////////////////////////////////////////////////////////////
    // Initialize the subsystems
    //
    // \param WindowTitle: String, that is shown in the title-bar of the window (default: "pwEngine")
    //
    // \return True, if initialisation was successful, false otherwise
    ////////////////////////////////////////////////////////////////
    bool Initialize(std::string WindowTitle = "pwEngine");

    ////////////////////////////////////////////////////////////////
    // Shuts the subsystems down
    //
    // \return True, if shutdown was successful, false otherwise
    ////////////////////////////////////////////////////////////////
	bool Shutdown();

    ////////////////////////////////////////////////////////////////
    // Get the static instance of the engine
    //
    // \return Static instance of the engine
    ////////////////////////////////////////////////////////////////
    static pwSystem* get();

    ////////////////////////////////////////////////////////////////
    // Register a game-state by passing a pwGameState-class pointer
    //
    // \param GameState: Pointer to the game-state to register
    //
    // \return True, if registration was successful, false otherwise
    ////////////////////////////////////////////////////////////////
    bool RegisterGameState(pwGameState *GameState);

    ////////////////////////////////////////////////////////////////
    // Register a game-state by passing a pwScript-class instance
    //
    // \param Script: Name of the Script-File
    //
    // \return True, if registration was successful, false otherwise
    ////////////////////////////////////////////////////////////////
    bool RegisterGameState(std::string Script);

    ////////////////////////////////////////////////////////////////
    // Sets the active GameState
    //
    // \param GameState: Name of the game-state, that is to be set as active
    //
    // \return True, if changing the game-state was successful, false otherwise
    ////////////////////////////////////////////////////////////////
    bool SetActiveGameState(unsigned int GameStateID);

    ////////////////////////////////////////////////////////////////
    // Starts running the main loop
    //
    // \param StartGameState: Name of the game-state, the engine is started with
    //
    // \return True, if an error occured while running the engine, false otherwise
    ////////////////////////////////////////////////////////////////
    bool RunEngine(unsigned int StartGameStateID);

    bool HasFocus() {return myHasFocus;}

    void Terminate()    {myRunning = false;}

    const pwHelper::split_data<bool, sf::Uint32>& GetEnteredText() {return myEnteredText;}
    const pwHelper::split_data<bool, std::list<sf::Keyboard::Key>>& GetKeyPressed() {return myEnteredKeys;}

    bool LoadDataList(const std::string& ListName, const std::string& FileName, const std::string& ClassName = "default", unsigned int GameStateID = 0);
    static void LoadData(pwLoadData* LoadData);

private:

    ////////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////////

	static pwSystem*                    p_inst;
    std::map<unsigned int, pwGameState*> myGameStateMap;
    std::string                         myTitle;
    unsigned int                        myCurrentGameState;
    unsigned int                        myNextGameState;
    bool                                myHasFocus;
    bool                                myRunning;

    pwHelper::split_data<bool, sf::Uint32> myEnteredText;
    pwHelper::split_data<bool, std::list<sf::Keyboard::Key>> myEnteredKeys;

    pwLoadGameState                     myLoadGameState;
};

#endif
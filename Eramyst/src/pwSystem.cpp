#include "..\include\pwSystem.hpp"

////////////////////////////////////////////////////////////////
// Declaration of the static member variable
////////////////////////////////////////////////////////////////
pwSystem* pwSystem::p_inst = NULL;

////////////////////////////////////////////////////////////////
// Default constructor
////////////////////////////////////////////////////////////////
pwSystem::pwSystem() : 
    myTitle("pwEngine"),
    myCurrentGameState(0),
    myNextGameState(0),
    myHasFocus(true),
    myRunning(false)

{
    // Nothing to do
}

////////////////////////////////////////////////////////////////
// Destructor
////////////////////////////////////////////////////////////////
pwSystem::~pwSystem()
{
    // Nothing to do
}

////////////////////////////////////////////////////////////////
// Initialize the subsystems
////////////////////////////////////////////////////////////////
bool pwSystem::Initialize(std::string WindowTitle)
{
    if (!pwLog::get()->Initialize())
    {
	    return false;
    }
    if (!pwEngineOptions::Initialize())
    {
        pwLog::get()->WriteLine("pwSystem::Initialize: Options could not be initialized");
        return false;
    }
    pwEngineOptions::SetWindowTitle(WindowTitle);

    if (!pwGraphics::get()->Initialize())
    {
        pwLog::get()->WriteLine("pwSystem::Initialize: Graphics-System could not be initialized");
        return false;
    }
    if (!pwAudio::get()->Initialize())
    {
        pwLog::get()->WriteLine("pwSystem::Initialize: Sound-System could not be initialized");
        return false;
    }
    if (!pwGameLogics::get()->Initialize())
    {
        pwLog::get()->WriteLine("pwSystem::Initialize: Logics-System could not be initialized");
        return false;
    }

    return true;
}

////////////////////////////////////////////////////////////////
// Shuts the subsystems down
////////////////////////////////////////////////////////////////
bool pwSystem::Shutdown()
{
    pwGameLogics::get()->Shutdown();
    pwGraphics::get()->Shutdown();
    pwAudio::get()->Shutdown();
    pwEngineOptions::Shutdown();
    pwLog::get()->Shutdown();

    // If the engine exists, delete it
	if (p_inst)
		delete p_inst;
	p_inst = NULL;

    // Succeeded
    return true;
}

////////////////////////////////////////////////////////////////
// Get the static instance of the engine
////////////////////////////////////////////////////////////////
pwSystem* pwSystem::get()
{
    // If the engine doesn't exist, create it
    if (!p_inst)
		p_inst = new pwSystem;

    // Return the pointer to the engine
	return p_inst;
}

////////////////////////////////////////////////////////////////
// Register a game-state by passing a pwGameState-class pointer
////////////////////////////////////////////////////////////////
bool pwSystem::RegisterGameState(pwGameState *GameState)
{
    // TODO: Catch predefined game-states [e.g. exit]
    std::pair<unsigned int, pwGameState*> GameStatePair;

    // Fill pair-data
    GameStatePair.first = GameState->GetID();
    GameStatePair.second = GameState;

    // Try to insert the game-state in the map
    std::pair<std::map<unsigned int, pwGameState*>::iterator, bool> pr = myGameStateMap.insert(GameStatePair);

    if (pr.second == true)
    {
        // No error occured: Return true
        return true;
    }
    else
    {
        // There is already a game-state with this name
        return false;
    }
}

////////////////////////////////////////////////////////////////
// Register a game-state by passing a pwScript-class instance
////////////////////////////////////////////////////////////////
bool pwSystem::RegisterGameState(std::string Script)
{
    return false;
}

////////////////////////////////////////////////////////////////
// Sets the active GameState
////////////////////////////////////////////////////////////////
bool pwSystem::SetActiveGameState(unsigned int GameStateID)
{
    if (GameStateID == 0)
        return false;

    // Does a game-state with this name actually exist?
    std::map<unsigned int, pwGameState*>::iterator  GameStateMapIterator = myGameStateMap.find(GameStateID);

    if (GameStateMapIterator != myGameStateMap.end())
    {
        // Game-State exists: Set it as next
        myNextGameState = GameStateID;

        // No error occured: Return true
        return true;
    }
    else
    {
        // A game-state with this name doesn't exist
        return false;
    }
}

////////////////////////////////////////////////////////////////
// Starts running the main loop
////////////////////////////////////////////////////////////////
bool pwSystem::RunEngine(unsigned int StartGameStateID)
{
    // Sets the current game-state to StartGameState 
    if (!SetActiveGameState(StartGameStateID))
    {
        // Error: Propably this game-state doesn't exist
        pwLog::get()->WriteLine("pwSystem::RunEngine: A game-state with the name, to start with could not be found");
        return false;
    }

    myRunning = true;

    // Loop is running until the game is closed
    while(myRunning)
    {
        myEnteredText.first = false;
        myEnteredText.second = 0;
        myEnteredKeys.first = false;
        myEnteredKeys.second.clear();

        // TODO: Get input
        /////////////////////////////////////////////////////////////////////////////////
        // TODO: This is to be put somewhere else
        // Process events
        sf::Event Event;
        while (pwGraphics::get()->GetRenderWindow().pollEvent(Event))
        {
            // Close window : exit
            if (Event.type == sf::Event::Closed) /* | (Event.type == sf::Event::KeyPressed && Event.key.code == sf::Keyboard::Escape)*/
			{
				myRunning = false;
			}
            else if (Event.type == sf::Event::LostFocus)
            {
                myHasFocus = false;
                pwAudio::get()->Pause();
                pwGameLogics::get()->SetPaused(true);
            }
            else if (Event.type == sf::Event::GainedFocus)
            {
                myHasFocus = true;
                pwAudio::get()->Resume();
                pwGameLogics::get()->SetPaused(false);
            }
            else if (Event.type == sf::Event::TextEntered)
            {
                myEnteredText.first = true;
                myEnteredText.second = Event.text.unicode;
            }
            else if (Event.type == sf::Event::KeyPressed)
            {
                myEnteredKeys.first = true;
                myEnteredKeys.second.push_back(Event.key.code);
            }
        }

        /////////////////////////////////////////////////////////////////////////////////

        // Update game-logics
        if (myCurrentGameState != 0)
            pwGameLogics::get()->Update(myCurrentGameState);

        if (myHasFocus)
        {
            // myNextGameState was set [and doesn't equal myCurrentGameState]
            if (myCurrentGameState != 0 && myNextGameState != 0 && 
                myCurrentGameState != myNextGameState)
            {
                // myCurrentGameState is closed
                if (myGameStateMap.at(myCurrentGameState)->Close())
                {
                    // Set myCurrentGameState to 0, if return of Close() is true
                    myCurrentGameState = 0;
                    pwGraphics::get()->ResetFrameTimer();
                }
            }

            // myCurrentGameState was set to 0 and the game is not to close
            if (myCurrentGameState == 0 && myNextGameState != 0)
            {
                // myNextGameState is started
                if (myGameStateMap.at(myNextGameState)->Start())
                {
                    // Set to current, if return of Start() is true
                    myCurrentGameState = myNextGameState;
                    pwGraphics::get()->ResetFrameTimer();
                }
            }

            // myNextGameState is equal to myCurrentGameState -> game-state is running
            if (myNextGameState == myCurrentGameState)
            {
                // The next game-state is set as the current one is running
                // To continue running this game-state, Run() has to return its own name
                if (!SetActiveGameState(myGameStateMap.at(myCurrentGameState)->Run()))
                {
                    // Error: Propably the next game-state doesn't exist
                    return false;
                }
            }
            
            if (myNextGameState == 0)
            {
                // TODO: Close the game
                return true;
            }

            pwGraphics::get()->DrawOverlay(pwGraphicsLog::get(), INT_MAX);
        }
        else 
        {
            pwGraphics::get()->SetPaused(true);
        }

        pwGraphicsLog::get()->update();
        pwStreamAnimationLoader::get()->CleanUp();
        
        pwAudio::get()->Update();
        // Render all listed objects and update the graphics
        pwGraphics::get()->Update();
    }

    return true;
}

bool pwSystem::LoadDataList(const std::string& ListName, const std::string& FileName, const std::string& ClassName, unsigned int GameStateID)
{
    pwLoadData LoadData;
    LoadData.ListName = ListName;
    LoadData.FileName = FileName;
    LoadData.ClassName = ClassName;
    LoadData.Loaded = false;
    LoadData.Return = false;
    pwGameState* LoadGameState = NULL;

    if (GameStateID != 0)
    {
        // Does a game-state with this id actually exist?
        std::map<unsigned int, pwGameState*>::iterator GameStateMapIterator = myGameStateMap.find(GameStateID);

        if (GameStateMapIterator != myGameStateMap.end())
        {
            // Game-State exists: Set it as next
            LoadGameState = GameStateMapIterator->second;
        }
    }

    sf::Thread Load(pwSystem::LoadData, &LoadData);
    Load.launch();

    if (LoadGameState == NULL)
        myLoadGameState.Start();
    else
        LoadGameState->Start();

    while (!LoadData.Loaded)
    {
        sf::Event Event;
        while (pwGraphics::get()->GetRenderWindow().pollEvent(Event))
        {
            // Close window : exit
            if (Event.type == sf::Event::Closed)
			{
                Load.terminate();
                Terminate();
                return false;
            }
            else if (Event.type == sf::Event::LostFocus)
            {
                myHasFocus = false;
            }
            else if (Event.type == sf::Event::GainedFocus)
            {
                myHasFocus = true;
            }
        }

        if (LoadGameState == NULL)
            myLoadGameState.Run();
        else
            LoadGameState->Run();

        pwAudio::get()->Update();
        // Render all listed objects and update the graphics
        pwGraphics::get()->Update();
    }

    if (LoadGameState == NULL)
        myLoadGameState.Close();
    else
        LoadGameState->Close();

    return LoadData.Return;
}

void pwSystem::LoadData(pwLoadData* LoadData)
{
    LoadData->Return = pwDataBox::get()->LoadDataList(LoadData->ListName, LoadData->FileName, LoadData->ClassName);
    LoadData->Loaded = true;
}
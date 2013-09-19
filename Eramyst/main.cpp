#define PWUSEGLOBALSINGLETONS

#include "include\pwSystem.hpp"
#include "include\Game\ID.hpp"

// Game

#include "include\Game\CMenuMain.h"
#include "include\Game\CMenuLevel.h"
#include "include\Game\CMenuOptions.h"
#include "include\Game\CMenuCredits.h"
#include "include\Game\CLoad.h"
#include "include\Game\CPlay.h"
#include "include\Game\CSettings.h"
#include "include\Game\CLevelData.h"
#include "include\Game\CTrailer.h"
#include "include\Game\CInstant.h"
#include "include\Game\CHighscore.h"
#include "include\Game\CIngameMenu.h"

// Debug
#include <iostream>

int main()
{
	// Create game-states, inherited from pwGameState
    CInstant InstantLogo;
    CLoad LoadGameState;
	CPlay PlayGameState;
    CMenuMain MenuMain;
    CMenuLevel MenuLevel;
    CMenuOptions MenuOptions;
    CMenuCredits MenuCredits;
    CLevelData LevelData;
    CTrailer Trailer;
    CHighscore Highscore;
    CIngame Ingame;

    pwEngineOptions::SetInternGraphicsSize(sf::Vector2u(1920, 1080));

    // Initialize the pwEngine
    glSystem->Initialize();

    // Limit frame-rate
    //glGraphics->GetRenderWindow().setFramerateLimit(60);

    // Register the game-states
    glSystem->RegisterGameState(&InstantLogo);
    glSystem->RegisterGameState(&LoadGameState);
    glSystem->RegisterGameState(&PlayGameState);
    glSystem->RegisterGameState(&MenuMain);
    glSystem->RegisterGameState(&MenuLevel);
    glSystem->RegisterGameState(&MenuOptions);
    glSystem->RegisterGameState(&MenuCredits);
    glSystem->RegisterGameState(&LevelData);
    glSystem->RegisterGameState(&Trailer);
    glSystem->RegisterGameState(&Highscore);
    glSystem->RegisterGameState(&Ingame);

    glGraphics->SetClearColor(sf::Color(29, 29, 29));

    // Load Settings
    CSettings::get()->init();

    // Start with LoadGameState
    glSystem->RunEngine(InstantLogo.GetID());

    // Save Settings
    CSettings::get()->save();

    // When the game-state-loop is left, shut the engine down
    glSystem->Shutdown();

    // Nothing can go wrong
    return EXIT_SUCCESS;
}

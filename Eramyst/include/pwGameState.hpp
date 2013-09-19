#ifndef PWGAMESTATE
#define PWGAMESTATE

#include <string>
#include "pwGameObject.hpp"

class pwGameObject;

////////////////////////////////////////////////////////////////
// A game state, that is registered at and run by the engine
////////////////////////////////////////////////////////////////
class pwGameState
{

public:

    ////////////////////////////////////////////////////////////////
    // Default constructor
    ////////////////////////////////////////////////////////////////
    pwGameState();

    ////////////////////////////////////////////////////////////////
    // Constructor overload to set myName
    //
    // \param GameStateName: ID, the game-state should be linked with
    ////////////////////////////////////////////////////////////////
    pwGameState(unsigned int GameStateID);

    ////////////////////////////////////////////////////////////////
    // Virtual destructor
    ////////////////////////////////////////////////////////////////
    virtual ~pwGameState();

    ////////////////////////////////////////////////////////////////
    // Starts the game-state
    //
    // \return True, if start-up was done, false otherwise
    ////////////////////////////////////////////////////////////////
    virtual bool Start();

    ////////////////////////////////////////////////////////////////
    // Runs the game-state
    //
    // \return ID of the next game-state to run
    ////////////////////////////////////////////////////////////////
    virtual unsigned int Run();

    ////////////////////////////////////////////////////////////////
    // Closes the game-state
    //
    // \return True, if closure was done, false otherwise
    ////////////////////////////////////////////////////////////////
    virtual bool Close();

    ////////////////////////////////////////////////////////////////
    // Renders the current game-state
    ////////////////////////////////////////////////////////////////
    virtual void Render();

    ////////////////////////////////////////////////////////////////
    // Is called, if two game-objects from this game-state collide
    //
    // \param Object1: First object, that collides
    // \param CollisionObject1ID: ID of the first collision object, that collided
    // \param Object2: Second object, that collides
    // \param CollisionObject2ID: ID of the second collision object, that collided
    ////////////////////////////////////////////////////////////////
    virtual void OnCollision(pwGameObject* Object1, unsigned int CollisionObject1ID, pwGameObject* CollisionObject2, unsigned int CollisionObject2ID);

    ////////////////////////////////////////////////////////////////
    // Returns the name of the game-state
    //
    // \return Name of the game-state
    ////////////////////////////////////////////////////////////////
    unsigned int GetID() const;


protected:

    ////////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////////
    unsigned int myID;
};

#endif
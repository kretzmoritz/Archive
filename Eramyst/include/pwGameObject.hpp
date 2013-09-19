#ifndef PWGAMEOBJECT
#define PWGAMEOBJECT

#include <map>
#include <SFML\Graphics.hpp>
#include <SFML\Audio.hpp>
#include "pwLogSystem.hpp"
#include "pwGameLogics.hpp"
#include "pwSoundSystem.hpp"

class pwCollisionObject;

////////////////////////////////////////////////////////////////
// The top-class for all the objects in a game
////////////////////////////////////////////////////////////////
class pwGameObject : public sf::Drawable, public sf::Transformable
{

public:

    ////////////////////////////////////////////////////////////
    // Default constructor
    ////////////////////////////////////////////////////////////
    pwGameObject();

    ////////////////////////////////////////////////////////////////
    // Constructor overload to set myID
    //
    // \param GameObjectID: ID, the game-object should be linked with
    ////////////////////////////////////////////////////////////////
    pwGameObject(unsigned int GameObjectID);

    ////////////////////////////////////////////////////////////
    // Destructor
    ////////////////////////////////////////////////////////////
    ~pwGameObject();

    ////////////////////////////////////////////////////////////
    // Add a drawable-object to the pwGameObject
    // TODO: pointer validation / tracing
    //
    // \param ObjectID: The id, under which the object is to be added to the map
    // \param DrawableObject: Pointer to the drawable-object to add to the pwGameObject
    //
    // \return True, if adding the object was succesfull, false otherwise
    ////////////////////////////////////////////////////////////
    bool AddDrawableObject(unsigned int ObjectID, sf::Drawable* DrawableObject);

    ////////////////////////////////////////////////////////////
    // Add a collision-object to the pwGameObject
    // TODO: pointer validation / tracing
    //
    // \param ObjectID: The id, under which the object is to be added to the map
    // \param CollisionObject: Pointer to the collision-object to add to the pwGameObject
    //
    // \return True, if adding the object was succesfull, false otherwise
    ////////////////////////////////////////////////////////////
    bool AddCollisionObject(unsigned int ObjectID, pwCollisionObject* CollisionObject);

    ////////////////////////////////////////////////////////////
    // Is called, if another object collides with this one
    //
    // \param CollisionObject: Constant reference to the pwGameObject, that collided with this one
    // \param thisCollisionObjectName: Name of the own collision-object, which collided
    // \param CollisionObjectName: Name of the collision-object of the pwGameObject that collided
    ////////////////////////////////////////////////////////////
    virtual void OnCollision(const pwGameObject* GameObject, unsigned int thisCollisionObjectID, unsigned int CollisionObjectID);
    
    ////////////////////////////////////////////////////////////
    // Is called, if another object collides with this one the first time
    //
    // \param CollisionObject: Constant reference to the pwGameObject, that collided with this one
    // \param thisCollisionObjectName: Name of the own collision-object, which collided
    // \param CollisionObjectName: Name of the collision-object of the pwGameObject that collided
    ////////////////////////////////////////////////////////////
    virtual void EnterCollision(const pwGameObject* GameObject, unsigned int thisCollisionObjectID, unsigned int CollisionObjectID);

    ////////////////////////////////////////////////////////////
    // Is called, if another object collided last frame with this one for the last time
    ////////////////////////////////////////////////////////////
    virtual void LeftCollision();

    ////////////////////////////////////////////////////////////
    // Updates the position of the object
    //
    // \param FrameTime: Time, elapsed since the last frame in seconds
    ////////////////////////////////////////////////////////////
    void Update(float FrameTime);

    ////////////////////////////////////////////////////////////
    // Custom update-function, that is called every frame
    //
    // \param FrameTime: Time, elapsed since the last frame in seconds
    ////////////////////////////////////////////////////////////
    virtual void OnUpdate(float FrameTime);

    ////////////////////////////////////////////////////////////
    // Set the force-vector of the pwGameObject (in: mass * pixel / sqr(second))
    //
    // \param Force: Constant reference to the force-vector
    ////////////////////////////////////////////////////////////
    void ApplyForce(const sf::Vector2f &Force);
    void ApplyForce(float xForce, float yForce);

    ////////////////////////////////////////////////////////////
    // Set the gravity multiplier 
    // - to switch gravity off, use SetGravityMultiplier(0.0f)
    // 
    // \param g: new gravity multiplier (default: 0.0f)
    ////////////////////////////////////////////////////////////
    void SetGravityMultiplier(float g = 1.0f)                                   {myGravityMultiplier = g;}
    float GetGravityMultiplier() const                                          {return myGravityMultiplier;}

    ////////////////////////////////////////////////////////////
    // Set the object mass
    // 
    // \param g: new object mass (default: 1.0f)
    ////////////////////////////////////////////////////////////
    void SetMass(float m = 1.0f);
    float GetMass() const                                                       {return myMass;}

    ////////////////////////////////////////////////////////////
    // Set the velocity-vector of the pwGameObject
    //
    // \param Velocity: Constant reference to the velocity-vector [Values are in PixelsPerSecond]
    ////////////////////////////////////////////////////////////
    void SetStartVelocity(const sf::Vector2f &Velocity)                         {myVelocity = Velocity;}
    void SetStartVelocity(float xVelocity, float yVelocity)                     {myVelocity.x = xVelocity; myVelocity.y = yVelocity;}
    const sf::Vector2f& GetStartVelocity() const                                {return myVelocity;}

    ////////////////////////////////////////////////////////////
    // Set the acceleration-vector of the pwGameObject
    //
    // \param Velocity: Constant reference to the acceleration-vector [Values are in PixelsPerSquareSeconds]
    ////////////////////////////////////////////////////////////
    void SetStartAcceleration(const sf::Vector2f &Acceleration)                 {myAcceleration = Acceleration;}
    void SetStartAcceleration(float xAcceleration, float yAcceleration)         {myAcceleration.x = xAcceleration; myAcceleration.y = yAcceleration;}
    const sf::Vector2f& GetStartAcceleration() const                            {return myAcceleration;}

    ////////////////////////////////////////////////////////////
    // Set the color of the object.
    // The default color is white
    //
    // \param Col: New color
    ////////////////////////////////////////////////////////////
    void SetColor(const sf::Color& Col);
    void SetColor(sf::Uint8 r, sf::Uint8 g, sf::Uint8 b, sf::Uint8 a);

    ////////////////////////////////////////////////////////////
    // Get a pointer to the collision-object-map
    //
    // \return Pointer to the collision-object-map
    ////////////////////////////////////////////////////////////
    std::list<std::pair<unsigned int, pwCollisionObject*>>* GetCollisionObjectMap()          {updateCollisionObjects(); return &myCollisionMap;}

    ////////////////////////////////////////////////////////////
    // Get a pointer to the drawable-object-map
    //
    // \return Pointer to the drawable-object-map
    ////////////////////////////////////////////////////////////
    const std::list<std::pair<unsigned int, sf::Drawable*>>* GetDrawableObjectMap() const    {return &myDrawableMap;}

    ////////////////////////////////////////////////////////////
    // Get the color of the object
    //
    // \return Current color
    ////////////////////////////////////////////////////////////
    const sf::Color& GetColor() const;

    ////////////////////////////////////////////////////////////
    // Get the Object Collide last frame
    //
    // \return True, if they collided
    ////////////////////////////////////////////////////////////
    bool GetCollision() const                                                   {return myCollision;}

    ////////////////////////////////////////////////////////////
    // Is the pwGameObject still alive?
    //
    // \return True, if it's alive
    ////////////////////////////////////////////////////////////
    bool GetAlive() const                                                       {return myAlive;}
    
    ////////////////////////////////////////////////////////////
    // Kill the Object
    ////////////////////////////////////////////////////////////
    void Kill();

    ////////////////////////////////////////////////////////////
    // Did the Object Collide last frame
    //
    // \param Collision: True, if they collided
    ////////////////////////////////////////////////////////////
    void SetCollision(bool Collision)                                           {myCollision = Collision;}

    ////////////////////////////////////////////////////////////
    // Are the CollisionObjects to be rendered?
    //
    // \param RenderCollisionObjects: True, if pwCollisionObjects are to be rendered
    ////////////////////////////////////////////////////////////
    void SetRenderCollisionObjects(bool RenderCollisionObjects = false)         {myRenderCollisionObjects = RenderCollisionObjects;}

    ////////////////////////////////////////////////////////////////
    // Returns the name of the game-object
    //
    // \return Name of the game-object
    ////////////////////////////////////////////////////////////////
    unsigned int GetID() const                                                  {return myID;}

    ////////////////////////////////////////////////////////////////
    // Sets the name of the game-object
    //
    // \param GameObjectName: Name of the game-object
    ////////////////////////////////////////////////////////////////
    void SetID(unsigned int GameObjectID)                                       {myID = GameObjectID;}

    ////////////////////////////////////////////////////////////////
    // Plays a sound at the current position
    ////////////////////////////////////////////////////////////////
    virtual void PlaySound(sf::SoundBuffer* Buffer, unsigned int Layer = 0);

    void updateCollisionObjects();

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

protected:

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    sf::Color    myColor;
    std::list<std::pair<unsigned int, pwCollisionObject*>> myCollisionMap;
    std::list<std::pair<unsigned int, sf::Drawable*>>      myDrawableMap;
    bool         myCollision;
    bool         myRenderCollisionObjects;
    unsigned int myID;

    float        myGravityMultiplier;
    float        myMass;
    float        myInvMass;
    sf::Vector2f myVelocity;
    sf::Vector2f myAcceleration;
    sf::Vector2f myForce;
    bool         myAlive;
};

#endif
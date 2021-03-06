#ifndef PWGAMELOGICS
#define PWGAMELOGICS

#ifndef PWSYSTEM
#ifdef PWUSEGLOBALSINGLETONS
#define glLogics pwGameLogics::get()
#endif
#endif

#include <list>
#include <SFML\Graphics.hpp>
#include "pwHelper.hpp"
#include "pwGameState.hpp"
#include "pwGameObject.hpp"
#include "pwGraphicsSystem.hpp"

class pwGameObject;
class pwGameState;

////////////////////////////////////////////////////////////////
// Base-class of the different types of collision-objects
////////////////////////////////////////////////////////////////
class pwCollisionObject : public sf::Transformable, public sf::Drawable
{

public:

    ////////////////////////////////////////////////////////////////
    // Default constructor
    ////////////////////////////////////////////////////////////////
    pwCollisionObject();

    ////////////////////////////////////////////////////////////////
    // Destructor
    ////////////////////////////////////////////////////////////////
    ~pwCollisionObject();

    ////////////////////////////////////////////////////////////////
    // Collides the Object with the given pwCollisionObject?
    //
    // \param CollisionObject: Object that is to be tested on collision
    //
    // \return True, if the pwCollisionObjects collide, false otherwise
    ////////////////////////////////////////////////////////////////
    virtual bool Collides(const pwCollisionObject* CollisionObject) const = 0;

    ////////////////////////////////////////////////////////////////
    // Set the transformation of the pwCollisionObject
    //
    // \param Constant reference to a sf::Transform-Object
    ////////////////////////////////////////////////////////////////
    void SetAbsTransformable(const sf::Transformable& Transformable)    {myAbsTransformable = Transformable;}

    ////////////////////////////////////////////////////////////////
    // Set the transformation of the pwCollisionObject
    //
    // \param Constant reference to a sf::Transform-Object
    ////////////////////////////////////////////////////////////////
    const sf::Transformable& GetAbsTransformable() const            {return myAbsTransformable;}

    ////////////////////////////////////////////////////////////////
    // Set the color of the pwCollisionObject
    //
    // \param Constant reference to a sf::Color-Object
    ////////////////////////////////////////////////////////////////
    virtual void SetColor(const sf::Color& col) {}

    ////////////////////////////////////////////////////////////////
    // Is this pwCollisionObject a rect or a circleS
    //
    // \return True, if the Object is a CollisionRect, false otherswise (if it's a CollisionCircle) 
    ////////////////////////////////////////////////////////////////
    virtual const bool IsRect() const = 0;

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {};

protected: 

    ////////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////////
    sf::Transformable myAbsTransformable;
};

////////////////////////////////////////////////////////////////
// Declaration of pwCollisionRect, as it is used in pwCollisionShere
////////////////////////////////////////////////////////////////
class pwCollisionRect;

////////////////////////////////////////////////////////////////
// A circle to test on collision
////////////////////////////////////////////////////////////////
class pwCollisionCircle : public pwCollisionObject
{

public:

    ////////////////////////////////////////////////////////////////
    // Default constructor
    ////////////////////////////////////////////////////////////////
    pwCollisionCircle();

    ////////////////////////////////////////////////////////////////
    // Destructor
    ////////////////////////////////////////////////////////////////
    ~pwCollisionCircle();

    ////////////////////////////////////////////////////////////////
    // Collides the Object with the given pwCollisionObject?
    //
    // \param CollisionObject: Object that is to be tested on collision
    //
    // \return True, if the pwCollisionObjects collide, false otherwise
    ////////////////////////////////////////////////////////////////
    bool Collides(const pwCollisionObject* CollisionObject) const;

    ////////////////////////////////////////////////////////////////
    // Collides the Object with the given pwCollisionCircle?
    //
    // \param CollisionCircle: Circle, that is to be tested on collision
    //
    // \return True, if the pwCollisionObjects collide, false otherwise
    ////////////////////////////////////////////////////////////////
    bool Collides(const pwCollisionCircle* CollisionCircle) const;

    ////////////////////////////////////////////////////////////////
    // Collides the Object with the given pwCollisionRect?
    //
    // \param Rect, that is to be tested on collision
    //
    // \return True, if the pwCollisionObjects collide, false otherwise
    ////////////////////////////////////////////////////////////////
    bool Collides(const pwCollisionRect* CollisionRect) const;

    ////////////////////////////////////////////////////////////////
    // Collides the pwCollisionCircle with the given line segment?
    // c:      Center of the circle
    // a:      First point of the line segment
    // b:      Second point of the line segment
    ////////////////////////////////////////////////////////////////
    inline bool CollidesWithLineSegment(const sf::Vector2f& c, const sf::Vector2f& a, const sf::Vector2f& b) const;

    ////////////////////////////////////////////////////////////////
    // Set a the radius of the circle
    ////////////////////////////////////////////////////////////////
    void SetRadius(float Radius);

    ////////////////////////////////////////////////////////////////
    // Get a the radius of the circle
    //
    // \return Radius of the circle
    ////////////////////////////////////////////////////////////////
    float GetRadius() const {return myRadius;}

    ////////////////////////////////////////////////////////////////
    // Set the color of the pwCollisionCircle
    //
    // \param Constant reference to a sf::Color-Object
    ////////////////////////////////////////////////////////////////
    void SetColor(const sf::Color& col)   { myShape.setFillColor(col);}

    ////////////////////////////////////////////////////////////////
    // Is this pwCollisionObject a rect or a circle
    //
    // \return See pwCollisionObject::IsRect()
    ////////////////////////////////////////////////////////////////
    const bool IsRect() const {return false;}

    void draw(sf::RenderTarget& target, sf::RenderStates states) const;


private:

    ////////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////////
    float myRadius;
    mutable sf::CircleShape myShape;
};

////////////////////////////////////////////////////////////////
// A rect to test on collision
////////////////////////////////////////////////////////////////
class pwCollisionRect : public pwCollisionObject
{

public:

    ////////////////////////////////////////////////////////////////
    // Default constructor
    ////////////////////////////////////////////////////////////////
    pwCollisionRect();

    ////////////////////////////////////////////////////////////////
    // Destructor
    ////////////////////////////////////////////////////////////////
    ~pwCollisionRect();

    ////////////////////////////////////////////////////////////////
    // Collides the Object with the given pwCollisionObject?
    //
    // \param CollisionObject: Object that is to be tested on collision
    //
    // \return True, if the pwCollisionObjects collide, false otherwise
    ////////////////////////////////////////////////////////////////
    bool Collides(const pwCollisionObject* CollisionObject) const;

    ////////////////////////////////////////////////////////////////
    // Collides the Object with the given pwCollisionCircle?
    //
    // \param CollisionCircle: Circle, that is to be tested on collision
    //
    // \return True, if the pwCollisionObjects collide, false otherwise
    ////////////////////////////////////////////////////////////////
    bool Collides(const pwCollisionCircle* CollisionCircle) const;

    ////////////////////////////////////////////////////////////////
    // Collides the Object with the given pwCollisionRect?
    //
    // \param Rect, that is to be tested on collision
    //
    // \return True, if the pwCollisionObjects collide, false otherwise
    ////////////////////////////////////////////////////////////////
    bool Collides(const pwCollisionRect* CollisionRect) const;

    ////////////////////////////////////////////////////////////////
    // Do two line segments collide?
    // \param a1:      Start of the first line segment
    // \param a2:      End of the first line segment
    // \param b1:      Start of the second line segment
    // \param b2:      End of the second line segment
    //
    // \return True, if line segments collide
    ////////////////////////////////////////////////////////////////
    inline bool CollideLineSegments(const sf::Vector2f& a1, const sf::Vector2f& a2, const sf::Vector2f& b1, const sf::Vector2f& b2) const;
    inline bool CollideRayAndLineSegment(const sf::Vector2f& a1, const sf::Vector2f& a2, const sf::Vector2f& b1, const sf::Vector2f& b2) const;

    ////////////////////////////////////////////////////////////////
    // Set the size of the rect
    ////////////////////////////////////////////////////////////////
    void SetSize(const sf::Vector2f& Size);
    void SetSize(float xSize, float ySize);

    const sf::Vector2f& GetSize() const   {return mySize;}

    ////////////////////////////////////////////////////////////////
    // Get a the rect
    //
    // \return Rect
    ////////////////////////////////////////////////////////////////
    const sf::FloatRect GetRect() const;

    ////////////////////////////////////////////////////////////////
    // Set the color of the pwCollisionRect
    //
    // \param Constant reference to a sf::Color-Object
    ////////////////////////////////////////////////////////////////
    void SetColor(const sf::Color& col)   { myShape.setFillColor(col);}

    ////////////////////////////////////////////////////////////////
    // Is this pwCollisionObject a rect or a circle
    //
    // \return See pwCollisionObject::IsRect()
    ////////////////////////////////////////////////////////////////
    const bool IsRect() const {return true;}

    void draw(sf::RenderTarget& target, sf::RenderStates states) const;


private:

    ////////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////////
    sf::Vector2f  mySize;
    mutable sf::RectangleShape myShape;
};

////////////////////////////////////////////////////////////////
// The top-class for all the objects in a game
////////////////////////////////////////////////////////////////
class pwGameLogics
{

public:

    ////////////////////////////////////////////////////////////////
    // Default constructor
    ////////////////////////////////////////////////////////////////
    pwGameLogics();

    ////////////////////////////////////////////////////////////////
    // Destructor
    ////////////////////////////////////////////////////////////////
    ~pwGameLogics();

    ////////////////////////////////////////////////////////////////
    // Get the static instance of the pwGameLogics
    //
    // \return Static instance of the pwGameLogics
    ////////////////////////////////////////////////////////////////
    static pwGameLogics* get();

    ////////////////////////////////////////////////////////////////
    // Initialize the logics-system
    //
    // \return True, if initialisation was successful, false otherwise
    ////////////////////////////////////////////////////////////////
    bool Initialize();

    ////////////////////////////////////////////////////////////////
    // Shuts the logics-system down
    //
    // \return True, if shutdown was successful, false otherwise
    ////////////////////////////////////////////////////////////////
	bool Shutdown();

    ////////////////////////////////////////////////////////////////
    // Updates the logics-system
    //
    // \param GameStateName: The name, under which the objects are registered, which are to be updated
    //
    // \return True, if update was successful, false otherwise
    ////////////////////////////////////////////////////////////////
    bool Update(unsigned int GameStateID);

    ////////////////////////////////////////////////////////////////
    // Register a game-object by passing a pwGameObject-class pointer
    //
    // \param GameObject: Pointer to the game-object to register
    // \param GameState: Under which game-state is the object to be registered
    //
    // \return True, if registration was successful, false otherwise
    ////////////////////////////////////////////////////////////////
	bool RegisterGameObject(pwGameObject *GameObject, unsigned int GameStateID);

	////////////////////////////////////////////////////////////////
    // Unregister a game-object by passing a pwGameObject-class pointer
    //
    // \param GameObject: Pointer to the game-object to unregister
    //
    // \return True, if unregistration was successful, false otherwise
    ////////////////////////////////////////////////////////////////
    bool UnregisterGameObject(pwGameObject *GameObject);

    ////////////////////////////////////////////////////////////////
    // Unregisters all game-object
    //
    // \return True, if unregistration was successful, false otherwise
    ////////////////////////////////////////////////////////////////
    bool UnregisterAll();

    // TODO: comment
    bool TestCollision(pwGameObject *GameObject1, pwGameObject *GameObject2, pwGameState* GameState);

    ////////////////////////////////////////////////////////////////
    // Set and get the gravity-value (in: pixel / sqr(second))
    ////////////////////////////////////////////////////////////////
    void SetGravity(float g = 100.0f)      {myGravity = g;}
    float GetGravity()                     {return myGravity;}

    void SetPaused(bool paused)            {myPaused = paused;}

private:

    ////////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////////
    static pwGameLogics* p_inst;

    bool myPaused;
    float myGravity;
    std::list<std::pair<unsigned int, pwGameObject*>> myGameStateObjects;
};

#endif
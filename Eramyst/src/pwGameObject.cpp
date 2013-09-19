#include "..\include\pwGameObject.hpp"

////////////////////////////////////////////////////////////
// Default constructor
////////////////////////////////////////////////////////////
pwGameObject::pwGameObject() :
    myCollision(false),
    myID(0),
    myGravityMultiplier(0.0f),
    myMass(1.0f),
    myInvMass(1.0f),
    myVelocity(0.0f, 0.0f),
    myAcceleration(0.0f, 0.0f),
    myForce(0.0f, 0.0f),
    myRenderCollisionObjects(false),
    myAlive (true)
{
}

////////////////////////////////////////////////////////////////
// Constructor overload to set myID
////////////////////////////////////////////////////////////////
pwGameObject::pwGameObject(unsigned int GameObjectID) :
    myCollision(false),
    myID(GameObjectID),
    myGravityMultiplier(0.0f),
    myMass(1.0f),
    myInvMass(1.0f),
    myVelocity(0.0f, 0.0f),
    myAcceleration(0.0f, 0.0f),
    myForce(0.0f, 0.0f),
    myRenderCollisionObjects(false),
    myAlive (true)
{
    // Nothing to do
}

////////////////////////////////////////////////////////////
// Destructor
////////////////////////////////////////////////////////////
pwGameObject::~pwGameObject()
{
    // Nothing to do
}

////////////////////////////////////////////////////////////
// Add a drawable-object to the pwGameObject
////////////////////////////////////////////////////////////
bool pwGameObject::AddDrawableObject(unsigned int ObjectID, sf::Drawable* DrawableObject)
{
    // Pointer not NULL?
    if (DrawableObject == NULL)
        return false;

    // The pair to later add to the map
    std::pair<unsigned int, sf::Drawable*> DrawablePair;

    // Fill the pair with the parameters
    DrawablePair.first = ObjectID;
    DrawablePair.second = DrawableObject;

    // Insert the pair
    myDrawableMap.push_back(DrawablePair);

    // Insertion successful
    return true;
}

////////////////////////////////////////////////////////////
// Add a collision-object to the pwGameObject
////////////////////////////////////////////////////////////
bool pwGameObject::AddCollisionObject(unsigned int ObjectID, pwCollisionObject* CollisionObject)
{
    // Pointer not NULL?
    if (CollisionObject == NULL)
        return false;

    // The pair to later add to the map
    std::pair<unsigned int, pwCollisionObject*> CollisionPair;

    // Fill the pair with the parameters
    CollisionPair.first = ObjectID;
    CollisionPair.second = CollisionObject;

    // Insert the pair
    myCollisionMap.push_back(CollisionPair);

    // Insertion successful
    return true;
}


////////////////////////////////////////////////////////////
// Is called, if another object collides with this one
////////////////////////////////////////////////////////////
void pwGameObject::OnCollision(const pwGameObject* GameObject, unsigned int thisCollisionObjectID, unsigned int CollisionObjectID)
{
    // Nothing to do here on default
}

////////////////////////////////////////////////////////////
// Is called, if another object collides with this one the first time
////////////////////////////////////////////////////////////
void pwGameObject::EnterCollision(const pwGameObject* GameObject, unsigned int thisCollisionObjectID, unsigned int CollisionObjectID)
{
    // Nothing to do here on default
}

////////////////////////////////////////////////////////////
// Is called, if another object collided last frame with this one for the last time
////////////////////////////////////////////////////////////
void pwGameObject::LeftCollision()
{
    // Nothing to do here on default
}

////////////////////////////////////////////////////////////
// Updates the position of the object
////////////////////////////////////////////////////////////
void pwGameObject::Update(float FrameTime)
{
    myVelocity = myVelocity + FrameTime*myAcceleration + FrameTime*(myForce*myInvMass);
    setPosition(getPosition() + (myVelocity*FrameTime));

    myForce.x = 0.0f;
    myForce.y = 0.0f;
}

////////////////////////////////////////////////////////////
// Custom update-function, that is called every frame
////////////////////////////////////////////////////////////
void pwGameObject::OnUpdate(float FrameTime)
{
    // Nothing to do here on default
}

////////////////////////////////////////////////////////////
// Set the force-vector of the pwGameObject
////////////////////////////////////////////////////////////
void pwGameObject::ApplyForce(const sf::Vector2f &Force)
{
    myForce += Force;
}
void pwGameObject::ApplyForce(float xForce, float yForce)
{
    myForce.x += xForce;
    myForce.y += yForce;
}

void pwGameObject::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    for (auto i = myDrawableMap.begin(); i != myDrawableMap.end(); i++)
    {
        target.draw(*(i->second), sf::RenderStates(states.blendMode, states.transform * getTransform(), states.texture, states.shader));
    }

    if (myRenderCollisionObjects)
    {
        for (auto i = myCollisionMap.begin(); i != myCollisionMap.end(); i++)
        {
            target.draw(*(i->second), sf::RenderStates(states.blendMode, states.transform * getTransform(), states.texture, states.shader));
        }
    }
}

////////////////////////////////////////////////////////////////
// Plays a sound at the current position
////////////////////////////////////////////////////////////////
void pwGameObject::PlaySound(sf::SoundBuffer* Buffer, unsigned int Layer)
{
    if (pwAudio::get()->PlaySound(Buffer, Layer))
    {
        pwAudio::get()->GetSoundLayerSettings(Layer)->SetPosition(this->getTransform().transformPoint(getPosition()));
    }
}

void pwGameObject::updateCollisionObjects() 
{
    for (auto i = myCollisionMap.begin(); i != myCollisionMap.end(); i++)
    {
        i->second->SetAbsTransformable(*this);
    }
}

void pwGameObject::SetMass(float m)
{
    if (m == 0.0f) 
    {
        pwLog::get()->WriteLine("Mass cannot be set to zero: Division through zero would accure!", pwLogType::pwLTCaution);
        return; 
    }
    
    myMass = m; 
    myInvMass = 1.0f / m;
}

void pwGameObject::Kill()
{
    myAlive = false;
    pwGameLogics::get()->UnregisterGameObject(this);
}
#include "..\include\pwGameLogics.hpp"


////////////////////////////////////////////////////////////////
// Declaration of the static member variable
////////////////////////////////////////////////////////////////
pwGameLogics* pwGameLogics::p_inst = NULL;

////////////////////////////////////////////////////////////
// Default constructor
////////////////////////////////////////////////////////////
pwCollisionObject::pwCollisionObject()
{
    // Nothing to do
}

////////////////////////////////////////////////////////////
// Destructor
////////////////////////////////////////////////////////////
pwCollisionObject::~pwCollisionObject()
{
    // Nothing to do
}

////////////////////////////////////////////////////////////////
// Default constructor
////////////////////////////////////////////////////////////////
pwCollisionCircle::pwCollisionCircle() :
    myRadius(1.0f)
{
    // Nothing to do
}

////////////////////////////////////////////////////////////////
// Destructor
////////////////////////////////////////////////////////////////
pwCollisionCircle::~pwCollisionCircle()
{
    // Nothing to do
}

////////////////////////////////////////////////////////////////
// Collides the Object with the given pwCollisionObject?
////////////////////////////////////////////////////////////////
bool pwCollisionCircle::Collides(const pwCollisionObject* CollisionObject) const
{
    // Is the object a rect
    if (CollisionObject->IsRect())
    {
        // Than convert it to a pwCollisionRect-object and call the Collides-funktion
        return Collides((pwCollisionRect*)(CollisionObject));
    }
    else
    {
        // Else convert it to a pwCollisionCircle-object and call the Collides-funktion
        return Collides((pwCollisionCircle*)(CollisionObject));
    }
}

////////////////////////////////////////////////////////////////
// Collides the Object with the given pwCollisionCircle?
////////////////////////////////////////////////////////////////
bool pwCollisionCircle::Collides(const pwCollisionCircle* CollisionCircle) const
{
    sf::Transformable TransformableObjectThis = *this; TransformableObjectThis.setOrigin(getPosition()+ getOrigin());
    sf::Transformable TransformableObjectOther = *CollisionCircle; TransformableObjectOther.setOrigin(CollisionCircle->getPosition() + CollisionCircle->getOrigin());
    sf::Vector2f PositionObjectThis = (this->myAbsTransformable.getTransform() * TransformableObjectThis.getTransform()).transformPoint(getPosition());
    sf::Vector2f PositionObjectOther = (CollisionCircle->GetAbsTransformable().getTransform() * TransformableObjectOther.getTransform()).transformPoint(CollisionCircle->getPosition());

    // Two circles collide, if the distance of their centers are smaller than the summ of their radii
    float DX = PositionObjectOther.x-PositionObjectThis.x; DX *= DX;
    float DY = PositionObjectOther.y-PositionObjectThis.y; DY *= DY; 
    float DD = CollisionCircle->GetRadius()+GetRadius(); DD *= DD;

    // Check with the Pythagorean theorem
    if (DX+DY <= DD)
        return true;
    else
        return false;

}

////////////////////////////////////////////////////////////////
// Collides the Object with the given pwCollisionRect?
////////////////////////////////////////////////////////////////
bool pwCollisionCircle::Collides(const pwCollisionRect* CollisionRect) const
{
    sf::Transformable TransformableObjectThis = *this;           TransformableObjectThis.setOrigin(getPosition()+ getOrigin());
    sf::Transformable TransformableObjectOther = *CollisionRect; TransformableObjectOther.setOrigin(CollisionRect->getPosition() + CollisionRect->getOrigin());
    sf::Vector2f PositionObjectThis = (this->myAbsTransformable.getTransform() * TransformableObjectThis.getTransform()).transformPoint(getPosition());

    if (CollisionRect->getRotation() == 0.0f && CollisionRect->GetAbsTransformable().getRotation() == 0.0f) 
    {
        // Use simple collision-detection, if the rotation of the square equals 0.0f
        sf::Vector2f PositionObjectOtherTL = (CollisionRect->GetAbsTransformable().getTransform() * TransformableObjectOther.getTransform()).transformPoint(CollisionRect->getPosition());
        sf::Vector2f PositionObjectOtherBR; 
        PositionObjectOtherBR.x = PositionObjectOtherTL.x + CollisionRect->GetRect().width;
        PositionObjectOtherBR.y = PositionObjectOtherTL.y + CollisionRect->GetRect().height;

        // A circle and a rect collide, if on of the 4 sides collide with the circle
        float xPointOnRect = PositionObjectThis.x;
        float yPointOnRect = PositionObjectThis.y;

        // Get the x and y coords of the point on the rect, thats nearest to the middle of the circle
        if (xPointOnRect < PositionObjectOtherTL.x) xPointOnRect = PositionObjectOtherTL.x;
        if (xPointOnRect > PositionObjectOtherBR.x) xPointOnRect = PositionObjectOtherBR.x;
        if (yPointOnRect < PositionObjectOtherTL.y) yPointOnRect = PositionObjectOtherTL.y;
        if (yPointOnRect > PositionObjectOtherBR.y) yPointOnRect = PositionObjectOtherBR.y;

        float DX = PositionObjectThis.x - xPointOnRect;
        float DY = PositionObjectThis.y - yPointOnRect;

        // Check with the Pythagorean theorem
        return ((DX*DX)+(DY*DY) <= myRadius*myRadius);
    } else {
        // Use advanced collision-detection, if the rotation of the square differs from 0.0f
        sf::Vector2f PositionObjectOtherTLRotated = CollisionRect->getPosition();
        sf::Vector2f PositionObjectOtherTRRotated = CollisionRect->getPosition(); PositionObjectOtherTRRotated.x += CollisionRect->GetRect().width;
        sf::Vector2f PositionObjectOtherBLRotated = CollisionRect->getPosition(); PositionObjectOtherBLRotated.y += CollisionRect->GetRect().height;
        sf::Vector2f PositionObjectOtherBRRotated = CollisionRect->getPosition(); PositionObjectOtherBRRotated += sf::Vector2f(CollisionRect->GetRect().width, CollisionRect->GetRect().height);

        PositionObjectOtherTLRotated = (CollisionRect->GetAbsTransformable().getTransform() * TransformableObjectOther.getTransform()).transformPoint(PositionObjectOtherTLRotated);
        PositionObjectOtherTRRotated = (CollisionRect->GetAbsTransformable().getTransform() * TransformableObjectOther.getTransform()).transformPoint(PositionObjectOtherTRRotated);
        PositionObjectOtherBLRotated = (CollisionRect->GetAbsTransformable().getTransform() * TransformableObjectOther.getTransform()).transformPoint(PositionObjectOtherBLRotated);
        PositionObjectOtherBRRotated = (CollisionRect->GetAbsTransformable().getTransform() * TransformableObjectOther.getTransform()).transformPoint(PositionObjectOtherBRRotated);
        
        return (CollidesWithLineSegment(PositionObjectThis, PositionObjectOtherTLRotated, PositionObjectOtherTRRotated) || 
                CollidesWithLineSegment(PositionObjectThis, PositionObjectOtherTRRotated, PositionObjectOtherBRRotated) ||
                CollidesWithLineSegment(PositionObjectThis, PositionObjectOtherBRRotated, PositionObjectOtherBLRotated) ||
                CollidesWithLineSegment(PositionObjectThis, PositionObjectOtherBLRotated, PositionObjectOtherTLRotated));
    }
}

////////////////////////////////////////////////////////////////
// Collides the pwCollisionCircle with the given line segment?
////////////////////////////////////////////////////////////////
inline bool pwCollisionCircle::CollidesWithLineSegment(const sf::Vector2f& c, const sf::Vector2f& a, const sf::Vector2f& b) const
{
    // Calculate the direction-vector of the line segment and it's length
    sf::Vector2f AB = (b - a);                                  // direction-vector
    const float ABlengthSQ = pwHelper::math::vector2::lengthSQ(AB); // length of the line-segment

    // Inverse of ABlength to speed up calculations (divisions are too slow!)
    const float invABlengthSQ = 1.0f / ABlengthSQ;

    // Precalculate term, thats used more than once
    const float p = (pwHelper::math::vector2::dot(AB, c) - pwHelper::math::vector2::dot(a, AB)) * invABlengthSQ;

    // Calculate diskriminant
    const float discriminant = p * p - (pwHelper::math::vector2::lengthSQ(a - c) - myRadius * myRadius) * invABlengthSQ;

    if (discriminant < 0.0f)
    {
        // No collision!
        return false;
    }
    else
    {
        // Collision with the line! Check if one of the collision-points is in the line segment.

        // Calculate t1 and t2
        if (discriminant == 0.0f)
        {
            // squareRoot = 0
            // t1 == t2 == p
            return (p >= 0 && p <= 1);
        }
        else
        {
            const float squareRoot = pwHelper::math::sqrt(discriminant);
            const float t1 = p - squareRoot;
            const float t2 = p + squareRoot;

            return (t1 >= 0 && t1 <= 1) || (t2 >= 0 && t2 <= 1);
        }

        // Collision occures at points:
        // p1 = a + t1*AB
        // p2 = a + t2*AB
        // Collision with the line segment (as it is from a to a+AB), if:
        // (t1 >= 0 && t1 <= 1) || (t2 >= 0 && t2 <= 1)
    }
}

////////////////////////////////////////////////////////////////
// Set a the radius of the circle
////////////////////////////////////////////////////////////////
void pwCollisionCircle::SetRadius(float Radius)
{
    // Set the new radius
    myRadius = Radius;
}


void pwCollisionCircle::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    myShape.setRadius(GetRadius());
    myShape.setOrigin(GetRadius(), GetRadius());
    target.draw(myShape, sf::RenderStates(states.blendMode, this->getTransform() * states.transform, states.texture, states.shader));
}


////////////////////////////////////////////////////////////////
// Default constructor
////////////////////////////////////////////////////////////////
pwCollisionRect::pwCollisionRect() :
    mySize(1.0f, 1.0f)
{
    // Nothing to do
}

////////////////////////////////////////////////////////////////
// Destructor
////////////////////////////////////////////////////////////////
pwCollisionRect::~pwCollisionRect()
{
    // Nothing to do
}

////////////////////////////////////////////////////////////////
// Collides the Object with the given pwCollisionObject?
////////////////////////////////////////////////////////////////
bool pwCollisionRect::Collides(const pwCollisionObject* CollisionObject) const
{
    // Is the object a rect
    if (CollisionObject->IsRect())
    {
        // Than convert it to a pwCollisionRect-object and call the Collides-funktion
        return Collides((pwCollisionRect*)(CollisionObject));
    }
    else
    {
        // Else convert it to a pwCollisionCircle-object and call the Collides-funktion
        return Collides((pwCollisionCircle*)(CollisionObject));
    }
}

////////////////////////////////////////////////////////////////
// Collides the Object with the given pwCollisionCircle?
////////////////////////////////////////////////////////////////
bool pwCollisionRect::Collides(const pwCollisionCircle* CollisionCircle) const
{
    // Collides with circle? = circle collides with this.
    return CollisionCircle->Collides(this);
}

////////////////////////////////////////////////////////////////
// Collides the Object with the given pwCollisionRect?
////////////////////////////////////////////////////////////////
bool pwCollisionRect::Collides(const pwCollisionRect* CollisionRect) const
{
    sf::Transformable TransformableObjectThis = *this; TransformableObjectThis.setOrigin(getPosition()+ getOrigin());
    sf::Transformable TransformableObjectOther = *CollisionRect; TransformableObjectOther.setOrigin(CollisionRect->getPosition() + CollisionRect->getOrigin());
    sf::Vector2f PositionObjectThis = (this->myAbsTransformable.getTransform() * TransformableObjectThis.getTransform()).transformPoint(getPosition());
    sf::Vector2f PositionObjectOther = (CollisionRect->GetAbsTransformable().getTransform() * TransformableObjectOther.getTransform()).transformPoint(CollisionRect->getPosition());

    sf::FloatRect Rect1 = GetRect();                // Get the own rect
    sf::FloatRect Rect2 = CollisionRect->GetRect(); // Get the rect of the CollisionRect

    if (CollisionRect->getRotation() == 0.0f && CollisionRect->GetAbsTransformable().getRotation() == 0.0f &&
        this->getRotation() == 0.0f && this->GetAbsTransformable().getRotation() == 0.0f) 
    {
        // Use simple collision-detection, if the rotation of the squares equals 0.0f

        // Do the rects collide?
        if ((PositionObjectThis.x > (PositionObjectOther.x + Rect2.width)) || ((PositionObjectThis.x + Rect1.width) < PositionObjectOther.x))
            return false;
        if ((PositionObjectThis.y > (PositionObjectOther.y + Rect2.height)) || ((PositionObjectThis.y + Rect1.height) < PositionObjectOther.y))
            return false;

        return true;
    } else {
        // Use advanced collision-detection, if the rotation of the square differs from 0.0f
        sf::Vector2f PositionObjectThisTLRotated = getPosition();
        sf::Vector2f PositionObjectThisTRRotated = getPosition(); PositionObjectThisTRRotated.x += Rect1.width;
        sf::Vector2f PositionObjectThisBLRotated = getPosition(); PositionObjectThisBLRotated.y += Rect1.height;
        sf::Vector2f PositionObjectThisBRRotated = getPosition(); PositionObjectThisBRRotated += sf::Vector2f(Rect1.width, Rect1.height);

        sf::Vector2f PositionObjectOtherTLRotated = CollisionRect->getPosition();
        sf::Vector2f PositionObjectOtherTRRotated = CollisionRect->getPosition(); PositionObjectOtherTRRotated.x += Rect2.width;
        sf::Vector2f PositionObjectOtherBLRotated = CollisionRect->getPosition(); PositionObjectOtherBLRotated.y += Rect2.height;
        sf::Vector2f PositionObjectOtherBRRotated = CollisionRect->getPosition(); PositionObjectOtherBRRotated += sf::Vector2f(Rect2.width, Rect2.height);

        // Transform the points
        PositionObjectThisTLRotated = (GetAbsTransformable().getTransform() * TransformableObjectThis.getTransform()).transformPoint(PositionObjectThisTLRotated);
        PositionObjectThisTRRotated = (GetAbsTransformable().getTransform() * TransformableObjectThis.getTransform()).transformPoint(PositionObjectThisTRRotated);
        PositionObjectThisBLRotated = (GetAbsTransformable().getTransform() * TransformableObjectThis.getTransform()).transformPoint(PositionObjectThisBLRotated);
        PositionObjectThisBRRotated = (GetAbsTransformable().getTransform() * TransformableObjectThis.getTransform()).transformPoint(PositionObjectThisBRRotated);

        PositionObjectOtherTLRotated = (CollisionRect->GetAbsTransformable().getTransform() * TransformableObjectOther.getTransform()).transformPoint(PositionObjectOtherTLRotated);
        PositionObjectOtherTRRotated = (CollisionRect->GetAbsTransformable().getTransform() * TransformableObjectOther.getTransform()).transformPoint(PositionObjectOtherTRRotated);
        PositionObjectOtherBLRotated = (CollisionRect->GetAbsTransformable().getTransform() * TransformableObjectOther.getTransform()).transformPoint(PositionObjectOtherBLRotated);
        PositionObjectOtherBRRotated = (CollisionRect->GetAbsTransformable().getTransform() * TransformableObjectOther.getTransform()).transformPoint(PositionObjectOtherBRRotated);

       if (CollideLineSegments(PositionObjectThisTLRotated, PositionObjectThisTRRotated, PositionObjectOtherTLRotated, PositionObjectOtherTRRotated) || 
           CollideLineSegments(PositionObjectThisTLRotated, PositionObjectThisTRRotated, PositionObjectOtherTRRotated, PositionObjectOtherBRRotated) ||
           CollideLineSegments(PositionObjectThisTLRotated, PositionObjectThisTRRotated, PositionObjectOtherBRRotated, PositionObjectOtherBLRotated) ||
           CollideLineSegments(PositionObjectThisTLRotated, PositionObjectThisTRRotated, PositionObjectOtherBLRotated, PositionObjectOtherTLRotated) ||
           CollideLineSegments(PositionObjectThisTRRotated, PositionObjectThisBRRotated, PositionObjectOtherTLRotated, PositionObjectOtherTRRotated) || 
           CollideLineSegments(PositionObjectThisTRRotated, PositionObjectThisBRRotated, PositionObjectOtherTRRotated, PositionObjectOtherBRRotated) ||
           CollideLineSegments(PositionObjectThisTRRotated, PositionObjectThisBRRotated, PositionObjectOtherBRRotated, PositionObjectOtherBLRotated) ||
           CollideLineSegments(PositionObjectThisTRRotated, PositionObjectThisBRRotated, PositionObjectOtherBLRotated, PositionObjectOtherTLRotated) ||
           CollideLineSegments(PositionObjectThisBRRotated, PositionObjectThisBLRotated, PositionObjectOtherTLRotated, PositionObjectOtherTRRotated) ||
           CollideLineSegments(PositionObjectThisBRRotated, PositionObjectThisBLRotated, PositionObjectOtherTRRotated, PositionObjectOtherBRRotated) ||
           CollideLineSegments(PositionObjectThisBRRotated, PositionObjectThisBLRotated, PositionObjectOtherBRRotated, PositionObjectOtherBLRotated) ||
           CollideLineSegments(PositionObjectThisBRRotated, PositionObjectThisBLRotated, PositionObjectOtherBLRotated, PositionObjectOtherTLRotated) ||
           CollideLineSegments(PositionObjectThisBLRotated, PositionObjectThisTLRotated, PositionObjectOtherTLRotated, PositionObjectOtherTRRotated) ||
           CollideLineSegments(PositionObjectThisBLRotated, PositionObjectThisTLRotated, PositionObjectOtherTRRotated, PositionObjectOtherBRRotated) ||
           CollideLineSegments(PositionObjectThisBLRotated, PositionObjectThisTLRotated, PositionObjectOtherBRRotated, PositionObjectOtherBLRotated) ||
           CollideLineSegments(PositionObjectThisBLRotated, PositionObjectThisTLRotated, PositionObjectOtherBLRotated, PositionObjectOtherTLRotated))
       {
            return true;
       }
       else
       {
           int i = 0;
           if (CollideRayAndLineSegment(PositionObjectThisTLRotated, sf::Vector2f(0.0f, 0.0f), PositionObjectOtherTLRotated, PositionObjectOtherTRRotated))
               i++;
           if (CollideRayAndLineSegment(PositionObjectThisTLRotated, sf::Vector2f(0.0f, 0.0f), PositionObjectOtherTRRotated, PositionObjectOtherBRRotated))
               i++;
           if (CollideRayAndLineSegment(PositionObjectThisTLRotated, sf::Vector2f(0.0f, 0.0f), PositionObjectOtherBRRotated, PositionObjectOtherBLRotated))
               i++;
           if (CollideRayAndLineSegment(PositionObjectThisTLRotated, sf::Vector2f(0.0f, 0.0f), PositionObjectOtherBLRotated, PositionObjectOtherTLRotated))
               i++;

           if (i == 1)
               return true;

           i = 0;
           if (CollideRayAndLineSegment(PositionObjectThisTRRotated, sf::Vector2f(0.0f, 0.0f), PositionObjectOtherTLRotated, PositionObjectOtherTRRotated))
               i++;
           if (CollideRayAndLineSegment(PositionObjectThisTRRotated, sf::Vector2f(0.0f, 0.0f), PositionObjectOtherTRRotated, PositionObjectOtherBRRotated))
               i++;
           if (CollideRayAndLineSegment(PositionObjectThisTRRotated, sf::Vector2f(0.0f, 0.0f), PositionObjectOtherBRRotated, PositionObjectOtherBLRotated))
               i++;
           if (CollideRayAndLineSegment(PositionObjectThisTRRotated, sf::Vector2f(0.0f, 0.0f), PositionObjectOtherBLRotated, PositionObjectOtherTLRotated))
               i++;

           if (i == 1)
               return true;
       }
       return false;
    }
}

inline bool pwCollisionRect::CollideLineSegments(const sf::Vector2f& a1, const sf::Vector2f& a2, const sf::Vector2f& b1, const sf::Vector2f& b2) const
{
    const float denom = (b2.y - b1.y) * (a2.x - a1.x) -
                        (b2.x - b1.x) * (a2.y - a1.y);
    if (abs(denom) < pwHelper::math::EPSILON) return false;
    
    const float ua = ((b2.x - b1.x) * (a1.y - b1.y) -
                      (b2.y - b1.y) * (a1.x - b1.x)) / denom;
    const float ub = ((a2.x - a1.x) * (a1.y - b1.y) -
                      (a2.y - a1.y) * (a1.x - b1.x)) / denom;
    return ua >= 0 && ua < 1 && ub >= 0 && ub <= 1;
}

inline bool pwCollisionRect::CollideRayAndLineSegment(const sf::Vector2f& a1, const sf::Vector2f& a2, const sf::Vector2f& b1, const sf::Vector2f& b2) const
{
    const float denom = (b2.y - b1.y) * (a2.x - a1.x) -
                        (b2.x - b1.x) * (a2.y - a1.y);
    if (abs(denom) < pwHelper::math::EPSILON) return false;
    
    const float ua = ((b2.x - b1.x) * (a1.y - b1.y) -
                      (b2.y - b1.y) * (a1.x - b1.x)) / denom;
    const float ub = ((a2.x - a1.x) * (a1.y - b1.y) -
                      (a2.y - a1.y) * (a1.x - b1.x)) / denom;
    return ua >= 0 && ub >= 0 && ub <= 1;
}

////////////////////////////////////////////////////////////////
// Set the size of the rect
////////////////////////////////////////////////////////////////
void pwCollisionRect::SetSize(const sf::Vector2f& Size)
{
    // Set the new size
    mySize = Size;
}

void pwCollisionRect::SetSize(float xSize, float ySize)
{
    // Set the new size
    mySize.x = xSize;
    mySize.y = ySize;
}

////////////////////////////////////////////////////////////////
// Get a the rect
////////////////////////////////////////////////////////////////
const sf::FloatRect pwCollisionRect::GetRect() const
{
    sf::FloatRect myRect;

    // Recalculate the rect
    myRect.left = getPosition().x;
    myRect.top = getPosition().y;
    myRect.width = mySize.x;
    myRect.height = mySize.y;

    // Return the rect
    return myRect;
}

void pwCollisionRect::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    myShape.setSize(GetSize());
    target.draw(myShape, sf::RenderStates(states.blendMode, states.transform * this->getTransform(), states.texture, states.shader));
}

////////////////////////////////////////////////////////////
// Default constructor
////////////////////////////////////////////////////////////
pwGameLogics::pwGameLogics()
{
    // Nothing to do
    myPaused = false;
}

////////////////////////////////////////////////////////////
// Destructor
////////////////////////////////////////////////////////////
pwGameLogics::~pwGameLogics()
{
    // Nothing to do
}

////////////////////////////////////////////////////////////////
// Get the static instance of the pwGraphics
////////////////////////////////////////////////////////////////
pwGameLogics* pwGameLogics::get()
{
    // If the engine doesn't exist, create it
    if (!p_inst)
		p_inst = new pwGameLogics;

    // Return the pointer to the engine
	return p_inst;
}

////////////////////////////////////////////////////////////////
// Initialize the logics-system
////////////////////////////////////////////////////////////////
bool pwGameLogics::Initialize()
{
    return true;
}

////////////////////////////////////////////////////////////////
// Shuts the logics-system down
////////////////////////////////////////////////////////////////
bool pwGameLogics::Shutdown()
{
    return true;
}

////////////////////////////////////////////////////////////////
// Updates the logics-system
////////////////////////////////////////////////////////////////
bool pwGameLogics::Update(unsigned int GameStateID)
{
    if (myPaused)
        return true;

    // Start the list at the beginning
    for (std::list<std::pair<unsigned int, pwGameObject*>>::iterator i = myGameStateObjects.begin(); i != myGameStateObjects.end(); i++)
    {
        // An object of the active game-state?
        if (i->first != GameStateID)
            continue;

        // Update the GameObject
        i->second->ApplyForce(sf::Vector2f(0.0f, i->second->GetMass() * myGravity * i->second->GetGravityMultiplier()));
        i->second->Update(pwGraphics::get()->GetLastFrameTime());
        i->second->OnUpdate(pwGraphics::get()->GetLastFrameTime());
    }

    // Nothing can go wrong
    return true;
}

////////////////////////////////////////////////////////////////
// Register a game-object by passing a pwGameObject-class pointer
////////////////////////////////////////////////////////////////
bool pwGameLogics::RegisterGameObject(pwGameObject *GameObject, unsigned int GameStateID)
{
    // Pair for insertion
    std::pair<unsigned int, pwGameObject*> GameObjectPair;

    // GameObject already existent?
    for (std::list<std::pair<unsigned int, pwGameObject*>>::iterator i = myGameStateObjects.begin(); i != myGameStateObjects.end(); i++)
    {
        if (i->first == GameStateID && i->second == GameObject)
        {
            // GameObject does already exist
            return false;
        }
    }

    // Fill pair-data
    GameObjectPair.first = GameStateID;
    GameObjectPair.second = GameObject;

    // Try to insert the game-state in the map
    myGameStateObjects.push_back(GameObjectPair);

    // Insertion successful
    return true;
}

////////////////////////////////////////////////////////////////
// Unregister a game-object by passing a pwGameObject-class pointer
////////////////////////////////////////////////////////////////
bool pwGameLogics::UnregisterGameObject(pwGameObject *GameObject)
{
	// GameObject existent?
    for (std::list<std::pair<unsigned int, pwGameObject*>>::iterator i = myGameStateObjects.begin(); i != myGameStateObjects.end(); i++)
    {
        if (i->second == GameObject)
        {
            // GameObject does exist
			myGameStateObjects.erase(i);
            return true;
        }
    }

	return false;
}

////////////////////////////////////////////////////////////////
// Unregisters all game-object
////////////////////////////////////////////////////////////////
bool pwGameLogics::UnregisterAll()
{
    // Clear the map
    myGameStateObjects.clear();

    // Nothing can go wrong
    return true;
}

bool pwGameLogics::TestCollision(pwGameObject *GameObject1, pwGameObject *GameObject2, pwGameState* GameState)
{
    // TODO: comment
    bool CollidedOnce = false;

    // Go through all the pwCollisionObjects of both objects, that are to be tested
    // List of the first object
    for (std::list<std::pair<unsigned int, pwCollisionObject*>>::iterator k = GameObject1->GetCollisionObjectMap()->begin(); k != GameObject1->GetCollisionObjectMap()->end(); k++)
    {
        // List of the second object
        for (std::list<std::pair<unsigned int, pwCollisionObject*>>::iterator l = GameObject2->GetCollisionObjectMap()->begin(); l != GameObject2->GetCollisionObjectMap()->end(); l++)
        {
            // Do pwCollisionObjects collide?
            if (k->second->Collides(l->second))
            {
                // Than notify the objects and the game-state
                if (GameObject1->GetCollision())
                    GameObject1->OnCollision(GameObject2, k->first, l->first);
                else 
                {
                    GameObject1->EnterCollision(GameObject2, k->first, l->first);
                    GameObject1->OnCollision(GameObject2, k->first, l->first);
                    GameObject1->SetCollision(true);
                }

                if (GameObject2->GetCollision())
                    GameObject2->OnCollision(GameObject1, l->first, k->first);
                else
                {
                    GameObject2->EnterCollision(GameObject1, l->first, k->first);
                    GameObject2->OnCollision(GameObject1, l->first, k->first);
                    GameObject2->SetCollision(true);
                }

                if (GameState != NULL)
                    GameState->OnCollision(GameObject1,  k->first, GameObject2, l->first);

                CollidedOnce = true;
            }
        }
    }

    if (CollidedOnce == false && GameObject1->GetCollision() == true)
    {
        GameObject1->LeftCollision();
        GameObject1->SetCollision(false);
    }
    if (CollidedOnce == false && GameObject2->GetCollision() == true)
    {
        GameObject2->LeftCollision();
        GameObject2->SetCollision(false);
    }

    return CollidedOnce;
}
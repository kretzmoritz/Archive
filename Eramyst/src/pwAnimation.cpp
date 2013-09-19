#include "..\include\pwAnimation.hpp"

////////////////////////////////////////////////////////////
// Default constructor
////////////////////////////////////////////////////////////
pwAnimation::pwAnimation() :
    myFrameSize     (0, 0),
    myTotalFrames   (1),
    myFramesPerSecond (1.0f),
    myWasPlayed     (false),
    myPlayOnce      (false),
    myCurrentFrame  (0),
    myCurrentAnimation(0, 0),
    myCurrentAnimationID(0),
    myAutoResetAnimation(false)
{
}

////////////////////////////////////////////////////////////
// Construct the animation from a source image
////////////////////////////////////////////////////////////
pwAnimation::pwAnimation(const sf::Texture& texture) :
    sf::Sprite(texture),
    myFrameSize     (texture.getSize().x, texture.getSize().y),
    myTotalFrames   (1),
    myFramesPerSecond (1.0f),
    myWasPlayed     (false),
    myPlayOnce      (false),
    myCurrentFrame  (0),
    myCurrentAnimation(0, 0)
{
}

////////////////////////////////////////////////////////////
// Set the animation-frame size
////////////////////////////////////////////////////////////
void pwAnimation::SetFrameSize(const sf::Vector2u& FrameSize)
{
    myFrameSize = FrameSize;
}

void pwAnimation::SetFrameSize(int xFrameSize, int yFrameSize)
{
    myFrameSize.x = xFrameSize;
    myFrameSize.y = yFrameSize;
}

////////////////////////////////////////////////////////////
// Set the total of animation-frames
////////////////////////////////////////////////////////////
void pwAnimation::SetTotalFrames(int TotalFrames)
{
    myTotalFrames = TotalFrames;
    ResetCurrentAnimation();
}

////////////////////////////////////////////////////////////
// Set the current animation-frame
////////////////////////////////////////////////////////////
void pwAnimation::SetCurrentFrame(float CurrentFrame)
{
    myCurrentFrame = CurrentFrame;
}

////////////////////////////////////////////////////////////
// Set the count of frames, to be played during one single second
////////////////////////////////////////////////////////////
void pwAnimation::SetFramesPerSecond(float FramesPerSecond)
{
    myFramesPerSecond = FramesPerSecond;
}

void pwAnimation::update() const
{
    const sf::Vector2u* mySize;
    if (myFrameSize.x == 0 || myFrameSize.y == 0)
    {
        mySize = &getTexture()->getSize();
    } else {
        mySize = &myFrameSize;
    }

    mySprite = *((sf::Sprite*) this);

    // Calculate, which frame is to be rendered next
    myCurrentFrame += myFramesPerSecond * pwGraphics::get()->GetLastFrameTime();

    if (myFramesPerSecond > 0.0f && myCurrentFrame >= (myCurrentAnimation.y+1))
    {
        myCurrentFrame = (float)(myCurrentAnimation.x);

        if (myPlayOnce)
            myWasPlayed = true;
    }
    else if (myFramesPerSecond < 0.0f && myCurrentFrame < (myCurrentAnimation.x))
    {
        myCurrentFrame = (float)(myCurrentAnimation.y);

        if (myPlayOnce)
            myWasPlayed = true;
    }

    if (myPlayOnce && myWasPlayed)
    {
        if ((myAutoResetAnimation && myFramesPerSecond > 0.0f) || (!myAutoResetAnimation && myFramesPerSecond < 0.0f))
            myCurrentFrame = (float)(myCurrentAnimation.x);
        else
            myCurrentFrame = (float)(myCurrentAnimation.y);
    }
    
    // Calculate the count of frames in the containing-rect
    int FramesInRow = (int)(getTextureRect().width / mySize->x);

    // Position of the current frame
    int CurrentFrameXPos = ((int)(myCurrentFrame) % FramesInRow) * mySize->x;
    int CurrentFrameYPos = (int)(myCurrentFrame / FramesInRow) * mySize->y;

    mySprite.setTextureRect(sf::IntRect(getTextureRect().left + CurrentFrameXPos, getTextureRect().top + CurrentFrameYPos,
        mySize->x, mySize->y));
}

////////////////////////////////////////////////////////////
// Updates the animation
////////////////////////////////////////////////////////////
void pwAnimation::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    update();

    target.draw(mySprite, states);
}

////////////////////////////////////////////////////////////
// Adds a named animation to the pwAnimation
////////////////////////////////////////////////////////////
void pwAnimation::AddAnimation(unsigned int AnimationID, const sf::Vector2i& Frames)
{
    // The pair to add to the map
    std::pair<unsigned int, sf::Vector2i> Pair;

    // The name of the animation is the key
    Pair.first = AnimationID;
    Pair.second = Frames;

    // Insert the pair into the map
    myAnimations.insert(Pair);
}

void pwAnimation::AddAnimation(unsigned int AnimationID, int xFrame, int yFrame)
{
    // The pair to add to the map
    std::pair<unsigned int, sf::Vector2i> Pair;

    // The name of the animation is the key
    Pair.first = AnimationID;
    Pair.second.x = xFrame;
    Pair.second.y = yFrame;

    // Insert the pair into the map
    myAnimations.insert(Pair);
}

////////////////////////////////////////////////////////////
// Sets the current animation of the pwAnimation
////////////////////////////////////////////////////////////
void pwAnimation::SetCurrentAnimation(unsigned int AnimationID)
{
    for (std::map<unsigned int, sf::Vector2i>::iterator i = myAnimations.begin(); i != myAnimations.end(); i++)
    {
        if (i->first == AnimationID)
        {
	        if (myCurrentAnimation == i->second)
            {
                if (myWasPlayed)
                    RestartAnimation();

                return;
            }

            myCurrentAnimationID = i->first;
            myCurrentAnimation = i->second;
            RestartAnimation();
            return;
        }
    }

    ResetCurrentAnimation();
}

bool pwAnimation::WasPlayed()
{
    return (myPlayOnce && myWasPlayed);
}

////////////////////////////////////////////////////////////
// Resets the current animation of the pwAnimation
////////////////////////////////////////////////////////////
void pwAnimation::ResetCurrentAnimation()
{
    myCurrentAnimation.x = 0;
    myCurrentAnimation.y = myTotalFrames-1;
    myCurrentAnimationID = 0;
}
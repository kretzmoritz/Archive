#ifndef PWANIMATION
#define PWANIMATION

#include <SFML\Graphics.hpp>
#include "pwGraphicsSystem.hpp"
#include "pwIniFile.hpp"

class pwAnimation : public sf::Sprite
{

public:

    ////////////////////////////////////////////////////////////
    // Default constructor
    ////////////////////////////////////////////////////////////
    pwAnimation();

    ////////////////////////////////////////////////////////////
    // Construct the pwAnimation from a source texture
    //
    // \param texture : Source texture
    //
    // \see setTexture
    ////////////////////////////////////////////////////////////
    explicit pwAnimation(const sf::Texture& texture);

    ////////////////////////////////////////////////////////////
    // Set the animation-frame size
    //
    // \param FrameSize: New size of the animation-frame
    ////////////////////////////////////////////////////////////
    void SetFrameSize(const sf::Vector2u& FrameSize);
    void SetFrameSize(int xFrameSize, int yFrameSize);

    ////////////////////////////////////////////////////////////
    // Get the animation-frame size
    //
    // \return Size of the animation-frame
    ////////////////////////////////////////////////////////////
    sf::Vector2u GetFrameSize() const            {return myFrameSize;}

    ////////////////////////////////////////////////////////////
    // Set the total of animation-frames
    //
    // \param TotalFrames: Total of animation-frames
    ////////////////////////////////////////////////////////////
    void SetTotalFrames(int TotalFrames);

    ////////////////////////////////////////////////////////////
    // Get the total of animation-frames
    //
    // \return Total of animation-frames
    ////////////////////////////////////////////////////////////
    int GetTotalFrames() const                   {return myTotalFrames;}

    ////////////////////////////////////////////////////////////
    // Set the current animation-frame
    //
    // \param CurrentFrame: Frame to set as current
    ////////////////////////////////////////////////////////////
    void SetCurrentFrame(float CurrentFrame);

    ////////////////////////////////////////////////////////////
    // Get the total of animation-frames
    //
    // \return Total of animation-frames
    ////////////////////////////////////////////////////////////
    float GetCurrentFrame() const                {return myCurrentFrame;}

    ////////////////////////////////////////////////////////////
    // Set the count of frames, to be played during one single second
    //
    // \param FramesPerSecond: Frames per second
    ////////////////////////////////////////////////////////////
    void SetFramesPerSecond(float FramesPerSecond);

    ////////////////////////////////////////////////////////////
    // Get the count of frames, which are played during one single second
    //
    // \return Frames per second
    ////////////////////////////////////////////////////////////
    float GetFramesPerSecond() const             {return myFramesPerSecond;}

    ////////////////////////////////////////////////////////////
    // Plays the animation only once
    //
    // \param PlayOnce: True, to play only once
    ////////////////////////////////////////////////////////////
    void PlayOnce(bool PlayOnce = true)          {myPlayOnce = PlayOnce;}

    ////////////////////////////////////////////////////////////
    // Was the animation already played once
    //
    // \return: True, if the animation was to be played once, and already was played once
    ////////////////////////////////////////////////////////////
    bool WasPlayed();

    ////////////////////////////////////////////////////////////
    // Restarts the animation
    ////////////////////////////////////////////////////////////
    void RestartAnimation()                      {myCurrentFrame = (myFramesPerSecond >= 0.0f ? (float)myCurrentAnimation.x : (float)myCurrentAnimation.y); myWasPlayed = false;}

    ////////////////////////////////////////////////////////////
    // Adds a named animation to the pwAnimation
    // 
    // \param AnimationName: ID, the animation should later be called with
    // \param Frames: Vector to set the start(x)- and end(y)-frame of the animation
    ////////////////////////////////////////////////////////////
	void AddAnimation(unsigned int AnimationID, const sf::Vector2i& Frames);
    void AddAnimation(unsigned int AnimationID, int xFrame, int yFrame);

    ////////////////////////////////////////////////////////////
    // Sets the current animation of the pwAnimation
    // 
    // \param AnimationName: ID of the animation, that is to be set as ccurrent
    ////////////////////////////////////////////////////////////
    void SetCurrentAnimation(unsigned int AnimationID);
    unsigned int GetCurrentAnimation()   {return myCurrentAnimationID;}
    int GetCurrentAnimationFrames()   {return myCurrentAnimation.y+1 - myCurrentAnimation.x;}

    ////////////////////////////////////////////////////////////
    // Resets the current animation of the pwAnimation
    ////////////////////////////////////////////////////////////
    void ResetCurrentAnimation();

    void AutoReset(bool reset)   {myAutoResetAnimation = reset;}

	virtual void update() const;

protected:

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    sf::Vector2u myFrameSize;       //< Size of every frame of the animation
    float        myFramesPerSecond;
    int          myTotalFrames;
    bool         myPlayOnce;
    sf::Vector2i myCurrentAnimation;
    unsigned int myCurrentAnimationID;
    std::map<unsigned int, sf::Vector2i> myAnimations;
    bool               myAutoResetAnimation;

    mutable bool       myWasPlayed;
    mutable float      myCurrentFrame;
    mutable sf::Sprite mySprite;
};

#endif

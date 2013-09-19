#include "..\include\pwStreamAnimation.hpp"

pwStreamAnimationLoader* pwStreamAnimationLoader::p_inst = NULL;


pwStreamAnimation::pwStreamAnimation()
{
}

pwStreamAnimation::~pwStreamAnimation()
{
}

pwStreamAnimation::pwStreamAnimation(std::string AnimationName, std::string Extension)
{
    setAnimation(AnimationName, Extension);
}

void pwStreamAnimation::setAnimation(std::string AnimationName, std::string Extension)
{
    myLoadData.FileName = AnimationName;
    myLoadData.ExtensionName = Extension;
}

void pwStreamAnimation::update() const
{
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

    mySprite.setTexture(*pwDataBox::get()->GetTexture(myLoadData.myFrameTextures.at((unsigned int)myCurrentFrame)), true);
}

////////////////////////////////////////////////////////////
// Updates the animation
////////////////////////////////////////////////////////////
void pwStreamAnimation::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    update();

    target.draw(mySprite, states);
}

void pwStreamAnimation::Load()
{
    myLoadData.TotalFrames = myTotalFrames;
    myLoadData.CurrentFrame = (unsigned int)myCurrentFrame;
    myLoadData.myFrameTextures.resize(myTotalFrames);

    std::stringstream typecast;
    std::string string;

    for (unsigned int i = 0; i < myLoadData.TotalFrames; i++)
    {
        typecast << std::setw(pwHelper::math::length32(myTotalFrames)) << std::setfill('0') << std::right << i;
        typecast >> string;
        typecast.clear();

        myLoadData.myFrameTextures[i] = myLoadData.FileName + string + myLoadData.ExtensionName;
    }
    
    pwStreamAnimationLoader::get()->Load(myLoadData.myFrameTextures, (int)myCurrentFrame);
}


pwStreamAnimationLoader* pwStreamAnimationLoader::get()
{
    if (p_inst == NULL)
    {
        p_inst = new pwStreamAnimationLoader;
    }

    return p_inst;
}

// a predicate implemented as a function:
bool pwStreamAnimationLoader::not_alive (std::pair<std::queue<std::string>, std::pair<sf::Thread*, bool>>& value) 
{
    if (value.second.second)
	{
        delete value.second.first;
        return true;
	}
	return false; 
}

void pwStreamAnimationLoader::CleanUp()
{
    StringList.remove_if(pwStreamAnimationLoader::not_alive);
}

void pwStreamAnimationLoader::Load(const std::vector<std::string>& LoadData, int BeginFrame)
{
    std::pair<std::queue<std::string>,  std::pair<sf::Thread*, bool>> TmpPair;
    std::list<std::pair<std::queue<std::string>, std::pair<sf::Thread*, bool>>>::iterator AddedIterator = StringList.insert(StringList.begin(), TmpPair);

    
    for (unsigned int i = (unsigned int)BeginFrame; i < LoadData.size(); i++)
    {
        AddedIterator->first.push(LoadData.at(i));
    }
    for (unsigned int i = 0; i < (unsigned int)BeginFrame; i++)
    {
        AddedIterator->first.push(LoadData.at(i));
    }

    AddedIterator->second.first = new sf::Thread(pwStreamAnimationLoader::PreloadStreamData, AddedIterator);
    AddedIterator->second.second = false;
    AddedIterator->second.first->launch();
}

void pwStreamAnimationLoader::PreloadStreamData(std::list<std::pair<std::queue<std::string>,  std::pair<sf::Thread*, bool>>>::iterator i)
{
    while(!i->first.empty())
    {
        pwDataBox::get()->GetTexture(i->first.front());
        i->first.pop();
    }
    
    i->second.second = true;
}
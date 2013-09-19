#ifndef PWSTREAMANIMATION
#define PWSTREAMANIMATION

#include <queue>
#include "pwAnimation.hpp"

class pwStreamAnimation : public pwAnimation
{
public:
    pwStreamAnimation();
    ~pwStreamAnimation();

    explicit pwStreamAnimation(std::string AnimationName, std::string Extension = ".png");

    void setAnimation(std::string AnimationName, std::string Extension = ".png");

    virtual void update() const;
    void Load();

    struct LoadData
    {
        std::string FileName;
        std::string ExtensionName;
        unsigned int TotalFrames;
        unsigned int CurrentFrame;
        std::vector<std::string> myFrameTextures;
    };

private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const;

    LoadData myLoadData;
};

class pwStreamAnimationLoader
{
public:
    pwStreamAnimationLoader() {}
    ~pwStreamAnimationLoader() {}

    static pwStreamAnimationLoader* get();

    void Load(const std::vector<std::string>& LoadData, int BeginFrame = 0);
    void CleanUp();

    static bool not_alive(std::pair<std::queue<std::string>, std::pair<sf::Thread*, bool>>& value);
    
private:
    static pwStreamAnimationLoader* p_inst;

    static void PreloadStreamData(std::list<std::pair<std::queue<std::string>, std::pair<sf::Thread*, bool>>>::iterator i);
    std::list<std::pair<std::queue<std::string>, std::pair<sf::Thread*, bool>>> StringList;
};

#endif
////////////////////////////////////////////////////////////
// Philipp Wittershagen
// 2012-17-07
// System implementation of the background 
// Uses pwGraphicsSystem-Layers -4 to -1
////////////////////////////////////////////////////////////

#ifndef BACKGROUND
#define BACKGROUND

#ifndef PWUSEGLOBALSINGLETONS
#define PWUSEGLOBALSINGLETONS
#endif

#include "..\pwSystem.hpp"
#include "CLevelData.h"

class CBackground
{
public:
    CBackground() {};
    ~CBackground() {};

    void init();
    void shutdown();

    void update();

    struct Element
    {
        sf::Texture* texture;
        unsigned int chance;
    };

    struct Layer
    {
        int layer;
        unsigned int max_chance;
        float timer;
        int y;
        int yVariance;
        bool start;
        bool animated;
        std::vector<Element> elements;
        std::list<sf::Sprite*> objects;
        float speed;
        float rate;
        float nextspawn;
        float variance;
    };

private:

    void draw();
    void update_this();

    std::vector<Layer> m_vLayer;
};

#endif
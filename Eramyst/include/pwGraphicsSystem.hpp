#ifndef PWGRAPHICSSYSTEM
#define PWGRAPHICSSYSTEM

#ifndef PWSYSTEM
#ifdef PWUSEGLOBALSINGLETONS
#define glGraphics pwGraphics::get()
#endif
#endif

class pwGameObject;
class pwCollisionCircle;

#include <utility>
#include <list>
#include <SFML\System.hpp>
#include <SFML\Graphics.hpp>
#include "pwLogSystem.hpp"
#include "pwEngineOptions.hpp"
#include "pwGameObject.hpp"
#include "pwGameState.hpp"
#include "pwGameLogics.hpp"

////////////////////////////////////////////////////////////////
// Manages all the graphics, done in the engine
////////////////////////////////////////////////////////////////
class pwGraphics
{

public:

    ////////////////////////////////////////////////////////////////
    // Default constructor
    ////////////////////////////////////////////////////////////////
    pwGraphics();

    ////////////////////////////////////////////////////////////////
    // Destructor
    ////////////////////////////////////////////////////////////////
    ~pwGraphics();

    ////////////////////////////////////////////////////////////////
    // Get the static instance of the pwGraphics
    //
    // \return Static instance of the pwGraphics
    ////////////////////////////////////////////////////////////////
    static pwGraphics* get();

    ////////////////////////////////////////////////////////////////
    // Initialize the graphics-system
    //
    // \return True, if initialisation was successful, false otherwise
    ////////////////////////////////////////////////////////////////
    bool Initialize();

    ////////////////////////////////////////////////////////////////
    // Shuts the graphics-system down
    //
    // \return True, if shutdown was successful, false otherwise
    ////////////////////////////////////////////////////////////////
	bool Shutdown();

    ////////////////////////////////////////////////////////////////
    // Updates the graphics-system
    //
    // \return True, if update was successful, false otherwise
    ////////////////////////////////////////////////////////////////
	bool Update();

    bool DrawPauseFrame();

    ////////////////////////////////////////////////////////////////
    // Adds a drawable to the render-vector myLayeredRenderObjects
    //
    // \return True, if update was successful, false otherwise
    ////////////////////////////////////////////////////////////////
    bool Draw(const sf::Drawable* DrawableObject, int Layer = 0, const sf::RenderStates &states = sf::RenderStates::Default);
    bool Draw(const pwGameObject* DrawableObject, int Layer = 0, const sf::RenderStates &states = sf::RenderStates::Default);
    bool DrawOverlay(const sf::Drawable* DrawableObject, int Layer = 0, const sf::RenderStates &states = sf::RenderStates::Default);
    bool DrawOverlay(const pwGameObject* DrawableObject, int Layer = 0, const sf::RenderStates &states = sf::RenderStates::Default);

    ////////////////////////////////////////////////////////////
    // Get a reference to the render window of the pwGraphics
    //
    // \return Reference to the render window
    ////////////////////////////////////////////////////////////
    sf::RenderWindow& GetRenderWindow();

    ////////////////////////////////////////////////////////////
    // Set the shader, thats used for the whole scene
    ////////////////////////////////////////////////////////////
    void AddGlobalShader(const sf::Shader* Shader, unsigned int ID);
    void RemoveGlobalShader(unsigned int ID);

    ////////////////////////////////////////////////////////////
    // Get the duration of the last rendered frame
    //
    // \return Duration of the last frame
    ////////////////////////////////////////////////////////////
    const float GetLastFrameTime()           {return myLastFrameTime;}

    void SetPaused(bool Paused)              {myDrawPaused = Paused;}

    sf::Vector2i getRelativeMousePos();
    const pwCollisionCircle* GetCursorCollisionObject() const;

    void ResetFrameTimer()                   {myFrameTimer.restart();}

    void SetClearColor(const sf::Color& c)   {myClearColor = c;}
    const sf::Color& GetClearColor()         {return myClearColor;}

    void ShakeScreen(float time, float distance, float speed = 100.0f);

    void ReRenderLastFrame()    {myReRenderOld = true;}


private:

    ////////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////////
    static pwGraphics* p_inst;
    bool             myInitialized;
    sf::RenderWindow myRenderWindow;
    sf::Clock        myFrameTimer;
    float            myLastFrameTime;
    sf::String       myFramesPerSecondString;
    bool             myDrawPaused;
    bool             myReRenderOld;

    struct pwScreenShake
    {
        float distance;
        float time_left;
        float current_pos;
        float speed;
        bool up;
    } myScreenShake;

    sf::Vector2f     myScale;
    sf::Vector2f     myPosition;
    sf::Color        myClearColor;

#ifndef PWATI
    sf::RenderTexture myTexture;
    sf::RenderTexture myTextureEx;
    bool              myUseEx;
#else
    sf::Texture       myTexture;
#endif

    sf::Sprite        mySprite;
    sf::Sprite        myPauseSprite;
    sf::RectangleShape myColoredBG;

    pwCollisionCircle* myCursorCollisionObject;

    std::map<unsigned int, const sf::Shader*> myShaderMap;

    struct pwDrawable
    {
        const sf::Drawable* drawable;
        sf::RenderStates states;
    };

    std::list<std::pair<int, pwDrawable>> myLayeredRenderObjects;
    std::list<std::pair<int, pwDrawable>> myLayeredAfterRenderObjects;
};

#endif
#include "..\include\pwGraphicsSystem.hpp"
#include "..\include\pwGraphicsLog.hpp"

////////////////////////////////////////////////////////////////
// Declaration of the static member variable
////////////////////////////////////////////////////////////////
pwGraphics* pwGraphics::p_inst = NULL;

////////////////////////////////////////////////////////////////
// Default constructor
////////////////////////////////////////////////////////////////
pwGraphics::pwGraphics() :
    
    myInitialized(false),
    myRenderWindow(),
    myFrameTimer(),
    myLastFrameTime(0.0f),
    myFramesPerSecondString(),
    myDrawPaused(false),
    myReRenderOld(false),

    myScale(0.0f, 0.0f),
    myPosition(0.0f, 0.0f),
    myClearColor(151, 151, 151)
{
    // Nothing to do
    myLastFrameTime = 0.0f;
    myCursorCollisionObject = new pwCollisionCircle;
}

////////////////////////////////////////////////////////////////
// Destructor
////////////////////////////////////////////////////////////////
pwGraphics::~pwGraphics()
{
    // Nothing to do
    delete myCursorCollisionObject;
}

////////////////////////////////////////////////////////////////
// Get the static instance of the pwGraphics
////////////////////////////////////////////////////////////////
pwGraphics* pwGraphics::get()
{
    // If the engine doesn't exist, create it
    if (!p_inst)
		p_inst = new pwGraphics;

    // Return the pointer to the engine
	return p_inst;
}

////////////////////////////////////////////////////////////////
// Initialize the graphics-system
////////////////////////////////////////////////////////////////
bool pwGraphics::Initialize()
{
    if (myInitialized)
        return true;

    myScale = sf::Vector2f(0.0f, 0.0f);
    myPosition = sf::Vector2f(0.0f, 0.0f);

    myScreenShake.current_pos = 0.0f;
    myScreenShake.distance = 0.0f;
    myScreenShake.time_left = 0.0f;
    myScreenShake.up = true;

    // Open window
    myRenderWindow.create(pwEngineOptions::GetCurrentSettings()->VideoMode, pwEngineOptions::GetCurrentSettings()->Title, pwEngineOptions::GetCurrentSettings()->WindowStyle);
    //myRenderWindow.setKeyRepeatEnabled(false);

#ifndef PWATI
    myTexture.create(pwEngineOptions::GetInternGraphicsSize().x, pwEngineOptions::GetInternGraphicsSize().y);
    myTextureEx.create(pwEngineOptions::GetInternGraphicsSize().x, pwEngineOptions::GetInternGraphicsSize().y);
    mySprite.setTexture(myTexture.getTexture());
    myUseEx = true;

    if (pwEngineOptions::GetVideoMode().width < pwEngineOptions::GetInternGraphicsSize().x ||
        pwEngineOptions::GetVideoMode().height < pwEngineOptions::GetInternGraphicsSize().y)
    {
        myTexture.setSmooth(true);
        myTextureEx.setSmooth(true);
    }

    myScale.x = (float)pwEngineOptions::GetCurrentSettings()->VideoMode.width / (float)pwEngineOptions::GetInternGraphicsSize().x;
    myScale.y = (float)pwEngineOptions::GetCurrentSettings()->VideoMode.height / (float)pwEngineOptions::GetInternGraphicsSize().y;
    myScale.x = pwHelper::math::min(myScale.x, myScale.y);
    myScale.y = myScale.x;

    myPosition = sf::Vector2f(((float)pwEngineOptions::GetCurrentSettings()->VideoMode.width - (float)myTexture.getSize().x * myScale.x) / 2.0f, ((float)pwEngineOptions::GetCurrentSettings()->VideoMode.height - (float)myTexture.getSize().y * myScale.y) / 2.0f);

    myColoredBG.setSize(sf::Vector2f((float)pwEngineOptions::GetCurrentSettings()->VideoMode.width, (float)pwEngineOptions::GetCurrentSettings()->VideoMode.height));
    myColoredBG.setFillColor(sf::Color(151, 151, 151, 151));

    myPauseSprite.setTexture(*pwDataBox::get()->GetTexture("default.png"));
    myPauseSprite.setOrigin(sf::Vector2f((float)myPauseSprite.getTexture()->getSize().x / 2.0f, (float)myPauseSprite.getTexture()->getSize().y / 2.0f));
    myPauseSprite.setPosition((float)pwEngineOptions::GetCurrentSettings()->VideoMode.width / 2.0f, (float)pwEngineOptions::GetCurrentSettings()->VideoMode.height / 2.0f);
#else
    myRenderWindow.setView(sf::View(sf::FloatRect(0.0f, 0.0f, (float)pwEngineOptions::GetInternGraphicsSize().x, (float)pwEngineOptions::GetInternGraphicsSize().y)));
    myTexture.create(pwEngineOptions::GetCurrentSettings()->VideoMode.width, pwEngineOptions::GetCurrentSettings()->VideoMode.height);
    mySprite.setTexture(myTexture, true);

    myScale.x = (float)pwEngineOptions::GetInternGraphicsSize().x / (float)pwEngineOptions::GetCurrentSettings()->VideoMode.width;
    myScale.y = (float)pwEngineOptions::GetInternGraphicsSize().y / (float)pwEngineOptions::GetCurrentSettings()->VideoMode.height;

    myColoredBG.setSize(sf::Vector2f((float)pwEngineOptions::GetInternGraphicsSize().x, (float)pwEngineOptions::GetInternGraphicsSize().y));
    myColoredBG.setFillColor(sf::Color(151, 151, 151, 151));

    myPauseSprite.setTexture(*pwDataBox::get()->GetTexture("default.png"));
    myPauseSprite.setOrigin(sf::Vector2f((float)myPauseSprite.getTexture()->getSize().x / 2.0f, (float)myPauseSprite.getTexture()->getSize().y / 2.0f));
    myPauseSprite.setPosition((float)pwEngineOptions::GetInternGraphicsSize().x / 2.0f, (float)pwEngineOptions::GetInternGraphicsSize().y / 2.0f);
#endif
    
    // Is window opened?
    if (myRenderWindow.isOpen())
        pwLog::get()->WriteLine("pwGraphics::Initialize: Creating the window was successful", pwLogType::pwLTInfo);
    else
    {
        pwLog::get()->WriteLine("pwGraphics::Initialize: An error occured at creating the window");
        return false;
    }

    // Clear the window after creation
    myRenderWindow.clear(sf::Color(151, 151, 151));
    myFrameTimer.restart();

    // Initialisation successful
    myInitialized = true;
    return true;
}

////////////////////////////////////////////////////////////////
// Shuts the graphics-system down
////////////////////////////////////////////////////////////////
bool pwGraphics::Shutdown()
{
    if (!myInitialized)
    {
        // Do nothing, if the graphics-system wasn't even started
        return false;
    }

    // Close window
    myRenderWindow.close();

    // Is window still open
    if (!myRenderWindow.isOpen())
        pwLog::get()->WriteLine("pwGraphics::Initialize: Closing the window was successful", pwLogType::pwLTInfo);
    else
    {
        pwLog::get()->WriteLine("pwGraphics::Initialize: An error occured at closing the window");
        return false;
    }

    // Shutdown successful
    myInitialized = false;
    return true;
}

////////////////////////////////////////////////////////////////
// Updates the graphics-system
////////////////////////////////////////////////////////////////
bool pwGraphics::Update()
{
    // Already initialized?
    if (!myInitialized)
    {
        pwLog::get()->WriteLine("pwGraphics::Update: The graphics-system has first to be initialized");
        return false;
    }

    if (pwEngineOptions::GetShowFramerateTitle())
    {
        std::stringstream TmpSStream;
        std::string TmpString;
        TmpSStream << (1.0f / myLastFrameTime);
        TmpSStream >> TmpString;
        myRenderWindow.setTitle(pwEngineOptions::GetWindowTitle() + " - " + TmpString);
    }

    if (!myDrawPaused)
    {
#ifndef PWATI
        if (!myReRenderOld)
        {
            myTexture.clear(myClearColor);
            myTextureEx.clear(myClearColor);
        }
#endif

   

        // Render all the objects in myLayeredRenderObjects after the layer-value
        while (!myLayeredRenderObjects.empty())
        {
            // Start at the beginning
            std::list<std::pair<int, pwDrawable>>::iterator i = myLayeredRenderObjects.begin();

            // Search for objects with smaller layer-value
            for (std::list<std::pair<int, pwDrawable>>::iterator j = myLayeredRenderObjects.begin(); j != myLayeredRenderObjects.end(); j++)
            {
                // Layer value is smaller?
                if (j->first < i->first)
                    // Then from now on compare with this one
                    i = j;
            }

            // Render the object
#ifndef PWATI
            myTexture.draw(*(i->second.drawable), i->second.states);
#else
            myRenderWindow.draw(*(i->second.drawable), i->second.states);
#endif

            // The object has been rendered: Erase it
            myLayeredRenderObjects.erase(i);
        }

        // Display the newly updated screen
#ifndef PWATI
            myTexture.display();
#else
            myTexture.update(myRenderWindow);
#endif

        if (!myReRenderOld)
        for (std::map<unsigned int, const sf::Shader*>::iterator i = myShaderMap.begin(); i != myShaderMap.end(); i++)
        {
#ifndef PWATI
            if (myUseEx)
            {
                myTextureEx.draw(mySprite, sf::RenderStates(i->second)); myTextureEx.display();
                mySprite.setTexture(myTextureEx.getTexture());
                myUseEx = false;
            }
            else
            {
                myTexture.draw(mySprite, sf::RenderStates(i->second)); myTexture.display();
                mySprite.setTexture(myTexture.getTexture());
                myUseEx = true;
            }
#else
            mySprite.setScale(myScale.x, myScale.y);
            myRenderWindow.draw(mySprite, sf::RenderStates(i->second)); //myRenderWindow.display();
            myTexture.update(myRenderWindow);
#endif
        }

#ifndef PWATI

        while (!myLayeredAfterRenderObjects.empty())
        {
            // Start at the beginning
            std::list<std::pair<int, pwDrawable>>::iterator i = myLayeredAfterRenderObjects.begin();

            // Search for objects with smaller layer-value
            for (std::list<std::pair<int, pwDrawable>>::iterator j = myLayeredAfterRenderObjects.begin(); j != myLayeredAfterRenderObjects.end(); j++)
            {
                // Layer value is smaller?
                if (j->first < i->first)
                    // Then from now on compare with this one
                    i = j;
            }

            if (myUseEx)
            {
                myTexture.draw(*(i->second.drawable), i->second.states);
            }
            else
            {
                myTextureEx.draw(*(i->second.drawable), i->second.states);
            }

            // The object has been rendered: Erase it
            myLayeredAfterRenderObjects.erase(i);
        }

        mySprite.setScale(myScale);
        mySprite.setPosition(sf::Vector2f(myPosition.x, myPosition.y + myScreenShake.current_pos * myScale.y));
        myRenderWindow.draw(mySprite);
        mySprite.setPosition(0.0f, 0.0f);
        mySprite.setScale(1.0f, 1.0f);
#else
        float w = 1.0f / myScale.x;
        float h = 1.0f / myScale.y;
        if (w < h)
        {
            mySprite.setScale(myScale.x, w * pwEngineOptions::GetInternGraphicsSize().y / pwEngineOptions::GetVideoMode().height * myScale.y);
        }
        else if (w > h)
        {
            mySprite.setScale(h * pwEngineOptions::GetInternGraphicsSize().x / pwEngineOptions::GetVideoMode().width * myScale.x, myScale.y);
        }
        else
        {
            mySprite.setScale(myScale.x, myScale.y);
        }
        myPosition = sf::Vector2f(((float)pwEngineOptions::GetInternGraphicsSize().x - (float)pwEngineOptions::GetVideoMode().width * mySprite.getScale().x) / 2.0f, ((float)pwEngineOptions::GetInternGraphicsSize().y - (float)pwEngineOptions::GetVideoMode().height * mySprite.getScale().y) / 2.0f + myScreenShake.current_pos);
        mySprite.setPosition(myPosition);
        myRenderWindow.clear();
        myRenderWindow.draw(mySprite);
        mySprite.setPosition(0.0f, 0.0f);
        mySprite.setScale(myScale.x, myScale.y);
#endif

        
    }
    else
    {
#ifndef PWATI
        mySprite.setScale(myScale);
        mySprite.setPosition(myPosition);
        myRenderWindow.draw(mySprite);
        mySprite.setPosition(0.0f, 0.0f);
        mySprite.setScale(1.0f, 1.0f);

        myRenderWindow.draw(myColoredBG);
        myRenderWindow.draw(myPauseSprite);
#else
        float w = 1.0f / myScale.x;
        float h = 1.0f / myScale.y;
        if (w < h)
        {
            mySprite.setScale(myScale.x, w * pwEngineOptions::GetInternGraphicsSize().y / pwEngineOptions::GetVideoMode().height * myScale.y);
        }
        else if (w > h)
        {
            mySprite.setScale(h * pwEngineOptions::GetInternGraphicsSize().x / pwEngineOptions::GetVideoMode().width * myScale.x, myScale.y);
        }
        else
        {
            mySprite.setScale(myScale.x, myScale.y);
        }
        myPosition = sf::Vector2f(((float)pwEngineOptions::GetInternGraphicsSize().x - (float)pwEngineOptions::GetVideoMode().width * mySprite.getScale().x) / 2.0f, ((float)pwEngineOptions::GetInternGraphicsSize().y - (float)pwEngineOptions::GetVideoMode().height * mySprite.getScale().y) / 2.0f + myScreenShake.current_pos);
        mySprite.setPosition(myPosition);
        myRenderWindow.clear();
        myRenderWindow.draw(mySprite);

        myPauseSprite.setScale(myScale.x, myScale.y);

        myRenderWindow.draw(myColoredBG);
        myRenderWindow.draw(myPauseSprite);

        mySprite.setPosition(0.0f, 0.0f);
        mySprite.setScale(myScale.x, myScale.y);
#endif

        myDrawPaused = false;
    }
    
    myRenderWindow.display();

    // Get the duration of the last frame
    myLastFrameTime = myFrameTimer.getElapsedTime().asSeconds();
    myFrameTimer.restart();

    // Update the CursorPosition
    myCursorCollisionObject->setPosition((sf::Vector2f)getRelativeMousePos());

    // Clear the screen after the update, to make custom rendering possible
#ifndef PWATI
    myRenderWindow.clear(sf::Color());
    myUseEx = true;
    mySprite.setTexture(myTexture.getTexture());
#else
    myRenderWindow.clear(myClearColor);
#endif

    if (myScreenShake.time_left > 0.0f)
    {
        if (myScreenShake.up)
        {
            myScreenShake.current_pos += myScreenShake.speed*GetLastFrameTime();

            if (myScreenShake.current_pos >= myScreenShake.distance)
            {
                myScreenShake.current_pos = myScreenShake.distance;
                myScreenShake.up = false;
            }
        }
        else
        {
            myScreenShake.current_pos -= myScreenShake.speed*GetLastFrameTime();

            if (myScreenShake.current_pos <= -myScreenShake.distance)
            {
                myScreenShake.current_pos = -myScreenShake.distance;
                myScreenShake.up = true;
            }
        }

        myScreenShake.time_left -= GetLastFrameTime();
    }
    else
    {
        myScreenShake.current_pos = 0.0f;
    }

    myReRenderOld = false;

    // No errors occured
    return true;
}

////////////////////////////////////////////////////////////////
// Adds a pwGameObject to the render-vector myLayeredRenderObjects
////////////////////////////////////////////////////////////////
bool pwGraphics::Draw(const pwGameObject* GameObject, int Layer, const sf::RenderStates &states)
{
    // Is object still alive?
    if (GameObject->GetAlive() == false)
        return false;

    return Draw((const sf::Drawable*)GameObject, Layer, states);
}

////////////////////////////////////////////////////////////////
// Adds a drawable to the render-vector myLayeredRenderObjects
////////////////////////////////////////////////////////////////
bool pwGraphics::Draw(const sf::Drawable* DrawableObject, int Layer, const sf::RenderStates &states)
{
    // Insert objects into a pair-object
    std::pair<int, pwDrawable> TempPair;
    TempPair.first = Layer;
    TempPair.second.drawable = DrawableObject;
    TempPair.second.states = states;
        
    // Insert the pair
    myLayeredRenderObjects.push_back(TempPair);

    // Nothing can go wrong
    return true;
}

////////////////////////////////////////////////////////////////
// Adds a pwGameObject to the render-vector myLayeredRenderObjects
////////////////////////////////////////////////////////////////
bool pwGraphics::DrawOverlay(const pwGameObject* GameObject, int Layer, const sf::RenderStates &states)
{
    // Is object still alive?
    if (GameObject->GetAlive() == false)
        return false;

    return DrawOverlay((const sf::Drawable*)GameObject, Layer, states);
}

////////////////////////////////////////////////////////////////
// Adds a drawable to the render-vector myLayeredRenderObjects
////////////////////////////////////////////////////////////////
bool pwGraphics::DrawOverlay(const sf::Drawable* DrawableObject, int Layer, const sf::RenderStates &states)
{
    // Insert objects into a pair-object
    std::pair<int, pwDrawable> TempPair;
    TempPair.first = Layer;
    TempPair.second.drawable = DrawableObject;
    TempPair.second.states = states;
        
    // Insert the pair
    myLayeredAfterRenderObjects.push_back(TempPair);

    // Nothing can go wrong
    return true;
}

////////////////////////////////////////////////////////////
// Get a reference to the render window of the pwGraphics
////////////////////////////////////////////////////////////
sf::RenderWindow& pwGraphics::GetRenderWindow()
{
    // Already initialized?
    if (!myInitialized)
    {
        pwLog::get()->WriteLine("pwGraphics::GetRenderWindow: The graphics-system has not been initialized yet");

        // Try to initialize
        if (!Initialize())
        {
            pwLog::get()->WriteLine("pwGraphics::GetRenderWindow: myGraphicsSystem could not be initialized");
            pwLog::get()->WriteLine("pwGraphics::GetRenderWindow: The returned sf::RenderWindow reference might not work correctly", pwLogType::pwLTCaution);
        }
        else
            pwLog::get()->WriteLine("pwGraphics::GetRenderWindow: Initializing the graphics-system automatically was successful", pwLogType::pwLTInfo);
    }

    return myRenderWindow;
}

void pwGraphics::AddGlobalShader(const sf::Shader* Shader, unsigned int ID)
{
    myShaderMap.insert(std::make_pair(ID, Shader));
}
void pwGraphics::RemoveGlobalShader(unsigned int ID)
{
    myShaderMap.erase(ID);
}

const pwCollisionCircle* pwGraphics::GetCursorCollisionObject() const
{
    return myCursorCollisionObject;
}

sf::Vector2i pwGraphics::getRelativeMousePos()
{
    sf::Vector2i RelativeMousePos = sf::Mouse::getPosition(myRenderWindow);

#ifndef PWATI

    float w = (float)pwEngineOptions::GetInternGraphicsSize().x / (float)pwEngineOptions::GetVideoMode().width;
    float h = (float)pwEngineOptions::GetInternGraphicsSize().y / (float)pwEngineOptions::GetVideoMode().height;
    sf::Vector2f TmpVector;
    if (w > h)
    {
        TmpVector.x = ((float)RelativeMousePos.x - myPosition.x) * w;
        TmpVector.y = ((float)RelativeMousePos.y - myPosition.y) * w;
    }
    else if (w < h)
    {
        TmpVector.x = ((float)RelativeMousePos.x - myPosition.x) * h;
        TmpVector.y = ((float)RelativeMousePos.y - myPosition.y) * h;
    }
    else
    {
        TmpVector.x = (float)RelativeMousePos.x * w;
        TmpVector.y = (float)RelativeMousePos.y * h;
    }
    
    return sf::Vector2i((int)TmpVector.x, (int)TmpVector.y);
#else
    float w = (float)pwEngineOptions::GetInternGraphicsSize().x / (float)pwEngineOptions::GetVideoMode().width;
    float h = (float)pwEngineOptions::GetInternGraphicsSize().y / (float)pwEngineOptions::GetVideoMode().height;
    sf::Vector2f TmpVector;
    if (w > h)
    {
        myPosition.x = 0.0f;
        myPosition.y = ((float)pwEngineOptions::GetVideoMode().height - (float)pwEngineOptions::GetVideoMode().width / (float)pwEngineOptions::GetInternGraphicsSize().x * (float)pwEngineOptions::GetInternGraphicsSize().y) / 2.0f;
        TmpVector.x = ((float)RelativeMousePos.x - myPosition.x) * w;
        TmpVector.y = ((float)RelativeMousePos.y - myPosition.y) * w;
    }
    else if (w < h)
    {
        myPosition.x = ((float)pwEngineOptions::GetVideoMode().width - (float)pwEngineOptions::GetVideoMode().height / (float)pwEngineOptions::GetInternGraphicsSize().y * (float)pwEngineOptions::GetInternGraphicsSize().x) / 2.0f;
        myPosition.y = 0.0f;
        TmpVector.x = ((float)RelativeMousePos.x - myPosition.x) * h;
        TmpVector.y = ((float)RelativeMousePos.y - myPosition.y) * h;
    }
    else
    {
        myPosition.x = 0.0f;
        myPosition.y = 0.0f;
        TmpVector.x = (float)RelativeMousePos.x * w;
        TmpVector.y = (float)RelativeMousePos.y * h;
    }
    return sf::Vector2i((int)TmpVector.x, (int)TmpVector.y);
#endif
}

void pwGraphics::ShakeScreen(float time, float distance, float speed)
{
    myScreenShake.time_left = time;
    myScreenShake.distance = distance;
    myScreenShake.speed = speed;
}
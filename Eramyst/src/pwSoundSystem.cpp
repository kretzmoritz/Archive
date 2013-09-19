#include "..\include\pwSoundSystem.hpp"
#include "..\include\pwLogSystem.hpp"

////////////////////////////////////////////////////////////////
// Declaration of the static member variable
////////////////////////////////////////////////////////////////
pwAudio* pwAudio::p_inst = NULL;

// Default constructor/destructor
pwSoundSettings::pwSoundSettings() :

    // Default values:
    myRelativeToListener(true),
    myLoop(false),
    myPitch(1.0f),
    myVolume(100.0f),
    myMinDistance(300.0f),
    myAttenuation(1.0f),
    myPosition(0.0f, 0.0f),
    myLayer(0)
{
    // Nothing to do
}
pwSoundSettings::~pwSoundSettings()
{
    // Nothing to do
}

// Set/get whether or not the sound on the layer should loop after reaching the end
pwSoundSettings* pwSoundSettings::SetLoop(bool Loop) 
{
    if (myLoop != Loop)
    {
        myLoop = Loop;
        pwAudio::get()->UpdateLayerSoundParameters(myLayer);
    }
    return this;
}
bool pwSoundSettings::GetLoop() const 
{
    return myLoop;
}

// Set/get the pitch of the sound-layer
pwSoundSettings* pwSoundSettings::SetPitch(float Pitch)
{
    if (myPitch != Pitch)
    {
        myPitch = Pitch;
        pwAudio::get()->UpdateLayerSoundParameters(myLayer);
    }
    return this;
}
float pwSoundSettings::GetPitch() const
{
    return myPitch;
}
      
// Set/get the volume of the sound-layer
pwSoundSettings* pwSoundSettings::SetVolume(float Volume)
{
    if (myVolume != Volume)
    {
        if (Volume > 100.0f)
            Volume = 100.0f;
        else if (Volume < 0.0f)
            Volume = 0.0f;

        myVolume = Volume;
        pwAudio::get()->UpdateLayerSoundParameters(myLayer);
    }
    return this;
}
float pwSoundSettings::GetVolume() const
{
    return myVolume;
}
 
// Set/get the position of the sound-layer
pwSoundSettings* pwSoundSettings::SetPosition(float PositionX, float PositionY, bool RelativeToListener)
{
    if ((myPosition.x != PositionX) || (myPosition.y != PositionY) || (myRelativeToListener != RelativeToListener))
    {
        myPosition.x = PositionX;
        myPosition.y = PositionY;
        myRelativeToListener = RelativeToListener;
        pwAudio::get()->UpdateLayerSoundParameters(myLayer);
    }
    return this;
}
pwSoundSettings* pwSoundSettings::SetPosition(sf::Vector2f Position, bool RelativeToListener)
{
    return SetPosition(Position.x, Position.y, RelativeToListener);
}
const sf::Vector2f& pwSoundSettings::GetPosition() const
{
    return myPosition;
}

// Set/get, if the position is relative to the listener
pwSoundSettings* pwSoundSettings::SetRelativeToListener(bool RelativeToListener)
{
    if (myRelativeToListener != RelativeToListener)
    {
        myRelativeToListener = RelativeToListener;
        pwAudio::get()->UpdateLayerSoundParameters(myLayer);
    }
    return this;
}
bool pwSoundSettings::GetRalativeToListener() const
{
    return myRelativeToListener;
}

// Set/get the minimum distance of the sound-layer
pwSoundSettings* pwSoundSettings::SetMinDistance(float MinDistance)
{
    if (myMinDistance != MinDistance)
    {
        myMinDistance = MinDistance;
        pwAudio::get()->UpdateLayerSoundParameters(myLayer);
    }
    return this;
}
float pwSoundSettings::GetMinDistance() const
{
    return myMinDistance;
}

// Set/get the attenuation factor of the sound-layer
pwSoundSettings* pwSoundSettings::SetAttenuation(float Attenuation)
{
    if (myAttenuation != Attenuation)
    {
        myAttenuation = Attenuation;
        pwAudio::get()->UpdateLayerSoundParameters(myLayer);
    }
    return this;
}
float  pwSoundSettings::GetAttenuation() const
{
    return myAttenuation;
}

// Default constructor/destructor
pwMusicSettings::pwMusicSettings() :
    pwSoundSettings(),
    myFade(true),
    myFadeTimeInv(1.0f)
{
    SetLoop(true);
}
pwMusicSettings::~pwMusicSettings()
{
    // Nothing to do
}

// Set/get whether or not the music on the layer should fade to the next one
pwMusicSettings* pwMusicSettings::SetFade(bool Fade)
{
    myFade = true;
    return this;
}
bool pwMusicSettings::GetFade() const
{
    return myFade;
}

// Set/get the fade-time of the layer
pwMusicSettings* pwMusicSettings::SetFadeTime(float FadeTime)
{
    if (FadeTime = 0.0f)
    {
        pwLog::get()->WriteLine("pwMusicSettings::SetFadeTime: FadeTime of 0.0f is not allowed -> myFade set off and FadeTime set to default", pwLogType::pwLTCaution);
        myFade = false;
        FadeTime = 1.0f;
    }
    else
    {
        myFadeTimeInv = 1.0f / FadeTime;
    }
    return this;
}
float pwMusicSettings::GetFadeTime() const
{
    return 1.0f / myFadeTimeInv;
}
float pwMusicSettings::GetFadeTimeInv() const
{
    return myFadeTimeInv;
}

////////////////////////////////////////////////////////////////
// Default constructor
////////////////////////////////////////////////////////////////
pwAudio::pwAudio() :
    myPaused(false),
    myInitialized(false)
{
    // Nothing to do
}

////////////////////////////////////////////////////////////////
// Destructor
////////////////////////////////////////////////////////////////
pwAudio::~pwAudio()
{
    // Nothing to do
}

////////////////////////////////////////////////////////////////
// Get the static instance of the pwAudio
////////////////////////////////////////////////////////////////
pwAudio* pwAudio::get()
{
    // If the engine doesn't exist, create it
    if (!p_inst)
		p_inst = new pwAudio;

    // Return the pointer to the engine
	return p_inst;
}

////////////////////////////////////////////////////////////////
// Initialize the audio-system
////////////////////////////////////////////////////////////////
bool pwAudio::Initialize()
{
    myLayeredSoundObjects.resize(pwEngineOptions::GetCurrentSettings()->SoundLayerCount);
    myLayeredSoundSettings.resize(pwEngineOptions::GetCurrentSettings()->SoundLayerCount);

    myLayeredMusicObjects.resize(pwEngineOptions::GetCurrentSettings()->MusicLayerCount*2, NULL);
    myLayeredMusicSettings.resize(pwEngineOptions::GetCurrentSettings()->MusicLayerCount);

    for (unsigned int i = 0; i < myLayeredSoundSettings.size(); i++)
    {
        myLayeredSoundSettings[i].SetLayer(i);
    }
    for (unsigned int i = 0; i < myLayeredMusicSettings.size(); i++)
    {
        myLayeredMusicSettings[i].SetLayer(i);
    }

    sf::Listener::setGlobalVolume(pwEngineOptions::GetCurrentSettings()->GlobalVolume);
    sf::Listener::setPosition((float)pwEngineOptions::GetInternGraphicsSize().x / 2.0f, -300.0f, (float)pwEngineOptions::GetInternGraphicsSize().y / 2.0f);

    // Initialisation successful
    myInitialized = true;
    return true;
}

////////////////////////////////////////////////////////////////
// Shuts the audio-system down
////////////////////////////////////////////////////////////////
bool pwAudio::Shutdown()
{
    if (!myInitialized)
    {
        // Do nothing, if the audio-system wasn't even started
        return false;
    }


    // Free all the Memory, that's still occupied
    for (std::vector<std::list<sf::Sound*>>::iterator i = myLayeredSoundObjects.begin(); i != myLayeredSoundObjects.end(); i++)
    {
        for (std::list<sf::Sound*>::iterator j = i->begin(); j != i->end(); j++)
        {
            delete (*j);
        }
        i->clear();
    }
    for (std::vector<sf::Music*>::iterator i = myLayeredMusicObjects.begin(); i != myLayeredMusicObjects.end(); i++)
    {
        if ((*i) != NULL)
        {
            delete (*i);
            (*i) = NULL;
        }
    }

    // Shutdown successful
    myInitialized = false;
    return true;
}

// a predicate implemented as a function:
bool pwAudio::not_alive (sf::Sound* value) 
{
    if (value->getStatus() == value->Stopped)
	{
        value->stop();
        delete value;
        return true;
	}
	return false; 
}

////////////////////////////////////////////////////////////////
// Update - Only used when fading on music-layer
////////////////////////////////////////////////////////////////
bool pwAudio::Update()
{
    // Already initialized?
    if (!myInitialized)
    {
        pwLog::get()->WriteLine("pwAudio::Update: The audio-system has first to be initialized");
        return false;
    }

    if (myPaused)
        return true;

    for (std::vector<std::list<sf::Sound*>>::iterator i = myLayeredSoundObjects.begin(); i != myLayeredSoundObjects.end(); i++)
    {
        i->remove_if(pwAudio::not_alive);
    }

    for (unsigned int i = 0; i < pwEngineOptions::GetCurrentSettings()->MusicLayerCount; i++)
    {
        if (myLayeredMusicSettings.at(i).GetFade())
        {
            if (myLayeredMusicObjects.at(i*2) != NULL && myLayeredMusicObjects.at(i*2)->getStatus() == sf::Music::Playing &&
                myLayeredMusicObjects.at(i*2+1) != NULL && myLayeredMusicObjects.at(i*2+1)->getStatus() == sf::Music::Playing)
            {
                float dV = 100.0f * myLayeredMusicSettings.at(i).GetFadeTimeInv() * pwGraphics::get()->GetLastFrameTime();
                float NewVolume = myLayeredMusicObjects.at(i*2)->getVolume() - dV;
                float NewVolumeTo = myLayeredMusicObjects.at(i*2+1)->getVolume() + dV;

                if (NewVolume <= 0.0f)
                {
                    myLayeredMusicObjects.at(i*2)->stop();
                    myLayeredMusicObjects.at(i*2) = myLayeredMusicObjects.at(i*2+1);
                    myLayeredMusicObjects.at(i*2+1) = NULL;
                    myLayeredMusicObjects.at(i*2)->setVolume(100.0f);
                }
                else
                {
                    myLayeredMusicObjects.at(i*2)->setVolume(NewVolume);
                    myLayeredMusicObjects.at(i*2+1)->setVolume(NewVolumeTo);
                }
            }
        }
        else if (myLayeredMusicObjects.at(i*2+1) != NULL)
        {
            myLayeredMusicObjects.at(i*2+1)->stop();
        }
    }

    return true;
}

////////////////////////////////////////////////////////////////
// Plays a sound on the defined Layer (0 to SoundLayerCount-1)
////////////////////////////////////////////////////////////////
bool pwAudio::PlaySound(const sf::SoundBuffer* SoundBuffer, unsigned int Layer)
{
    // Already initialized?
    if (!myInitialized)
    {
        pwLog::get()->WriteLine("pwAudio::PlaySound: The audio-system has first to be initialized");
        return false;
    }

    // Layer available
    if (Layer >= myLayeredSoundObjects.size())
    {
        pwLog::get()->WriteLine("pwAudio::PlaySound: The layer is not available");
        return false;
    }

    sf::Sound* TmpSound = new sf::Sound(*SoundBuffer);
    myLayeredSoundObjects[Layer].push_back(TmpSound);
    SetLayerSoundParameters(TmpSound, Layer)->play();

    return true;
}
bool pwAudio::PlayMusic(sf::Music* Music, unsigned int Layer)
{
    // Already initialized?
    if (!myInitialized)
    {
        pwLog::get()->WriteLine("pwAudio::PlayMusic: The audio-system has first to be initialized");
        return false;
    }

    // Layer available
    if ((Layer*2+1) >= myLayeredMusicObjects.size())
    {
        pwLog::get()->WriteLine("pwAudio::PlayMusic: The layer is not available");
        return false;
    }

    // Layer free, already stopped, or fading is off?
    if (!myLayeredMusicSettings.at(Layer).GetFade() || myLayeredMusicObjects.at(Layer*2) == NULL || myLayeredMusicObjects.at(Layer*2)->getStatus() == sf::Music::Stopped)
    {
        if (myLayeredMusicObjects.at(Layer*2) != NULL)
        {
            myLayeredMusicObjects.at(Layer*2)->stop();
        }
        if (myLayeredMusicObjects.at(Layer*2+1) != NULL)
        {
            myLayeredMusicObjects.at(Layer*2+1)->stop();
        }

        SetLayerMusicParameters(myLayeredMusicObjects[Layer*2] = Music, Layer)->play();
        return true;
    }
    // Fading on?
    else
    {
        if (myLayeredMusicObjects.at(Layer*2+1) != NULL)
        {
            myLayeredMusicObjects.at(Layer*2+1)->stop();
        }

        SetLayerMusicParameters(myLayeredMusicObjects[Layer*2+1] = Music, Layer)->play();
        Music->setVolume(0.0f);
        return true;
    }

    return false;
}

////////////////////////////////////////////////////////////////
// Returns the pointer to the music on the defined Layer (0 to MusicLayerCount-1)
// - Use this with caution: Pointer may be invalid
////////////////////////////////////////////////////////////////
sf::Music* pwAudio::GetMusic(unsigned int Layer)
{
    // Already initialized?
    if (!myInitialized)
    {
        pwLog::get()->WriteLine("pwAudio::GetMusic: The audio-system has first to be initialized");
        return NULL;
    }

    // Layer available
    if (Layer >= myLayeredMusicObjects.size() / 2)
    {
        pwLog::get()->WriteLine("pwAudio::GetMusic: The layer is not available");
        return NULL;
    }

    if (myLayeredMusicObjects.at(Layer*2+1) != NULL)
    {
        return myLayeredMusicObjects.at(Layer*2+1);
    }
    else if (myLayeredMusicObjects.at(Layer*2) != NULL)
    {
        return myLayeredMusicObjects.at(Layer*2);
    }

    return NULL;
}

////////////////////////////////////////////////////////////////
// Capsulation of the sf::Listener
////////////////////////////////////////////////////////////////
void pwAudio::SetListenerPosition(const sf::Vector2f& Position)
{
    sf::Listener::setPosition(Position.x, 0.0f, Position.y);
}
void pwAudio::SetListenerDirection(const sf::Vector2f& Direction)
{
    sf::Listener::setDirection(Direction.x, 0.0f, Direction.y);
}
void pwAudio::SetListenerTargetPosition(const sf::Vector2f& TargetPos)
{
    sf::Listener::setDirection(sf::Listener::getPosition().x - TargetPos.x, 0.0f, sf::Listener::getPosition().z - TargetPos.y);
}
void pwAudio::SetListenerDistance(float Distance)
{
    sf::Listener::setPosition(sf::Listener::getPosition().x, Distance, sf::Listener::getPosition().z);
}
void pwAudio::ResetListener()
{
    sf::Listener::setPosition(pwEngineOptions::GetCurrentSettings()->VideoMode.width / 2.0f, -300.0f, pwEngineOptions::GetCurrentSettings()->VideoMode.height / 2.0f);
    sf::Listener::setDirection(0.0f, 0.0f, -1.0f);
}

////////////////////////////////////////////////////////////////
// Set defualt parameters of the defined layer to the given sound
////////////////////////////////////////////////////////////////
sf::Sound* pwAudio::SetLayerSoundParameters(sf::Sound* Sound, unsigned int Layer)
{
    // Layer available
    if (Layer >= myLayeredSoundSettings.size())
    {
        pwLog::get()->WriteLine("pwAudio::SetLayerSoundParameters: The layer is not available");
        return NULL;
    }

    const pwSoundSettings* TmpLayer = &myLayeredSoundSettings.at(Layer);
    Sound->setAttenuation(TmpLayer->GetAttenuation());
    Sound->setLoop(TmpLayer->GetLoop());
    Sound->setMinDistance(TmpLayer->GetMinDistance());
    Sound->setPitch(TmpLayer->GetPitch());
    Sound->setPosition(TmpLayer->GetPosition().x, 0.0f, TmpLayer->GetPosition().y);
    Sound->setRelativeToListener(TmpLayer->GetRalativeToListener());
    Sound->setVolume(TmpLayer->GetVolume());

    return Sound;
}
void pwAudio::UpdateLayerSoundParameters(unsigned int Layer)
{
    // Layer available
    if (Layer >= myLayeredSoundObjects.size())
    {
        pwLog::get()->WriteLine("pwAudio::UpdateLayerSoundParameters: The layer is not available");
        return;
    }
    
    // Layer free?
    for (std::list<sf::Sound*>::iterator i = myLayeredSoundObjects.at(Layer).begin(); i != myLayeredSoundObjects.at(Layer).end(); i++)
    {
        SetLayerSoundParameters(*i, Layer);
    }
}
sf::Music* pwAudio::SetLayerMusicParameters(sf::Music* Music, unsigned int Layer)
{
    // Layer available
    if (Layer >= myLayeredMusicSettings.size())
    {
        pwLog::get()->WriteLine("pwAudio::SetLayerMusicParameters: The layer is not available");
        return NULL;
    }

    const pwSoundSettings* TmpLayer = &myLayeredMusicSettings.at(Layer);
    Music->setAttenuation(TmpLayer->GetAttenuation());
    Music->setLoop(TmpLayer->GetLoop());
    Music->setMinDistance(TmpLayer->GetMinDistance());
    Music->setPitch(TmpLayer->GetPitch());
    Music->setPosition(TmpLayer->GetPosition().x, 0.0f, TmpLayer->GetPosition().y);
    Music->setRelativeToListener(TmpLayer->GetRalativeToListener());
    Music->setVolume(TmpLayer->GetVolume());

    return Music;
}
void pwAudio::UpdateLayerMusicParameters(unsigned int Layer)
{
    // Layer available
    if (Layer >= myLayeredMusicObjects.size())
    {
        pwLog::get()->WriteLine("pwAudio::UpdateLayerMusicParameters: The layer is not available");
        return;
    }
    
    // Layer free?
    if (myLayeredMusicObjects.at(Layer) != NULL)
    {
        SetLayerMusicParameters(myLayeredMusicObjects.at(Layer), Layer);
    }
}

pwSoundSettings* pwAudio::GetSoundLayerSettings(unsigned int Layer)
{
    // Layer available
    if (Layer >= myLayeredSoundSettings.size())
    {
        pwLog::get()->WriteLine("pwAudio::GetSoundLayerSettings: The layer is not available");
        return NULL;
    }
    else
    {
        return &myLayeredSoundSettings.at(Layer);
    }
}
pwMusicSettings* pwAudio::GetMusicLayerSettings(unsigned int Layer)
{
    // Layer available
    if (Layer >= myLayeredMusicSettings.size())
    {
        pwLog::get()->WriteLine("pwAudio::GetMusicLayerSettings: The layer is not available");
        return NULL;
    }
    else
    {
        return &myLayeredMusicSettings.at(Layer);
    }
}

void pwAudio::Pause()
{
    for (std::vector<std::list<sf::Sound*>>::iterator i = myLayeredSoundObjects.begin(); i != myLayeredSoundObjects.end(); i++)
    {
        for (std::list<sf::Sound*>::iterator j = i->begin(); j != i->end(); j++)
        {
            if (*j != NULL && (*j)->getStatus() != (*j)->Stopped)
            {
                (*j)->pause();
            }    
        }
    }
    for (std::vector<sf::Music*>::iterator i = myLayeredMusicObjects.begin(); i != myLayeredMusicObjects.end(); i++)
    {
        if (*i != NULL && (*i)->getStatus() != (*i)->Stopped)
        {
            (*i)->pause();
        }
    }

    myPaused = true;
}

void pwAudio::Resume()
{
    for (std::vector<std::list<sf::Sound*>>::iterator i = myLayeredSoundObjects.begin(); i != myLayeredSoundObjects.end(); i++)
    {
        for (std::list<sf::Sound*>::iterator j = i->begin(); j != i->end(); j++)
        {
            if (*j != NULL && (*j)->getStatus() != (*j)->Stopped)
            {
                (*j)->play();
            }    
        }
    }
    for (std::vector<sf::Music*>::iterator i = myLayeredMusicObjects.begin(); i != myLayeredMusicObjects.end(); i++)
    {
        if (*i != NULL && (*i)->getStatus() != (*i)->Stopped)
        {
            (*i)->play();
        }
    }

    myPaused = false;
}
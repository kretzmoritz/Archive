#ifndef PWSOUNDSYSTEM
#define PWSOUNDSYSTEM

#ifndef PWSYSTEM
#ifdef PWUSEGLOBALSINGLETONS
#define glAudio pwAudio::get()
#endif
#endif

#include <utility>
#include <list>
#include <SFML\System.hpp>
#include <SFML\Audio.hpp>
#include "pwEngineOptions.hpp"
#include "pwGameObject.hpp"
#include "pwDataBox.hpp"

class pwAudio;

class pwSoundSettings
{
    friend pwAudio;

public:

    // Default constructor/destructor
    pwSoundSettings();
    ~pwSoundSettings();

    // Set/get whether or not the sound on the layer should loop after reaching the end
    pwSoundSettings* SetLoop(bool Loop);
    bool GetLoop() const;

    // Set/get the pitch of the sound-layer
    pwSoundSettings* SetPitch(float Pitch);
    float GetPitch() const;
      
    // Set/get the volume of the sound-layer
    pwSoundSettings* SetVolume(float Volume);
    float GetVolume() const;
 
    // Set/get the position of the sound-layer
    pwSoundSettings* SetPosition(float PositionX, float PositionY, bool RelativeToListener = false);
    pwSoundSettings* SetPosition(sf::Vector2f Position, bool RelativeToListener = false);
    const sf::Vector2f& GetPosition () const;

    // Set/get, if the position is relative to the listener
    pwSoundSettings* SetRelativeToListener(bool RelativeToListener = false);
    bool GetRalativeToListener() const;

    // Set/get the minimum distance of the sound-layer
    pwSoundSettings* SetMinDistance(float MinDistance);
    float GetMinDistance() const;

    // Set/get the attenuation factor of the sound-layer
    pwSoundSettings* SetAttenuation(float Attenuation);
    float GetAttenuation() const;

protected:
    void SetLayer(unsigned int Layer)   {myLayer = Layer;}
    
private:

    bool myRelativeToListener;
    bool myLoop;
    float myPitch;
    float myVolume;
    float myMinDistance;
    float myAttenuation;
    sf::Vector2f myPosition;

    unsigned int myLayer;
};

class pwMusicSettings : public pwSoundSettings
{
    friend pwAudio;

public:

    // Default constructor/destructor
    pwMusicSettings();
    ~pwMusicSettings();

    // Set/get whether or not the music on the layer should fade to the next one
    pwMusicSettings* SetFade(bool Fade);
    bool GetFade() const;

    // Set/get the fade-time of the layer
    pwMusicSettings* SetFadeTime(float FadeTime);
    float GetFadeTime() const;
    float GetFadeTimeInv() const;
    
private:

    bool myFade;
    float myFadeTimeInv;
};

////////////////////////////////////////////////////////////////
// Manages all the audio, done in the engine
////////////////////////////////////////////////////////////////
class pwAudio
{

public:

    ////////////////////////////////////////////////////////////////
    // Default constructor
    ////////////////////////////////////////////////////////////////
    pwAudio();

    ////////////////////////////////////////////////////////////////
    // Destructor
    ////////////////////////////////////////////////////////////////
    ~pwAudio();

    ////////////////////////////////////////////////////////////////
    // Get the static instance of the pwAudio
    //
    // \return Static instance of the pwAudio
    ////////////////////////////////////////////////////////////////
    static pwAudio* get();

    ////////////////////////////////////////////////////////////////
    // Initialize the audio-system
    //
    // \return True, if initialisation was successful, false otherwise
    ////////////////////////////////////////////////////////////////
    bool Initialize();

    ////////////////////////////////////////////////////////////////
    // Shuts the audio-system down
    //
    // \return True, if shutdown was successful, false otherwise
    ////////////////////////////////////////////////////////////////
	bool Shutdown();

    ////////////////////////////////////////////////////////////////
    // Update - Only used when fading on music-layer
    ////////////////////////////////////////////////////////////////
    bool Update();

    ////////////////////////////////////////////////////////////////
    // Plays a sound on the defined Layer (0 to SoundLayerCount-1)
    //
    // \return True, if no sound was playing on the layer, false otherwise
    ////////////////////////////////////////////////////////////////
    bool PlaySound(const sf::SoundBuffer* SoundBuffer, unsigned int Layer = 0);
    bool PlayMusic(sf::Music* Music, unsigned int Layer = 0);

    ////////////////////////////////////////////////////////////////
    // Returns the pointer to the sound on the defined Layer (0 to SoundLayerCount-1)
    // - Use this with caution: Pointer may be invalid
    //
    // \return A Pointer to the Sound on a defined layer, NULL, if no sound is playing on the layer
    ////////////////////////////////////////////////////////////////
    sf::Sound* GetSound(unsigned int Layer = 0);
    sf::Music* GetMusic(unsigned int Layer = 0);

    ////////////////////////////////////////////////////////////////
    // Capsulation of the sf::Listener
    ////////////////////////////////////////////////////////////////
    void SetListenerPosition(const sf::Vector2f& Position);
    void SetListenerDirection(const sf::Vector2f& Direction);
    void SetListenerTargetPosition(const sf::Vector2f& TargetPos);
    void SetListenerDistance(float Distance);
    void ResetListener();

    ////////////////////////////////////////////////////////////////
    // Set default parameters of the defined layer to the given sound
    ////////////////////////////////////////////////////////////////
    sf::Sound* SetLayerSoundParameters(sf::Sound* Sound, unsigned int Layer = 0);
    void UpdateLayerSoundParameters(unsigned int Layer = 0);

    sf::Music* SetLayerMusicParameters(sf::Music* Music, unsigned int Layer = 0);
    void UpdateLayerMusicParameters(unsigned int Layer = 0);

    //Get a pointer to the pwSoundSettings of the given layer
    pwSoundSettings* GetSoundLayerSettings(unsigned int Layer = 0);
    pwMusicSettings* GetMusicLayerSettings(unsigned int Layer = 0);

    static bool not_alive (sf::Sound* value);

    void Pause();
    void Resume();


private:

    ////////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////////
    static pwAudio*  p_inst;
    bool             myInitialized;
    bool             myPaused;
    std::vector<std::list<sf::Sound*>> myLayeredSoundObjects;
    std::vector<sf::Music*> myLayeredMusicObjects;
    std::vector<pwSoundSettings> myLayeredSoundSettings;
    std::vector<pwMusicSettings> myLayeredMusicSettings;
};

#endif
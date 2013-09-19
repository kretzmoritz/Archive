#ifndef PWENGINEOPTIONS
#define PWENGINEOPTIONS

#include <string>
#include <SFML\Graphics.hpp>
#include <SFML\Audio.hpp>
#include "pwIniFile.hpp"
#include "pwSoundSystem.hpp"
#include "pwGraphicsSystem.hpp"

////////////////////////////////////////////////////////////////
// Contains all the options of the engine
////////////////////////////////////////////////////////////////
struct pwEngineSettings
{
    // Video / Window Settings
    sf::VideoMode VideoMode;
    
    sf::Vector2u InternSize;

    unsigned long WindowStyle;
    std::string Title;
    bool ShowFramerateTitle;

    // Sound Settings
    unsigned int SoundLayerCount;
    unsigned int MusicLayerCount;
    float GlobalVolume;
};

////////////////////////////////////////////////////////////////
// Reads and writes the options
////////////////////////////////////////////////////////////////
class pwEngineOptions
{

public:

    ////////////////////////////////////////////////////////////////
    // Default constructor
    ////////////////////////////////////////////////////////////////
    pwEngineOptions();

    ////////////////////////////////////////////////////////////////
    // Destructor
    ////////////////////////////////////////////////////////////////
    ~pwEngineOptions();

    ////////////////////////////////////////////////////////////////
    // Load settings from the options-file
    //
    // \return True, if loading the settings was successful, false otherwise
    ////////////////////////////////////////////////////////////////
    static bool Initialize();

    ////////////////////////////////////////////////////////////////
    // Weites the new settings to the options-file
    //
    // \return True, if writing the settings was successful, false otherwise
    ////////////////////////////////////////////////////////////////
	static bool Shutdown();

    ////////////////////////////////////////////////////////////////
    // Get the current Settings
    //
    // \return pwEngineSettings struct with all important options set
    ////////////////////////////////////////////////////////////////
    static const pwEngineSettings* GetCurrentSettings();


    //
    // Video / Window Settings
    static void SetVideoMode(const sf::VideoMode& VideoMode);
    static const sf::VideoMode& GetVideoMode();

    static void SetInternGraphicsSize(const sf::Vector2u& GraphicsSize);
    static const sf::Vector2u& GetInternGraphicsSize();

    static bool GetScaled();

    static void SetWindowStyle(unsigned long WindowStyle);
    static unsigned long GetWindowStyle();

    static void SetWindowTitle(const std::string& Title = "pwEngine");
    static const std::string& GetWindowTitle();

    static void SetShowFramerateTitle(bool ShowFramerate = false);
    static bool GetShowFramerateTitle();

    // Sound Settings
    static void SetSoundLayerCount(unsigned int LayerCount);
    static unsigned int GetSoundLayerCount();
    
    static void SetMusicLayerCount(unsigned int LayerCount);
    static unsigned int GetMusicLayerCount();

    static void SetGlobalVolume(float Volume);
    static float GetGlobalVolume();
    //


    // Log Settings
    static bool LogEnabled;

private:

    ////////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////////
    static pwEngineSettings myEngineSettings;
    static bool             myInitialized;
    static bool             myChanged;
};

#endif
#include "..\include\pwEngineOptions.hpp"

////////////////////////////////////////////////////////////////
// Declaration of the static member variables
////////////////////////////////////////////////////////////////
pwEngineSettings pwEngineOptions::myEngineSettings;
bool             pwEngineOptions::myInitialized = false;
bool             pwEngineOptions::myChanged = false;
bool             pwEngineOptions::LogEnabled = true;

////////////////////////////////////////////////////////////////
// Default constructor
////////////////////////////////////////////////////////////////
pwEngineOptions::pwEngineOptions()
{
    // Nothing to do
}


////////////////////////////////////////////////////////////////
// Destructor
////////////////////////////////////////////////////////////////
pwEngineOptions::~pwEngineOptions()
{
    // Nothing to do
}

////////////////////////////////////////////////////////////////
// Load settings from the options-file
//
// \return True, if loading the settings was successful, false otherwise
////////////////////////////////////////////////////////////////
bool pwEngineOptions::Initialize()
{
    if (myInitialized)
        return true;

    myChanged = false;

    // Define default options
    myEngineSettings.VideoMode = sf::VideoMode::getDesktopMode();
    myEngineSettings.WindowStyle = sf::Style::Close | sf::Style::Fullscreen;
    myEngineSettings.Title = "pwEngine";
    myEngineSettings.ShowFramerateTitle = true;
    myEngineSettings.SoundLayerCount = 32;
    myEngineSettings.MusicLayerCount = 3;
    myEngineSettings.GlobalVolume = 100.0f;

    // Temporary variables for reading in the options
    std::string TmpString;
    int         TmpInt;
    unsigned int TmpUInt;
    float       TmpFloat;
    bool        TmpBool;
    pwIniFile   IniFile;

    // Open the options-file in the pwIniReader
    if (IniFile.LoadFromFile("options.ini"))
    {
        // Get the width
        if (IniFile.GetData(&TmpInt, "width", "graphics"))
        {
            myEngineSettings.VideoMode.width = TmpInt;
        } else {
            myChanged = true;
        }

        // Get the height
        if (IniFile.GetData(&TmpInt, "height", "graphics"))
        {
            myEngineSettings.VideoMode.height = TmpInt;
        } else {
            myChanged = true;
        }

        // Get the bits per pixel
        if (IniFile.GetData(&TmpInt, "bpp", "graphics") && (TmpInt == 16 || TmpInt == 32))
        {
            myEngineSettings.VideoMode.bitsPerPixel = TmpInt;
        } else {
            myChanged = true;
        }

        // Fullscreen?
        if (IniFile.GetData(&TmpBool, "fullscreen", "graphics"))
        {
            if (TmpBool)
                myEngineSettings.WindowStyle |= sf::Style::Fullscreen; // Sets fullscreen on
            else
                myEngineSettings.WindowStyle &= sf::Style::Close | sf::Style::Resize | sf::Style::Titlebar; // Sets fullscreen off
        } else {
            myChanged = true;
        }

        // Append the current framerate to the window-title
        if (IniFile.GetData(&TmpBool, "framerate_title", "graphics"))
        {
            myEngineSettings.ShowFramerateTitle = TmpBool;
        } else {
            myChanged = true;
        }

        // Get the sound layer count
        if (IniFile.GetData(&TmpUInt, "soundlayers", "audio"))
        {
            myEngineSettings.SoundLayerCount = TmpUInt;
        } else {
            myChanged = true;
        }

        // Get the music layer count
        if (IniFile.GetData(&TmpUInt, "musiclayers", "audio"))
        {
            myEngineSettings.MusicLayerCount = TmpUInt;
        } else {
            myChanged = true;
        }

        // Get the global volume
        if (IniFile.GetData(&TmpFloat, "volume", "audio") && (TmpFloat >= 0 && TmpFloat <= 100))
        {
            myEngineSettings.GlobalVolume = TmpFloat;
        } else {
            myChanged = true;
        }
    }
    else
    {
        // Caution: Default values are used
        myChanged = true;
    }

    if ((myEngineSettings.WindowStyle & sf::Style::Fullscreen) == sf::Style::Fullscreen)
    {
        if (!myEngineSettings.VideoMode.isValid())
        {
            myEngineSettings.VideoMode = sf::VideoMode::getDesktopMode();
            myChanged = true;
        }
    }
    else
    {
        if (myEngineSettings.VideoMode.width > sf::VideoMode::getDesktopMode().width ||
            myEngineSettings.VideoMode.height > sf::VideoMode::getDesktopMode().height)
        {
            myEngineSettings.VideoMode = sf::VideoMode::getDesktopMode();
            myChanged = true;
        }
    }

    myInitialized = true;
    return true;
}

////////////////////////////////////////////////////////////////
// Writes the new settings to the options-file
//
// \return True, if writing the settings was successful, false otherwise
////////////////////////////////////////////////////////////////
bool pwEngineOptions::Shutdown()
{
    if (!myInitialized || !myChanged)
    {
        // Do nothing, if the logging-system wasn't even started
        return true;
    }

    pwIniFile IniFile;

    // Write the width
    IniFile.AddData(myEngineSettings.VideoMode.width, "width", "graphics");

    // Write the height
    IniFile.AddData(myEngineSettings.VideoMode.height, "height", "graphics");

    // Write the bits per pixel
    IniFile.AddData(myEngineSettings.VideoMode.bitsPerPixel, "bpp", "graphics");

    // Write, if fullscreen?
    IniFile.AddData(((myEngineSettings.WindowStyle & sf::Style::Fullscreen) == sf::Style::Fullscreen), "fullscreen", "graphics");

    // Write, if the framerate should be appended to the window-title
    IniFile.AddData(myEngineSettings.ShowFramerateTitle, "framerate_title", "graphics");

    // Write the sound layer count
    IniFile.AddData(myEngineSettings.SoundLayerCount, "soundlayers", "audio");

    // Write the music layer count
    IniFile.AddData(myEngineSettings.MusicLayerCount, "musiclayers", "audio");

    // Write the global volume
    IniFile.AddData(myEngineSettings.GlobalVolume, "volume", "audio");

    // Write into the options-file
    IniFile.WriteToFile("options.ini");

    // The pwEngineOptions are not initialized anymore
    myInitialized = false;
    return true;
}

////////////////////////////////////////////////////////////////
// Get the current Settings
//
// \return pwEngineSettings struct with all important options set
////////////////////////////////////////////////////////////////
const pwEngineSettings* pwEngineOptions::GetCurrentSettings()
{
    // Were the pwEngineOptions already initialized
    if (!myInitialized)
    {
        // If not, initialize now and continue
        Initialize();
    }

    return &myEngineSettings;
}

//
// Video / Window Settings
void pwEngineOptions::SetVideoMode(const sf::VideoMode& VideoMode)
{
    if (myEngineSettings.VideoMode == VideoMode)
        return;

    myChanged = true;

    if ((myEngineSettings.WindowStyle & sf::Style::Fullscreen) == sf::Style::Fullscreen && !VideoMode.isValid())
    {
        pwLog::get()->WriteLine("pwEngineOptions::SetVideoMode: Setting an invalid VideoMode while being in fullscreen. [nothing is changed]", pwLogType::pwLTCaution);
        return;
    }

    bool TmpBool = pwGraphics::get()->Shutdown();
    myEngineSettings.VideoMode = VideoMode;

    if (TmpBool)
    {
        pwGraphics::get()->Initialize();
    }
}
const sf::VideoMode& pwEngineOptions::GetVideoMode()
{
    return myEngineSettings.VideoMode;
}

void pwEngineOptions::SetWindowStyle(unsigned long WindowStyle)
{
    if (myEngineSettings.WindowStyle == WindowStyle)
        return;

    myChanged = true;

    if ((WindowStyle & sf::Style::Fullscreen) == sf::Style::Fullscreen && !myEngineSettings.VideoMode.isValid())
    {
        pwLog::get()->WriteLine("pwEngineOptions::SetWindowStyle: Setting to fullscreen, where the current VideoMode is not valid. [nothing is changed]", pwLogType::pwLTCaution);
        return;
    }

    bool TmpBool = pwGraphics::get()->Shutdown();
    myEngineSettings.WindowStyle = WindowStyle;

    if (TmpBool)
    {
        pwGraphics::get()->Initialize();
    }
}
unsigned long pwEngineOptions::GetWindowStyle()
{
    return myEngineSettings.WindowStyle;
}

void pwEngineOptions::SetWindowTitle(const std::string& Title)
{
    if (myEngineSettings.Title == Title)
        return;

    myEngineSettings.Title = Title;
    pwGraphics::get()->GetRenderWindow().setTitle(Title);
}
const std::string& pwEngineOptions::GetWindowTitle()
{
    return myEngineSettings.Title;
}

void pwEngineOptions::SetShowFramerateTitle(bool ShowFramerate)
{
    if (myEngineSettings.ShowFramerateTitle == ShowFramerate)
        return;

    myChanged = true;

    myEngineSettings.ShowFramerateTitle = ShowFramerate;
}
bool pwEngineOptions::GetShowFramerateTitle()
{
    return myEngineSettings.ShowFramerateTitle;
}

// Sound Settings
void pwEngineOptions::SetSoundLayerCount(unsigned int LayerCount)
{
    if (myEngineSettings.SoundLayerCount == LayerCount)
        return;

    myChanged = true;

    bool TmpBool = pwAudio::get()->Shutdown();
    myEngineSettings.SoundLayerCount = LayerCount;

    if (TmpBool)
    {
        pwAudio::get()->Initialize();
    }
}
unsigned int pwEngineOptions::GetSoundLayerCount()
{
    return myEngineSettings.SoundLayerCount;
}
    
void pwEngineOptions::SetMusicLayerCount(unsigned int LayerCount)
{
    if (myEngineSettings.MusicLayerCount == LayerCount)
        return;

    myChanged = true;

    bool TmpBool = pwAudio::get()->Shutdown();
    myEngineSettings.MusicLayerCount = LayerCount;

    if (TmpBool)
    {
        pwAudio::get()->Initialize();
    }
}
unsigned int pwEngineOptions::GetMusicLayerCount()
{
    return myEngineSettings.MusicLayerCount;
}

void pwEngineOptions::SetGlobalVolume(float Volume)
{
    if (myEngineSettings.GlobalVolume == Volume)
        return;

    myChanged = true;

    myEngineSettings.GlobalVolume = Volume;
    sf::Listener::setGlobalVolume(Volume);
}
float pwEngineOptions::GetGlobalVolume()
{
    return myEngineSettings.GlobalVolume;
}

void pwEngineOptions::SetInternGraphicsSize(const sf::Vector2u& GraphicsSize)
{
    if (myEngineSettings.InternSize.x == GraphicsSize.x &&
        myEngineSettings.InternSize.y == GraphicsSize.y)
        return;
        
    bool TmpBool = pwGraphics::get()->Shutdown();
    myEngineSettings.InternSize = GraphicsSize;

    if (TmpBool)
    {
        pwGraphics::get()->Initialize();
    }
}
const sf::Vector2u& pwEngineOptions::GetInternGraphicsSize()
{
    return myEngineSettings.InternSize;
}

bool pwEngineOptions::GetScaled()
{
    return (GetVideoMode().width < GetInternGraphicsSize().x || GetVideoMode().height < GetInternGraphicsSize().y);
}
//
#include "..\..\include\Game\CSettings.h"
#include "..\..\include\Game\CMusicSystem.h"

CSettings* CSettings::m_s_p_ciInstance = NULL;

CSettings::CSettings() :
    
    m_fSoundVolume(100.0f),
    m_fMusicVolume(100.0f),
    
    m_eLayer00(sf::Keyboard::Q),
	m_eLayer01(sf::Keyboard::W),
    m_eLayer02(sf::Keyboard::E),
    m_eLayer03(sf::Keyboard::R),
    m_eLayer04(sf::Keyboard::Space),

    m_bChanged(false),
    m_bInitialized(false),

    m_sHighscoreFileName("data\\ini\\highscore")
{
    m_aSoundVolumes[0] = 1.0f;
    m_aSoundVolumes[1] = 1.0f;
    m_aSoundVolumes[2] = 1.0f;
    m_aSoundVolumes[3] = 1.0f;
    m_aSoundVolumes[4] = 0.4f;
    m_aSoundVolumes[5] = 0.4f;
    m_aSoundVolumes[6] = 0.4f;
    m_aSoundVolumes[7] = 0.4f;
    m_aSoundVolumes[8] = 0.4f;
    m_aSoundVolumes[9] = 0.4f;
    m_aSoundVolumes[10] = 0.4f;
    m_aSoundVolumes[11] = 0.4f;
    m_aSoundVolumes[12] = 0.4f;
    m_aSoundVolumes[13] = 0.4f;
    m_aSoundVolumes[14] = 0.4f;
    m_aSoundVolumes[15] = 0.4f;
    m_aSoundVolumes[16] = 0.4f;
    m_aSoundVolumes[17] = 0.4f;
    m_aSoundVolumes[18] = 0.4f;
    m_aSoundVolumes[19] = 0.4f;
    m_aSoundVolumes[20] = 0.4f;
    m_aSoundVolumes[21] = 0.4f;
    m_aSoundVolumes[22] = 0.4f;
    m_aSoundVolumes[23] = 0.4f;
    m_aSoundVolumes[24] = 0.4f;
    m_aSoundVolumes[25] = 0.4f;
    m_aSoundVolumes[26] = 0.4f;
    m_aSoundVolumes[27] = 0.4f;
    m_aSoundVolumes[28] = 0.4f;
    m_aSoundVolumes[29] = 0.4f;
    m_aSoundVolumes[30] = 0.4f;
    m_aSoundVolumes[31] = 0.4f;
}

CSettings::~CSettings() 
{
}

void CSettings::setSoundVolume(float Volume)
{
    if (Volume == m_fSoundVolume)
        return;
    
    m_bChanged = true;

    for (unsigned int i = 0; i < pwEngineOptions::GetSoundLayerCount(); i++)
    {
        glAudio->GetSoundLayerSettings(i)->SetVolume(Volume * m_aSoundVolumes[i]);
        glAudio->UpdateLayerSoundParameters(i);
    }

    m_fSoundVolume = Volume;
}
float CSettings::getSoundVolume()
{
    return m_fSoundVolume;
}

void CSettings::setMusicVolume(float Volume)
{
    if (Volume == m_fMusicVolume)
        return;
    
    m_bChanged = true;

    CMusicSystem::get()->getSound()->setVolume(Volume);
    
    m_fMusicVolume = Volume;
}
float CSettings::getMusicVolume()
{
    return m_fMusicVolume;
}

void CSettings::setLayerKey(unsigned int Layer, sf::Keyboard::Key Key)
{
    if (Key == getLayerKey(Layer))
        return;

    m_bChanged = true;

    switch (Layer)
    {
    case 0:
        m_eLayer00 = Key;
        break;

    case 1:
        m_eLayer01 = Key;
        break;

    case 2:
        m_eLayer02 = Key;
        break;

    case 3:
        m_eLayer03 = Key;
        break;

    case 4:
        m_eLayer04 = Key;
        break;
    }
}
sf::Keyboard::Key CSettings::getLayerKey(unsigned int Layer)
{
    switch (Layer)
    {
    case 0:
        return m_eLayer00;
        break;

    case 1:
        return m_eLayer01;
        break;

    case 2:
        return m_eLayer02;
        break;

    case 3:
        return m_eLayer03;
        break;

    case 4:
        return m_eLayer04;
        break;

    default:
        return sf::Keyboard::Escape;
    }
}

const std::string& CSettings::getHighscoreFileName()
{
    return m_sHighscoreFileName;
}

void CSettings::init()
{
    if (m_bInitialized)
        return;

    m_bChanged = false;

    // Temporary variables for reading in the options
    std::string TmpString;
    int         TmpInt;
    float       TmpFloat;
    pwIniFile*  IniFile = glDataBox->GetIniFile("settings.ini");

    // Open the options-file in the pwIniReader
    if (IniFile != NULL)
    {
        // Get the sound-volume
        if (IniFile->GetData(&TmpFloat, "soundvolume", "audio"))
        {
            setSoundVolume(TmpFloat);
        } else {
            m_bChanged = true;
        }

        // Get the music-volume
        if (IniFile->GetData(&TmpFloat, "musicvolume", "audio"))
        {
            setMusicVolume(TmpFloat);
        } else {
            m_bChanged = true;
        }

        // Get the keys for the layers
        if (IniFile->GetData(&TmpInt, "0", "keyboard"))
        {
            m_eLayer00 = (sf::Keyboard::Key)TmpInt;
        } else {
            m_bChanged = true;
        }
        if (IniFile->GetData(&TmpInt, "1", "keyboard"))
        {
            m_eLayer01 = (sf::Keyboard::Key)TmpInt;
        } else {
            m_bChanged = true;
        }
        if (IniFile->GetData(&TmpInt, "2", "keyboard"))
        {
            m_eLayer02 = (sf::Keyboard::Key)TmpInt;
        } else {
            m_bChanged = true;
        }
        if (IniFile->GetData(&TmpInt, "3", "keyboard"))
        {
            m_eLayer03 = (sf::Keyboard::Key)TmpInt;
        } else {
            m_bChanged = true;
        }
        if (IniFile->GetData(&TmpInt, "4", "keyboard"))
        {
            m_eLayer04 = (sf::Keyboard::Key)TmpInt;
        } else {
            m_bChanged = true;
        }

        if (IniFile->GetData(&TmpString, "path", "highscore"))
        {
            m_sHighscoreFileName = TmpString;
        } else {
            m_bChanged = true;
        }
    }
    else
    {
        // Caution: Default values are used
        m_bChanged = true;
    }

    m_bInitialized = true;
}

void CSettings::save()
{
    if (!m_bInitialized || !m_bChanged)
    {
        // Do nothing, if the logging-system wasn't even started
        return;
    }

    pwIniFile IniFile;

    // Write the sound-volume
    IniFile.AddData(m_fSoundVolume, "soundvolume", "audio");

    // Write the music-volume
    IniFile.AddData(m_fMusicVolume, "musicvolume", "audio");

    // Write the keys for the layers
    IniFile.AddData((int)m_eLayer00, "0", "keyboard");
    IniFile.AddData((int)m_eLayer01, "1", "keyboard");
    IniFile.AddData((int)m_eLayer02, "2", "keyboard");
    IniFile.AddData((int)m_eLayer03, "3", "keyboard");
    IniFile.AddData((int)m_eLayer04, "4", "keyboard");

    //
    IniFile.AddData(m_sHighscoreFileName, "path", "highscore");

    // Write into the settings-file
    IniFile.WriteToFile("data\\ini\\settings.ini");
}

CSettings* CSettings::get()
{
    if (m_s_p_ciInstance == NULL)
    {
        m_s_p_ciInstance = new CSettings;
    }

    return m_s_p_ciInstance;
}
#include "..\..\include\Game\CMusicSystem.h"
#include "..\..\include\Game\CBeatSystem.h"
#include "..\..\include\Game\CSettings.h"

std::string CMusicSystem::m_ciFileName;
std::string CMusicSystem::m_ciBPM;
unsigned int CMusicSystem::m_uiBeats = 1;
bool CMusicSystem::m_bChangeBPM = false;
CMusicSystem* CMusicSystem::m_s_p_ciInstance = NULL;

void CMusicSystem::CorrectSound()
{
    if (m_bChangeBPM)
    {
        CMusicSystem::get()->getSound()->stop();

        CMusicSystem::get()->getSound()->setBuffer(*glDataBox->GetSoundBuffer(m_ciFileName + "_" + m_ciBPM + ".ogg"));
        CMusicSystem::get()->getSound()->setPlayingOffset(sf::seconds(m_uiBeats*CBeatSystem::GetPerBeatTime()));
        CMusicSystem::get()->getSound()->play();

        m_bChangeBPM = false;
    }
    
  
    if (CMusicSystem::get()->getSound()->getStatus() == CMusicSystem::get()->getSound()->Stopped)
    {
        CMusicSystem::get()->getSound()->stop();
        CMusicSystem::get()->getSound()->play();

        //glGLog << "Music restart" << pwGraphicsLog::flush;

        m_uiBeats = 0;
    }

    float Offset = CMusicSystem::get()->getSound()->getPlayingOffset().asSeconds() - m_uiBeats * CBeatSystem::GetPerBeatTime();

    //glGLog << CMusicSystem::get()->getSound()->getStatus() << pwGraphicsLog::flush;

    if (Offset > 0.1f || Offset < -0.1f)
    {
        // std::cout << TmpMusic->getPlayingOffset().asSeconds() - /*Offset - */TmpMusic->getBuffer()->getDuration().asSeconds() << std::endl;
        if (CMusicSystem::get()->getSound()->getPlayingOffset().asSeconds() - Offset < CMusicSystem::get()->getSound()->getBuffer()->getDuration().asSeconds())
            CMusicSystem::get()->getSound()->setPlayingOffset(sf::seconds(CMusicSystem::get()->getSound()->getPlayingOffset().asSeconds() - Offset));
    }

    m_uiBeats++;
}

void CMusicSystem::SetBPM(unsigned int BPM)
{
    std::stringstream TmpSStream;

    TmpSStream << BPM << "bpm";
    TmpSStream >> m_ciBPM;
    float Offset = 0.0f;

    if (m_uiBPM != 0)
    {
        // Offset = glAudio->GetSound()->getPlayingOffset().asSeconds() * (float)m_uiBPM / (float)BPM;
        m_bChangeBPM = true;
    }
    else 
    {
        m_ciSound->stop();
        m_ciSound->setBuffer(*glDataBox->GetSoundBuffer(m_ciFileName + "_" + m_ciBPM + ".ogg"));
        m_ciSound->play();
    }

    m_uiBPM = BPM;

    m_ciTimer.changeTimerSettings(CBeatSystem::GetPerBeatTime());
    m_ciTimer.start();
}

void CMusicSystem::init()
{
    m_ciFileName = glDataBox->GetIniFile("leveldata.ini")->GetString("Music", CLevelData::getCurrentLevelString());
    //m_uiBeats = 0;
    
    glAudio->SetLayerSoundParameters(m_ciSound);
    m_ciSound->setVolume(CSettings::get()->getMusicVolume());

    m_ciTimer.changeTimerSettings(CBeatSystem::GetPerBeatTime());
    m_ciTimer.addEventListener(TimerTick, &CMusicSystem::CorrectSound);
    m_ciTimer.start();
}

void CMusicSystem::shutdown()
{
    m_uiBPM = 0;
    m_uiBeats = 1;
    m_bChangeBPM = false;

    m_ciSound->stop();
}

CMusicSystem* CMusicSystem::get()
{
    if (m_s_p_ciInstance == NULL)
    {
        m_s_p_ciInstance = new CMusicSystem;
    }

    return m_s_p_ciInstance;
}
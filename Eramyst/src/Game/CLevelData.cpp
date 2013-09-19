#include "..\..\include\Game\CLevelData.h"
#include "..\..\include\Game\CInterface.h"
#include "..\..\include\Game\CSettings.h"
#include "..\..\include\Game\CColoss.h"
#include <ctype.h>

float CLevelData::m_s_fCurrentLevelTime;
float CLevelData::m_s_fCurrentLevelEndTime;
std::string CLevelData::m_s_ciCurrentLevelString;
bool CLevelData::m_bPlayLevelOnce;

bool EraseWhiteSpaces(std::string *String)
{
    while(true)
    {
        // Is string empty?
        if (String->empty())
            return false;
        
        // Find a white-space (' ', '\t') from the beginning of the string
        if (String->at(0) == ' ' || String->at(0) == '\t')
            // Erase the white-space
            String->erase(String->begin());
        else
            break;
    }
    while(true)
    {
        // Is string empty?
        if (String->empty())
            return false;
        
        // Find a white-space (' ', '\t') from the end of the string
        if (String->at(String->length()-1) == ' ' || String->at(String->length()-1) == '\t')
            // Erase the white-space
            String->pop_back();
        else
            break;
    }

    // Erasing white-spaces was successful
    return true;
}

std::string EraseWhiteSpaces(std::string String)
{
    EraseWhiteSpaces(&String);
    return String;
}

bool CLevelData::Start()
{
    if (!m_bInitialized)
    {
        m_ciShape.setSize(sf::Vector2f((float)pwEngineOptions::GetInternGraphicsSize().x, (float)pwEngineOptions::GetInternGraphicsSize().y));
        m_ciShape.setFillColor(sf::Color(29, 29, 29));

        pwIniFile* TmpIni = glDataBox->GetIniFile("gameover.ini");

        //m_ciGameOverSprite.setTexture(*glDataBox->GetTexture(TmpIni->GetString("file", "title")));
        //m_ciGameOverSprite.setOrigin((float)m_ciGameOverSprite.getTexture()->getSize().x / 2.0f, (float)m_ciGameOverSprite.getTexture()->getSize().y / 2.0f);
        //m_ciGameOverSprite.setPosition((float)pwEngineOptions::GetInternGraphicsSize().x / 2.0f, TmpIni->GetFloat("y", "title"));
        //m_ciGameOverSprite.setScale(TmpIni->GetFloat("scale", "title"), TmpIni->GetFloat("scale", "title"));


        m_ciScoreText.setFont(*glDataBox->GetFont("averia.ttf"));
        m_ciScoreText.setString(TmpIni->GetString("title", "score_text"));
        m_ciScoreText.setOrigin(m_ciScoreText.getGlobalBounds().width / 2.0f, (float)m_ciScoreText.getCharacterSize() / 2.0f);
        m_ciScoreText.setPosition((float)pwEngineOptions::GetInternGraphicsSize().x / 2.0f, TmpIni->GetFloat("y", "score_text"));
        m_ciScoreText.setCharacterSize(TmpIni->GetUInt("size", "score_text"));

        m_ciScore.setFont(*glDataBox->GetFont("averia.ttf"));
        m_ciScore.setPosition((float)pwEngineOptions::GetInternGraphicsSize().x / 2.0f, TmpIni->GetFloat("y", "score"));
        m_ciScore.setCharacterSize(TmpIni->GetUInt("size", "score"));

        m_ciInput.setFont(*glDataBox->GetFont("averia.ttf"));
        m_ciInput.setPosition((float)pwEngineOptions::GetInternGraphicsSize().x / 2.0f, TmpIni->GetFloat("y", "input"));
        m_ciInput.setCharacterSize(TmpIni->GetUInt("size", "input"));

        m_ciContinue.setFont(*glDataBox->GetFont("averia.ttf"));
        m_ciContinue.setString(TmpIni->GetString("title", "continue"));
        m_ciContinue.setOrigin(m_ciContinue.getGlobalBounds().width / 2.0f, (float)m_ciContinue.getCharacterSize() / 2.0f);
        m_ciContinue.setPosition((float)pwEngineOptions::GetInternGraphicsSize().x / 2.0f, TmpIni->GetFloat("y", "continue"));
        m_ciContinue.setCharacterSize(TmpIni->GetUInt("size", "continue"));

        m_ciEnterName.setFont(*glDataBox->GetFont("averia.ttf"));
        m_ciEnterName.setString(TmpIni->GetString("title_ex", "continue"));
        m_ciEnterName.setOrigin(m_ciEnterName.getGlobalBounds().width / 2.0f, (float)m_ciEnterName.getCharacterSize() / 2.0f);
        m_ciEnterName.setPosition((float)pwEngineOptions::GetInternGraphicsSize().x / 2.0f, TmpIni->GetFloat("y", "continue"));
        m_ciEnterName.setCharacterSize(TmpIni->GetUInt("size", "continue"));

        m_bInitialized = true;
    }

    std::stringstream typecast;
    std::string string;
    typecast << CInterface::get()->getCurrentScore();
    typecast >> string;
    typecast.clear();

    m_ciScore.setString(string);
    m_ciScore.setOrigin(m_ciScore.getGlobalBounds().width / 2.0f, (float)m_ciScore.getCharacterSize() / 2.0f);

    m_HighscoreIni.clear();
    m_HighscoreIni.LoadFromFile(CSettings::get()->getHighscoreFileName());
    m_HighscoreIni.SetFileName(CSettings::get()->getHighscoreFileName());

    m_sInput = "";
    m_ciInput.setString("_");
    m_ciInput.setOrigin(m_ciInput.getGlobalBounds().width / 2.0f, (float)m_ciInput.getCharacterSize() / 2.0f);
    m_bHighscore = true;
    m_HighscoreMap.clear();
    for (int i = 0; i < m_HighscoreIni.GetInt("count"); i++)
    {
        typecast << i;
        typecast >> string;
        typecast.clear();

        m_HighscoreMap.insert(std::make_pair(m_HighscoreIni.GetString(string + "name"), m_HighscoreIni.GetUInt(string + "score")));
    }

    m_bNewPressKey = false;


    return true;
}

unsigned int CLevelData::Run()
{
    if (CInterface::get()->getCurrentLife() == 0.0f && !m_bHighscore)
    {
        if (glSystem->GetKeyPressed().first && !m_bNewPressKey)
        {
            CInterface::get()->shutdown();
            CInterface::get()->setCurrentScore(0);
            return ID_GS_MENU_MAIN;
        }
        else if (!glSystem->GetKeyPressed().first)
        {
            m_bNewPressKey = false;
        }
        
        glGraphics->Draw(&m_ciShape);
        //glGraphics->Draw(&m_ciGameOverSprite);
        glGraphics->Draw(&m_ciScoreText);
        glGraphics->Draw(&m_ciScore);
        glGraphics->Draw(&m_ciContinue);

        return myID;
    }
    else if (CInterface::get()->getCurrentLife() == 0.0f)
    {
        if (glSystem->GetEnteredText().first && iswprint(glSystem->GetEnteredText().second) && m_sInput.size() < m_iMax)
        {
            m_sInput = m_sInput + sf::String(glSystem->GetEnteredText().second);

            if (m_sInput.size() != m_iMax)
                m_ciInput.setString(m_sInput + "_");
            else 
                m_ciInput.setString(m_sInput);

            m_ciInput.setOrigin(m_ciInput.getGlobalBounds().width / 2.0f, (float)m_ciInput.getCharacterSize() / 2.0f);
        }
        else if (glSystem->GetEnteredText().first && glSystem->GetEnteredText().second == 8 && !m_sInput.empty())
        {
            m_sInput.pop_back();
            m_ciInput.setString(m_sInput + "_");
            m_ciInput.setOrigin(m_ciInput.getGlobalBounds().width / 2.0f, (float)m_ciInput.getCharacterSize() / 2.0f);
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return) && EraseWhiteSpaces(m_sInput) != "")
        {
            m_bNewPressKey = true;

            std::stringstream typecast;
            std::string string;
            unsigned int i = 0;
            std::multimap<unsigned int, std::string> WriteMap;
            EraseWhiteSpaces(&m_sInput);

            m_HighscoreMap.clear();
            for (int j = 0; j < m_HighscoreIni.GetInt("count"); j++)
            {
                typecast << j;
                typecast >> string;
                typecast.clear();

                m_HighscoreMap.insert(std::make_pair(m_HighscoreIni.GetString(string + "name"), m_HighscoreIni.GetUInt(string + "score")));
            }

            std::pair<std::map<std::string, unsigned int>::iterator, bool> InsertPair = m_HighscoreMap.insert(std::make_pair(m_sInput, CInterface::get()->getCurrentScore()));

            if (!InsertPair.second && InsertPair.first->second < CInterface::get()->getCurrentScore())
            {
                InsertPair.first->second = CInterface::get()->getCurrentScore();
            }
            else if (!InsertPair.second)
            {
                m_bHighscore = false;
                return myID;
            }

            for (std::map<std::string, unsigned int>::iterator j = m_HighscoreMap.begin(); j != m_HighscoreMap.end(); j++)
            {
                WriteMap.insert(std::make_pair(j->second, j->first));
            }

            m_HighscoreIni.AddData(WriteMap.size(), "count");
            for (std::multimap<unsigned int, std::string>::reverse_iterator j = WriteMap.rbegin(); j != WriteMap.rend(); j++)
            {
                typecast << i;
                typecast >> string;
                typecast.clear();
                
                m_HighscoreIni.AddData(j->second, string + "name");
                m_HighscoreIni.AddData(j->first, string + "score");

                i++;
            }

            m_HighscoreIni.WriteToFile(m_HighscoreIni.GetFileName());
            CInterface::get()->shutdown();
            CInterface::get()->setCurrentScore(0);
            return ID_GS_HIGHSCORE;
        }

        glGraphics->Draw(&m_ciShape);
        glGraphics->Draw(&m_ciScoreText);
        glGraphics->Draw(&m_ciScore);
        glGraphics->Draw(&m_ciInput);
        glGraphics->Draw(&m_ciEnterName);

        return myID;
    }
    else
    {
        pwIniFile* TmpIni = glDataBox->GetIniFile("leveldata.ini");

        m_s_ciCurrentLevelString = TmpIni->GetString("Next", m_s_ciCurrentLevelString);

        if (m_s_ciCurrentLevelString == "end" || m_bPlayLevelOnce)
        {
            CInterface::get()->setCurrentLife(0.0f);
            return myID;
        }

        m_s_fCurrentLevelEndTime = (float)TmpIni->GetInt("Duration", m_s_ciCurrentLevelString);
        m_s_fCurrentLevelTime = 0.0f;
    }

    glGraphics->Draw(&m_ciShape);

    CInterface::get()->shutdown();
    return ID_GS_PLAY;
}

void CLevelData::setCurrentLevelString(std::string LevelString, bool PlayOnce)
{
    m_s_ciCurrentLevelString = LevelString;
    m_bPlayLevelOnce = PlayOnce;

    pwIniFile* TmpIni = glDataBox->GetIniFile("leveldata.ini");

    m_s_fCurrentLevelEndTime = (float)TmpIni->GetInt("Duration", m_s_ciCurrentLevelString);
    m_s_fCurrentLevelTime = 0.0f;
}
std::string CLevelData::getCurrentLevelString()
{
    return m_s_ciCurrentLevelString;
}

void CLevelData::UpdateCurrentLevelTime(float FrameTime)
{
    m_s_fCurrentLevelTime += FrameTime;
	CInterface::get()->setCurrentLevelTime((m_s_fCurrentLevelEndTime - m_s_fCurrentLevelTime) >= 0.0f ? m_s_fCurrentLevelEndTime - m_s_fCurrentLevelTime : 0.0f);

	if(m_s_fCurrentLevelTime >= m_s_fCurrentLevelEndTime && !CColoss::getBossFinished())
	{
		if(!CColoss::getBoss())
		{
			if(CLevelData::getCurrentLevelString() == "darkage")
			{
				CBeatSystem::SetBPM(100);
			}
			else if(CLevelData::getCurrentLevelString() == "future")
			{
				CBeatSystem::SetBPM(120);
			}
		}

		CColoss::setBoss(true);
	}
}
bool CLevelData::LevelToChange()
{
	return (m_s_fCurrentLevelTime >= m_s_fCurrentLevelEndTime) && CColoss::getBossFinished();
}

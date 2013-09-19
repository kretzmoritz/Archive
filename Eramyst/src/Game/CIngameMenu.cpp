#include "..\..\include\Game\CIngameMenu.h"
#include "..\..\include\Game\CPlay.h"

CIngame::CIngame() : pwGameState(ID_GS_INGAME)
{
    Initialized = false;
}

CIngame::~CIngame()
{
}

bool CIngame::Start() 
{
    if (Initialized)
        return true;

    pwIniFile* TmpIni = glDataBox->GetIniFile("menu_ingame.ini");

    m_ciBackground.setTexture(*glDataBox->GetTexture(TmpIni->GetString("file", "background")));
    
    m_ciBackgroundObject00.setTexture(*glDataBox->GetTexture(TmpIni->GetString("file", "object00")));
    m_ciBackgroundObject01.setTexture(*glDataBox->GetTexture(TmpIni->GetString("file", "object01")));
    m_ciTitle.setTexture(*glDataBox->GetTexture(TmpIni->GetString("file", "title")));
    
    m_ciBackgroundObject00.setOrigin((float)(m_ciBackgroundObject00.getTexture()->getSize().x) / 2.0f - 12.0f, (float)m_ciBackgroundObject00.getTexture()->getSize().y / 2.0f - 25.0f);
    m_ciBackgroundObject01.setOrigin((float)m_ciBackgroundObject01.getTexture()->getSize().x / 2.0f, (float)m_ciBackgroundObject01.getTexture()->getSize().y / 2.0f);
    m_ciTitle.setOrigin((float)m_ciTitle.getTexture()->getSize().x / 2.0f, (float)m_ciTitle.getTexture()->getSize().y / 2.0f);

    m_ciBackgroundObject00.setPosition(TmpIni->GetFloat("x", "object00"), TmpIni->GetFloat("y", "object00"));
    m_ciBackgroundObject01.setPosition(TmpIni->GetFloat("x", "object01"), TmpIni->GetFloat("y", "object01"));
    m_ciTitle.setPosition(TmpIni->GetFloat("x", "title"), TmpIni->GetFloat("y", "title"));

    m_ciBackgroundObject00.setScale(TmpIni->GetFloat("scale", "object00"), TmpIni->GetFloat("scale", "object00"));
    m_ciBackgroundObject01.setScale(TmpIni->GetFloat("scale", "object01"), TmpIni->GetFloat("scale", "object01"));
    m_ciTitle.setScale(TmpIni->GetFloat("scale", "title"), TmpIni->GetFloat("scale", "title"));

    m_fBackground00Rotation = TmpIni->GetFloat("rotation_speed", "object00");
    m_fBackground01Rotation = TmpIni->GetFloat("rotation_speed", "object01");

    m_ciBackgroundObject00.setColor(sf::Color(255, 255, 255, TmpIni->GetInt("opacity", "object00")));
    m_ciBackgroundObject01.setColor(sf::Color(255, 255, 255, TmpIni->GetInt("opacity", "object01")));
    m_ciTitle.setColor(sf::Color(255, 255, 255, TmpIni->GetInt("opacity", "title")));


    m_ciPlayIdle.setTexture(*glDataBox->GetTexture(TmpIni->GetString("file", "play")));
    m_ciPlayHover.setTexture(*m_ciPlayIdle.getTexture());
    m_ciPlayIdle.setColor(sf::Color(255, 255, 255, TmpIni->GetInt("opacity", "play")));
    m_ciPlayHover.setColor(sf::Color(255, 255, 255, TmpIni->GetInt("over_opacity", "play")));
    m_ciPlayIdle.setOrigin(m_ciPlayIdle.getTextureRect().width / 2.0f, m_ciPlayIdle.getTextureRect().height / 2.0f);
    m_ciPlayHover.setOrigin(m_ciPlayHover.getTextureRect().width / 2.0f, m_ciPlayHover.getTextureRect().height / 2.0f);
    m_ciPlayIdle.setPosition(m_ciPlayIdle.getTextureRect().width / 2.0f, m_ciPlayIdle.getTextureRect().height / 2.0f);
    m_ciPlayHover.setPosition(m_ciPlayHover.getTextureRect().width / 2.0f, m_ciPlayHover.getTextureRect().height / 2.0f);
    m_ciPlayIdle.setScale(TmpIni->GetFloat("scale", "play"), TmpIni->GetFloat("scale", "play"));
    m_ciPlayHover.setScale(TmpIni->GetFloat("scale", "play"), TmpIni->GetFloat("scale", "play"));

    m_ciReplayIdle.setTexture(*glDataBox->GetTexture(TmpIni->GetString("file", "replay")));
    m_ciReplayHover.setTexture(*m_ciReplayIdle.getTexture());
    m_ciReplayIdle.setColor(sf::Color(255, 255, 255, TmpIni->GetInt("opacity", "replay")));
    m_ciReplayHover.setColor(sf::Color(255, 255, 255, TmpIni->GetInt("over_opacity", "replay")));
    m_ciReplayIdle.setOrigin(m_ciReplayIdle.getTextureRect().width / 2.0f, m_ciReplayIdle.getTextureRect().height / 2.0f);
    m_ciReplayHover.setOrigin(m_ciReplayHover.getTextureRect().width / 2.0f, m_ciReplayHover.getTextureRect().height / 2.0f);
    m_ciReplayIdle.setPosition(m_ciReplayIdle.getTextureRect().width / 2.0f, m_ciReplayIdle.getTextureRect().height / 2.0f);
    m_ciReplayHover.setPosition(m_ciReplayHover.getTextureRect().width / 2.0f, m_ciReplayHover.getTextureRect().height / 2.0f);
    m_ciReplayIdle.setScale(TmpIni->GetFloat("scale", "replay"), TmpIni->GetFloat("scale", "replay"));
    m_ciReplayHover.setScale(TmpIni->GetFloat("scale", "replay"), TmpIni->GetFloat("scale", "replay"));

    m_ciBackToMenuIdle.setTexture(*glDataBox->GetTexture(TmpIni->GetString("file", "backtomenu")));
    m_ciBackToMenuHover.setTexture(*m_ciBackToMenuIdle.getTexture());
    m_ciBackToMenuIdle.setColor(sf::Color(255, 255, 255, TmpIni->GetInt("opacity", "backtomenu")));
    m_ciBackToMenuHover.setColor(sf::Color(255, 255, 255, TmpIni->GetInt("over_opacity", "backtomenu")));
    m_ciBackToMenuIdle.setOrigin(m_ciBackToMenuIdle.getTextureRect().width / 2.0f, m_ciBackToMenuIdle.getTextureRect().height / 2.0f);
    m_ciBackToMenuHover.setOrigin(m_ciBackToMenuHover.getTextureRect().width / 2.0f, m_ciBackToMenuHover.getTextureRect().height / 2.0f);
    m_ciBackToMenuIdle.setPosition(m_ciBackToMenuIdle.getTextureRect().width / 2.0f, m_ciBackToMenuIdle.getTextureRect().height / 2.0f);
    m_ciBackToMenuHover.setPosition(m_ciBackToMenuHover.getTextureRect().width / 2.0f, m_ciBackToMenuHover.getTextureRect().height / 2.0f);
    m_ciBackToMenuIdle.setScale(TmpIni->GetFloat("scale", "backtomenu"), TmpIni->GetFloat("scale", "backtomenu"));
    m_ciBackToMenuHover.setScale(TmpIni->GetFloat("scale", "backtomenu"), TmpIni->GetFloat("scale", "backtomenu"));    

    m_ciBackToMenuText.setFont(*glDataBox->GetFont("averia.ttf"));
    m_ciBackToMenuText.setString(TmpIni->GetString("title", "backtomenu"));
    m_ciBackToMenuText.setOrigin(m_ciBackToMenuText.getGlobalBounds().width / 2.0f, (float)m_ciBackToMenuText.getCharacterSize() / 2.0f);

    m_ciPlayCollision.SetSize((float)m_ciPlayIdle.getTexture()->getSize().x, (float)m_ciPlayIdle.getTexture()->getSize().y);
    m_ciBackToMenuCollision.SetSize((float)m_ciBackToMenuIdle.getTexture()->getSize().x, (float)m_ciBackToMenuIdle.getTexture()->getSize().y);
    m_ciReplayCollision.SetSize((float)m_ciReplayIdle.getTexture()->getSize().x, (float)m_ciReplayIdle.getTexture()->getSize().y);

    m_ciBackToMenuText.setPosition(m_ciBackToMenuCollision.GetSize().x / 2.0f, m_ciBackToMenuCollision.GetSize().y / 2.0f - (float)m_ciBackToMenuText.getCharacterSize() / 4.0f);
   
    m_ciPlayButton.AddCollisionObject(0, &m_ciPlayCollision);
    m_ciPlayButton.AddDrawableObject(0, &m_ciPlayIdle);
    m_ciPlayButton.AddDrawableObject(1, &m_ciPlayHover);
    m_ciPlayButton.AddDrawableObject(2, &m_ciPlayHover);
    m_ciPlayButton.BindDrawableToState(0, pwGUI::pwButton::Idle);
    m_ciPlayButton.BindDrawableToState(1, pwGUI::pwButton::Hover);
    m_ciPlayButton.BindDrawableToState(2, pwGUI::pwButton::Down);
    m_ciPlayButton.setPosition(TmpIni->GetFloat("x", "play"), TmpIni->GetFloat("y", "play"));

    m_ciBackToMenuButton.AddCollisionObject(0, &m_ciBackToMenuCollision);
    m_ciBackToMenuButton.AddDrawableObject(0, &m_ciBackToMenuIdle);
    m_ciBackToMenuButton.AddDrawableObject(1, &m_ciBackToMenuHover);
    m_ciBackToMenuButton.AddDrawableObject(2, &m_ciBackToMenuHover);
    m_ciBackToMenuButton.AddDrawableObject(3, &m_ciBackToMenuText);
    m_ciBackToMenuButton.BindDrawableToState(0, pwGUI::pwButton::Idle);
    m_ciBackToMenuButton.BindDrawableToState(1, pwGUI::pwButton::Hover);
    m_ciBackToMenuButton.BindDrawableToState(2, pwGUI::pwButton::Down);
    m_ciBackToMenuButton.setPosition(TmpIni->GetFloat("x", "backtomenu"), TmpIni->GetFloat("y", "backtomenu"));

    m_ciReplayButton.AddCollisionObject(0, &m_ciReplayCollision);
    m_ciReplayButton.AddDrawableObject(0, &m_ciReplayIdle);
    m_ciReplayButton.AddDrawableObject(1, &m_ciReplayHover);
    m_ciReplayButton.AddDrawableObject(2, &m_ciReplayHover);
    m_ciReplayButton.BindDrawableToState(0, pwGUI::pwButton::Idle);
    m_ciReplayButton.BindDrawableToState(1, pwGUI::pwButton::Hover);
    m_ciReplayButton.BindDrawableToState(2, pwGUI::pwButton::Down);
    m_ciReplayButton.setPosition(TmpIni->GetFloat("x", "replay"), TmpIni->GetFloat("y", "replay"));
    
    glLogics->RegisterGameObject(&m_ciPlayButton, myID);
    glLogics->RegisterGameObject(&m_ciBackToMenuButton, myID);
    glLogics->RegisterGameObject(&m_ciReplayButton, myID);

    Initialized = true;

    return true;
}

unsigned int CIngame::Run()
{
    m_ciBackgroundObject00.rotate(m_fBackground00Rotation * glGraphics->GetLastFrameTime());
    m_ciBackgroundObject01.rotate(m_fBackground01Rotation * glGraphics->GetLastFrameTime());

    glGraphics->Draw(&m_ciBackground);
    glGraphics->Draw(&m_ciBackgroundObject00);
    glGraphics->Draw(&m_ciBackgroundObject01);
    glGraphics->Draw(&m_ciTitle);

    glGraphics->Draw(&m_ciPlayButton);
    glGraphics->Draw(&m_ciReplayButton);
    glGraphics->Draw(&m_ciBackToMenuButton);

    if (m_ciPlayButton.GetClicked())
    {
        return ID_GS_PLAY;
    }
    else if (m_ciBackToMenuButton.GetClicked())
    {
        CPlay::SetMenu(false);
        CMusicSystem::get()->shutdown();
        CInterface::get()->setCurrentScore(0);
        return ID_GS_MENU_MAIN;
    }
    else if (m_ciReplayButton.GetClicked())
    {
        if (CLevelData::getPlayOnce())
        {
            CLevelData::setCurrentLevelString(CLevelData::getCurrentLevelString(), true);
        }
        else
        {
            CLevelData::setCurrentLevelString("darkage");
        }

        CPlay::SetMenu(false);
        CMusicSystem::get()->shutdown();
        CInterface::get()->setCurrentScore(0);
        return ID_GS_PLAY;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) && glSystem->GetKeyPressed().first)
    {
        return ID_GS_PLAY;
    }

    return myID;
}
#include "..\..\include\Game\CMenuCredits.h"

CMenuCredits::CMenuCredits() : pwGameState(ID_GS_MENU_CREDITS)
{
    Initialized = false;
}

CMenuCredits::~CMenuCredits()
{
}

bool CMenuCredits::Start() 
{
    if (Initialized)
        return true;

    pwIniFile* TmpIni = glDataBox->GetIniFile("menu_credits.ini");

    m_ciBackground.setTexture(*glDataBox->GetTexture(TmpIni->GetString("file", "background")));
    
    m_ciCredits.setTexture(*glDataBox->GetTexture("credits.png"));
    m_ciCredits.setOrigin((float)m_ciCredits.getTexture()->getSize().x / 2.0f, (float)m_ciCredits.getTexture()->getSize().y / 2.0f);
    m_ciCredits.setPosition((float)pwEngineOptions::GetInternGraphicsSize().x / 2.0f, (float)pwEngineOptions::GetInternGraphicsSize().y / 2.0f);
    
    m_ciTitle.setTexture(*glDataBox->GetTexture(TmpIni->GetString("file", "title")));
    m_ciTitle.setOrigin((float)m_ciTitle.getTexture()->getSize().x / 2.0f, (float)m_ciTitle.getTexture()->getSize().y / 2.0f);
    m_ciTitle.setPosition(TmpIni->GetFloat("x", "title"), TmpIni->GetFloat("y", "title"));
    m_ciTitle.setScale(TmpIni->GetFloat("scale", "title"), TmpIni->GetFloat("scale", "title"));
    m_ciTitle.setColor(sf::Color(255, 255, 255, TmpIni->GetInt("opacity", "title")));
    
    m_ciBacklIdle.setTexture(*glDataBox->GetTexture(TmpIni->GetString("file", "back")));
    m_ciBackHover.setTexture(*m_ciBacklIdle.getTexture());
    m_ciBacklIdle.setColor(sf::Color(255, 255, 255, TmpIni->GetInt("opacity", "back")));
    m_ciBackHover.setColor(sf::Color(255, 255, 255, TmpIni->GetInt("over_opacity", "back")));
    m_ciBacklIdle.setOrigin(m_ciBacklIdle.getTextureRect().width / 2.0f, m_ciBacklIdle.getTextureRect().height / 2.0f);
    m_ciBackHover.setOrigin(m_ciBackHover.getTextureRect().width / 2.0f, m_ciBackHover.getTextureRect().height / 2.0f);
    m_ciBacklIdle.setPosition(m_ciBacklIdle.getTextureRect().width / 2.0f, m_ciBacklIdle.getTextureRect().height / 2.0f);
    m_ciBackHover.setPosition(m_ciBackHover.getTextureRect().width / 2.0f, m_ciBackHover.getTextureRect().height / 2.0f);
    m_ciBacklIdle.setScale(TmpIni->GetFloat("scale", "back"), TmpIni->GetFloat("scale", "back"));
    m_ciBackHover.setScale(TmpIni->GetFloat("over_scale", "back"), TmpIni->GetFloat("over_scale", "back"));

    m_ciBackCollision.SetSize((float)m_ciBacklIdle.getTexture()->getSize().x, (float)m_ciBacklIdle.getTexture()->getSize().y);

    m_ciBackText.setFont(*glDataBox->GetFont("averia.ttf"));
    m_ciBackText.setString(TmpIni->GetString("title", "back"));
    m_ciBackText.setOrigin(m_ciBackText.getGlobalBounds().width / 2.0f, (float)m_ciBackText.getCharacterSize() / 2.0f);
    m_ciBackText.setPosition(m_ciBackCollision.GetSize().x / 2.0f, m_ciBackCollision.GetSize().y / 2.0f - (float)m_ciBackText.getCharacterSize() / 4.0f);
    
    m_ciBackButton.AddCollisionObject(0, &m_ciBackCollision);
    m_ciBackButton.AddDrawableObject(0, &m_ciBacklIdle);
    m_ciBackButton.AddDrawableObject(1, &m_ciBackHover);
    m_ciBackButton.AddDrawableObject(2, &m_ciBackHover);
    m_ciBackButton.AddDrawableObject(3, &m_ciBackText);
    m_ciBackButton.BindDrawableToState(0, pwGUI::pwButton::Idle);
    m_ciBackButton.BindDrawableToState(1, pwGUI::pwButton::Hover);
    m_ciBackButton.BindDrawableToState(2, pwGUI::pwButton::Down);
    m_ciBackButton.setPosition(TmpIni->GetFloat("x", "back"), TmpIni->GetFloat("y", "back"));

    glLogics->RegisterGameObject(&m_ciBackButton, myID);

    Initialized = true;

    return true;
}

unsigned int CMenuCredits::Run()
{
    glGraphics->Draw(&m_ciBackground);
    glGraphics->Draw(&m_ciTitle);
    glGraphics->Draw(&m_ciCredits);

    glGraphics->Draw(&m_ciBackButton);

    if (m_ciBackButton.GetClicked())
    {
        return ID_GS_MENU_MAIN;
    }

    return myID;
}
#include "..\..\include\Game\CMenuLevel.h"

CMenuLevel::CMenuLevel() : pwGameState(ID_GS_MENU_LEVEL)
{
    Initialized = false;
}

CMenuLevel::~CMenuLevel()
{
}

bool CMenuLevel::Start() 
{
    if (Initialized)
        return true;

    pwIniFile* TmpIni = glDataBox->GetIniFile("menu_era.ini");

    m_ciBackground.setTexture(*glDataBox->GetTexture(TmpIni->GetString("file", "background")));
    m_ciTitle.setTexture(*glDataBox->GetTexture(TmpIni->GetString("file", "title")));
    m_ciTitle.setOrigin((float)m_ciTitle.getTexture()->getSize().x / 2.0f, (float)m_ciTitle.getTexture()->getSize().y / 2.0f);
    m_ciTitle.setPosition(TmpIni->GetFloat("x", "title"), TmpIni->GetFloat("y", "title"));
    m_ciTitle.setScale(TmpIni->GetFloat("scale", "title"), TmpIni->GetFloat("scale", "title"));
    m_ciTitle.setColor(sf::Color(255, 255, 255, TmpIni->GetInt("opacity", "title")));

    m_ciDarkIdle.setTexture(*glDataBox->GetTexture(TmpIni->GetString("file", "darkage")));
    m_ciDarkHover.setTexture(*m_ciDarkIdle.getTexture());
    m_ciDarkIdle.setColor(sf::Color(255, 255, 255, TmpIni->GetInt("opacity", "darkage")));
    m_ciDarkHover.setColor(sf::Color(255, 255, 255, TmpIni->GetInt("over_opacity", "darkage")));
    m_ciDarkIdle.setOrigin(m_ciDarkIdle.getTextureRect().width / 2.0f, m_ciDarkIdle.getTextureRect().height / 2.0f);
    m_ciDarkHover.setOrigin(m_ciDarkHover.getTextureRect().width / 2.0f, m_ciDarkHover.getTextureRect().height / 2.0f);
    m_ciDarkIdle.setPosition(m_ciDarkIdle.getTextureRect().width / 2.0f, m_ciDarkIdle.getTextureRect().height / 2.0f);
    m_ciDarkHover.setPosition(m_ciDarkHover.getTextureRect().width / 2.0f, m_ciDarkHover.getTextureRect().height / 2.0f);
    m_ciDarkIdle.setScale(TmpIni->GetFloat("scale", "darkage"), TmpIni->GetFloat("scale", "darkage"));
    m_ciDarkHover.setScale(TmpIni->GetFloat("over_scale", "darkage"), TmpIni->GetFloat("over_scale", "darkage"));
    
    m_ciFutureIdle.setTexture(*glDataBox->GetTexture(TmpIni->GetString("file", "future")));
    m_ciFutureHover.setTexture(*m_ciFutureIdle.getTexture());
    m_ciFutureIdle.setColor(sf::Color(255, 255, 255, TmpIni->GetInt("opacity", "future")));
    m_ciFutureHover.setColor(sf::Color(255, 255, 255, TmpIni->GetInt("over_opacity", "future")));
    m_ciFutureIdle.setOrigin(m_ciFutureIdle.getTextureRect().width / 2.0f, m_ciFutureIdle.getTextureRect().height / 2.0f);
    m_ciFutureHover.setOrigin(m_ciFutureHover.getTextureRect().width / 2.0f, m_ciFutureHover.getTextureRect().height / 2.0f);
    m_ciFutureIdle.setPosition(m_ciFutureIdle.getTextureRect().width / 2.0f, m_ciFutureIdle.getTextureRect().height / 2.0f);
    m_ciFutureHover.setPosition(m_ciFutureHover.getTextureRect().width / 2.0f, m_ciFutureHover.getTextureRect().height / 2.0f);
    m_ciFutureIdle.setScale(TmpIni->GetFloat("scale", "future"), TmpIni->GetFloat("scale", "future"));
    m_ciFutureHover.setScale(TmpIni->GetFloat("over_scale", "future"), TmpIni->GetFloat("over_scale", "future"));
    
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

    m_ciDarkCollision.SetSize((float)m_ciDarkIdle.getTexture()->getSize().x, (float)m_ciDarkIdle.getTexture()->getSize().y);
    m_ciFutureCollision.SetSize((float)m_ciFutureIdle.getTexture()->getSize().x, (float)m_ciFutureIdle.getTexture()->getSize().y);
    m_ciBackCollision.SetSize((float)m_ciBacklIdle.getTexture()->getSize().x, (float)m_ciBacklIdle.getTexture()->getSize().y);


    m_ciDarkText.setFont(*glDataBox->GetFont("averia.ttf"));
    m_ciFutureText.setFont(*glDataBox->GetFont("averia.ttf"));
    m_ciBackText.setFont(*glDataBox->GetFont("averia.ttf"));

    m_ciDarkText.setString(TmpIni->GetString("title", "darkage"));
    m_ciFutureText.setString(TmpIni->GetString("title", "future"));
    m_ciBackText.setString(TmpIni->GetString("title", "back"));

    m_ciDarkText.setOrigin(m_ciDarkText.getGlobalBounds().width / 2.0f, (float)m_ciDarkText.getCharacterSize() / 2.0f);
    m_ciFutureText.setOrigin(m_ciFutureText.getGlobalBounds().width / 2.0f, (float)m_ciFutureText.getCharacterSize() / 2.0f);
    m_ciBackText.setOrigin(m_ciBackText.getGlobalBounds().width / 2.0f, (float)m_ciBackText.getCharacterSize() / 2.0f);

    m_ciDarkText.setPosition(m_ciDarkCollision.GetSize().x / 2.0f, m_ciDarkCollision.GetSize().y / 2.0f - (float)m_ciDarkText.getCharacterSize() / 4.0f);
    m_ciFutureText.setPosition(m_ciFutureCollision.GetSize().x / 2.0f, m_ciFutureCollision.GetSize().y / 2.0f - (float)m_ciFutureText.getCharacterSize() / 4.0f);
    m_ciBackText.setPosition(m_ciBackCollision.GetSize().x / 2.0f, m_ciBackCollision.GetSize().y / 2.0f - (float)m_ciBackText.getCharacterSize() / 4.0f);

    m_ciDarkButton.AddCollisionObject(0, &m_ciDarkCollision);
    m_ciDarkButton.AddDrawableObject(0, &m_ciDarkIdle);
    m_ciDarkButton.AddDrawableObject(1, &m_ciDarkHover);
    m_ciDarkButton.AddDrawableObject(2, &m_ciDarkHover);
    m_ciDarkButton.AddDrawableObject(3, &m_ciDarkText);
    m_ciDarkButton.BindDrawableToState(0, pwGUI::pwButton::Idle);
    m_ciDarkButton.BindDrawableToState(1, pwGUI::pwButton::Hover);
    m_ciDarkButton.BindDrawableToState(2, pwGUI::pwButton::Down);
    m_ciDarkButton.setPosition(TmpIni->GetFloat("x", "darkage"), TmpIni->GetFloat("y", "darkage"));

    m_ciFutureButton.AddCollisionObject(0, &m_ciFutureCollision);
    m_ciFutureButton.AddDrawableObject(0, &m_ciFutureIdle);
    m_ciFutureButton.AddDrawableObject(1, &m_ciFutureHover);
    m_ciFutureButton.AddDrawableObject(2, &m_ciFutureHover);
    m_ciFutureButton.AddDrawableObject(3, &m_ciFutureText);
    m_ciFutureButton.BindDrawableToState(0, pwGUI::pwButton::Idle);
    m_ciFutureButton.BindDrawableToState(1, pwGUI::pwButton::Hover);
    m_ciFutureButton.BindDrawableToState(2, pwGUI::pwButton::Down);
    m_ciFutureButton.setPosition(TmpIni->GetFloat("x", "future"), TmpIni->GetFloat("y", "future"));

    m_ciBackButton.AddCollisionObject(0, &m_ciBackCollision);
    m_ciBackButton.AddDrawableObject(0, &m_ciBacklIdle);
    m_ciBackButton.AddDrawableObject(1, &m_ciBackHover);
    m_ciBackButton.AddDrawableObject(2, &m_ciBackHover);
    m_ciBackButton.AddDrawableObject(3, &m_ciBackText);
    m_ciBackButton.BindDrawableToState(0, pwGUI::pwButton::Idle);
    m_ciBackButton.BindDrawableToState(1, pwGUI::pwButton::Hover);
    m_ciBackButton.BindDrawableToState(2, pwGUI::pwButton::Down);
    m_ciBackButton.setPosition(TmpIni->GetFloat("x", "back"), TmpIni->GetFloat("y", "back"));

    glLogics->RegisterGameObject(&m_ciDarkButton, myID);
    glLogics->RegisterGameObject(&m_ciFutureButton, myID);
    glLogics->RegisterGameObject(&m_ciBackButton, myID);

    Initialized = true;

    return true;
}

unsigned int CMenuLevel::Run()
{
    glGraphics->Draw(&m_ciBackground);
    glGraphics->Draw(&m_ciTitle);
    
    glGraphics->Draw(&m_ciDarkButton);
    glGraphics->Draw(&m_ciFutureButton);
    glGraphics->Draw(&m_ciBackButton);

    if (m_ciDarkButton.GetClicked())
    {
        CLevelData::setCurrentLevelString("darkage", true);
        return ID_GS_PLAY;
    }
    
    else if (m_ciFutureButton.GetClicked())
    {
        CLevelData::setCurrentLevelString("future", true);
        return ID_GS_PLAY;
    }

    else if (m_ciBackButton.GetClicked())
    {
        return ID_GS_MENU_MAIN;
    }

    return myID;
}

bool CMenuLevel::Close()
{
    /*glGraphics->Draw(&m_ciBackground);
    glGraphics->Draw(&m_ciMenuWrapper);

    if (m_ciMenuWrapper.getRotation() >= 90.0f)
    {
        return true;
    }
    else
    {
        m_ciMenuWrapper.rotate(glGraphics->GetLastFrameTime() * 45.0f);
        return false;
    }*/

    return true;
}

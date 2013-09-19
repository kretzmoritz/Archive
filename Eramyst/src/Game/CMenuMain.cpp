#include "..\..\include\Game\CMenuMain.h"

CMenuMain::CMenuMain() : pwGameState(ID_GS_MENU_MAIN)
{
    Initialized = false;
}

CMenuMain::~CMenuMain()
{
}

bool CMenuMain::Start() 
{
    if (Initialized)
        return true;

    if (!glSystem->LoadDataList("main", "game_load.pws", "default", ID_GS_LOAD))
        return 0;

    pwIniFile* TmpIni = glDataBox->GetIniFile("menu_main.ini");

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

    m_ciLevelIdle.setTexture(*glDataBox->GetTexture(TmpIni->GetString("file", "level")));
    m_ciLevelHover.setTexture(*m_ciLevelIdle.getTexture());
    m_ciLevelIdle.setColor(sf::Color(255, 255, 255, TmpIni->GetInt("opacity", "level")));
    m_ciLevelHover.setColor(sf::Color(255, 255, 255, TmpIni->GetInt("over_opacity", "level")));
    m_ciLevelIdle.setOrigin(m_ciLevelIdle.getTextureRect().width / 2.0f, m_ciLevelIdle.getTextureRect().height / 2.0f);
    m_ciLevelHover.setOrigin(m_ciLevelHover.getTextureRect().width / 2.0f, m_ciLevelHover.getTextureRect().height / 2.0f);
    m_ciLevelIdle.setPosition(m_ciLevelIdle.getTextureRect().width / 2.0f, m_ciLevelIdle.getTextureRect().height / 2.0f);
    m_ciLevelHover.setPosition(m_ciLevelHover.getTextureRect().width / 2.0f, m_ciLevelHover.getTextureRect().height / 2.0f);
    m_ciLevelIdle.setScale(TmpIni->GetFloat("scale", "level"), TmpIni->GetFloat("scale", "level"));
    m_ciLevelHover.setScale(TmpIni->GetFloat("scale", "level"), TmpIni->GetFloat("scale", "level"));

    m_ciTutorialIdle.setTexture(*glDataBox->GetTexture(TmpIni->GetString("file", "tutorial")));
    m_ciTutorialHover.setTexture(*m_ciTutorialIdle.getTexture());
    m_ciTutorialIdle.setColor(sf::Color(255, 255, 255, TmpIni->GetInt("opacity", "tutorial")));
    m_ciTutorialHover.setColor(sf::Color(255, 255, 255, TmpIni->GetInt("over_opacity", "tutorial")));
    m_ciTutorialIdle.setOrigin(m_ciTutorialIdle.getTextureRect().width / 2.0f, m_ciTutorialIdle.getTextureRect().height / 2.0f);
    m_ciTutorialHover.setOrigin(m_ciTutorialHover.getTextureRect().width / 2.0f, m_ciTutorialHover.getTextureRect().height / 2.0f);
    m_ciTutorialIdle.setPosition(m_ciTutorialIdle.getTextureRect().width / 2.0f, m_ciTutorialIdle.getTextureRect().height / 2.0f);
    m_ciTutorialHover.setPosition(m_ciTutorialHover.getTextureRect().width / 2.0f, m_ciTutorialHover.getTextureRect().height / 2.0f);
    m_ciTutorialIdle.setScale(TmpIni->GetFloat("scale", "tutorial"), TmpIni->GetFloat("scale", "tutorial"));
    m_ciTutorialHover.setScale(TmpIni->GetFloat("scale", "tutorial"), TmpIni->GetFloat("scale", "tutorial"));

    m_ciOptionsIdle.setTexture(*glDataBox->GetTexture(TmpIni->GetString("file", "options")));
    m_ciOptionsHover.setTexture(*m_ciOptionsIdle.getTexture());
    m_ciOptionsIdle.setColor(sf::Color(255, 255, 255, TmpIni->GetInt("opacity", "options")));
    m_ciOptionsHover.setColor(sf::Color(255, 255, 255, TmpIni->GetInt("over_opacity", "options")));
    m_ciOptionsIdle.setOrigin(m_ciOptionsIdle.getTextureRect().width / 2.0f, m_ciOptionsIdle.getTextureRect().height / 2.0f);
    m_ciOptionsHover.setOrigin(m_ciOptionsHover.getTextureRect().width / 2.0f, m_ciOptionsHover.getTextureRect().height / 2.0f);
    m_ciOptionsIdle.setPosition(m_ciOptionsIdle.getTextureRect().width / 2.0f, m_ciOptionsIdle.getTextureRect().height / 2.0f);
    m_ciOptionsHover.setPosition(m_ciOptionsHover.getTextureRect().width / 2.0f, m_ciOptionsHover.getTextureRect().height / 2.0f);
    m_ciOptionsIdle.setScale(TmpIni->GetFloat("scale", "options"), TmpIni->GetFloat("scale", "options"));
    m_ciOptionsHover.setScale(TmpIni->GetFloat("scale", "options"), TmpIni->GetFloat("scale", "options"));

    m_ciHighscoreIdle.setTexture(*glDataBox->GetTexture(TmpIni->GetString("file", "highscore")));
    m_ciHighscoreHover.setTexture(*m_ciHighscoreIdle.getTexture());
    m_ciHighscoreIdle.setColor(sf::Color(255, 255, 255, TmpIni->GetInt("opacity", "highscore")));
    m_ciHighscoreHover.setColor(sf::Color(255, 255, 255, TmpIni->GetInt("over_opacity", "highscore")));
    m_ciHighscoreIdle.setOrigin(m_ciHighscoreIdle.getTextureRect().width / 2.0f, m_ciHighscoreIdle.getTextureRect().height / 2.0f);
    m_ciHighscoreHover.setOrigin(m_ciHighscoreHover.getTextureRect().width / 2.0f, m_ciHighscoreHover.getTextureRect().height / 2.0f);
    m_ciHighscoreIdle.setPosition(m_ciHighscoreIdle.getTextureRect().width / 2.0f, m_ciHighscoreIdle.getTextureRect().height / 2.0f);
    m_ciHighscoreHover.setPosition(m_ciHighscoreHover.getTextureRect().width / 2.0f, m_ciHighscoreHover.getTextureRect().height / 2.0f);
    m_ciHighscoreIdle.setScale(TmpIni->GetFloat("scale", "highscore"), TmpIni->GetFloat("scale", "highscore"));
    m_ciHighscoreHover.setScale(TmpIni->GetFloat("scale", "highscore"), TmpIni->GetFloat("scale", "highscore"));

    m_ciQuitIdle.setTexture(*glDataBox->GetTexture(TmpIni->GetString("file", "quit")));
    m_ciQuitHover.setTexture(*m_ciQuitIdle.getTexture());
    m_ciQuitIdle.setColor(sf::Color(255, 255, 255, TmpIni->GetInt("opacity", "quit")));
    m_ciQuitHover.setColor(sf::Color(255, 255, 255, TmpIni->GetInt("over_opacity", "quit")));
    m_ciQuitIdle.setOrigin(m_ciQuitIdle.getTextureRect().width / 2.0f, m_ciQuitIdle.getTextureRect().height / 2.0f);
    m_ciQuitHover.setOrigin(m_ciQuitHover.getTextureRect().width / 2.0f, m_ciQuitHover.getTextureRect().height / 2.0f);
    m_ciQuitIdle.setPosition(m_ciQuitIdle.getTextureRect().width / 2.0f, m_ciQuitIdle.getTextureRect().height / 2.0f);
    m_ciQuitHover.setPosition(m_ciQuitHover.getTextureRect().width / 2.0f, m_ciQuitHover.getTextureRect().height / 2.0f);
    m_ciQuitIdle.setScale(TmpIni->GetFloat("scale", "quit"), TmpIni->GetFloat("scale", "quit"));
    m_ciQuitHover.setScale(TmpIni->GetFloat("scale", "quit"), TmpIni->GetFloat("scale", "quit"));

    m_ciCreditsIdle.setTexture(*glDataBox->GetTexture(TmpIni->GetString("file", "credits")));
    m_ciCreditsHover.setTexture(*m_ciCreditsIdle.getTexture());
    m_ciCreditsIdle.setColor(sf::Color(255, 255, 255, TmpIni->GetInt("opacity", "credits")));
    m_ciCreditsHover.setColor(sf::Color(255, 255, 255, TmpIni->GetInt("over_opacity", "credits")));
    m_ciCreditsIdle.setOrigin(m_ciCreditsIdle.getTextureRect().width / 2.0f, m_ciCreditsIdle.getTextureRect().height / 2.0f);
    m_ciCreditsHover.setOrigin(m_ciCreditsHover.getTextureRect().width / 2.0f, m_ciCreditsHover.getTextureRect().height / 2.0f);
    m_ciCreditsIdle.setPosition(m_ciCreditsIdle.getTextureRect().width / 2.0f, m_ciCreditsIdle.getTextureRect().height / 2.0f);
    m_ciCreditsHover.setPosition(m_ciCreditsHover.getTextureRect().width / 2.0f, m_ciCreditsHover.getTextureRect().height / 2.0f);
    m_ciCreditsIdle.setScale(TmpIni->GetFloat("scale", "credits"), TmpIni->GetFloat("scale", "credits"));
    m_ciCreditsHover.setScale(TmpIni->GetFloat("scale", "credits"), TmpIni->GetFloat("scale", "credits"));
    

    m_ciPlayText.setFont(*glDataBox->GetFont("averia.ttf"));
    m_ciLevelText.setFont(*glDataBox->GetFont("averia.ttf"));
    m_ciTutorialText.setFont(*glDataBox->GetFont("averia.ttf"));
    m_ciOptionsText.setFont(*glDataBox->GetFont("averia.ttf"));
    m_ciHighscoreText.setFont(*glDataBox->GetFont("averia.ttf"));
    m_ciQuitText.setFont(*glDataBox->GetFont("averia.ttf"));
    m_ciCreditsText.setFont(*glDataBox->GetFont("averia.ttf"));

    m_ciPlayText.setString(TmpIni->GetString("title", "play"));
    m_ciLevelText.setString(TmpIni->GetString("title", "level"));
    m_ciTutorialText.setString(TmpIni->GetString("title", "tutorial"));
    m_ciOptionsText.setString(TmpIni->GetString("title", "options"));
    m_ciHighscoreText.setString(TmpIni->GetString("title", "highscore"));
    m_ciQuitText.setString(TmpIni->GetString("title", "quit"));
    m_ciCreditsText.setString(TmpIni->GetString("title", "credits"));

    m_ciPlayText.setOrigin(m_ciPlayText.getGlobalBounds().width / 2.0f, (float)m_ciPlayText.getCharacterSize() / 2.0f);
    m_ciLevelText.setOrigin(m_ciLevelText.getGlobalBounds().width / 2.0f, (float)m_ciLevelText.getCharacterSize() / 2.0f);
    m_ciTutorialText.setOrigin(m_ciTutorialText.getGlobalBounds().width / 2.0f, (float)m_ciTutorialText.getCharacterSize() / 2.0f);
    m_ciOptionsText.setOrigin(m_ciOptionsText.getGlobalBounds().width / 2.0f, (float)m_ciOptionsText.getCharacterSize() / 2.0f);
    m_ciHighscoreText.setOrigin(m_ciHighscoreText.getGlobalBounds().width / 2.0f, (float)m_ciHighscoreText.getCharacterSize() / 2.0f);
    m_ciQuitText.setOrigin(m_ciQuitText.getGlobalBounds().width / 2.0f, (float)m_ciQuitText.getCharacterSize() / 2.0f);
    m_ciCreditsText.setOrigin(m_ciCreditsText.getGlobalBounds().width / 2.0f, (float)m_ciCreditsText.getCharacterSize() / 2.0f);

    m_ciPlayCollision.SetSize((float)m_ciPlayIdle.getTexture()->getSize().x, (float)m_ciPlayIdle.getTexture()->getSize().y);
    m_ciLevelCollision.SetSize((float)m_ciLevelIdle.getTexture()->getSize().x, (float)m_ciLevelIdle.getTexture()->getSize().y);
    m_ciTutorialCollision.SetSize((float)m_ciTutorialIdle.getTexture()->getSize().x, (float)m_ciTutorialIdle.getTexture()->getSize().y);
    m_ciOptionsCollision.SetSize((float)m_ciOptionsIdle.getTexture()->getSize().x, (float)m_ciOptionsIdle.getTexture()->getSize().y);
    m_ciHighscoreCollision.SetSize((float)m_ciHighscoreIdle.getTexture()->getSize().x, (float)m_ciHighscoreIdle.getTexture()->getSize().y);
    m_ciQuitCollision.SetSize((float)m_ciQuitIdle.getTexture()->getSize().x, (float)m_ciQuitIdle.getTexture()->getSize().y);
    m_ciCreditsCollision.SetSize((float)m_ciCreditsIdle.getTexture()->getSize().x, (float)m_ciCreditsIdle.getTexture()->getSize().y);

    m_ciPlayText.setPosition(m_ciPlayCollision.GetSize().x / 2.0f, m_ciPlayCollision.GetSize().y / 2.0f - (float)m_ciPlayText.getCharacterSize() / 4.0f);
    m_ciLevelText.setPosition(m_ciLevelCollision.GetSize().x / 2.0f, m_ciLevelCollision.GetSize().y / 2.0f - (float)m_ciLevelText.getCharacterSize() / 4.0f);
    m_ciTutorialText.setPosition(m_ciTutorialCollision.GetSize().x / 2.0f, m_ciTutorialCollision.GetSize().y / 2.0f - (float)m_ciTutorialText.getCharacterSize() / 4.0f);
    m_ciOptionsText.setPosition(m_ciOptionsCollision.GetSize().x / 2.0f, m_ciOptionsCollision.GetSize().y / 2.0f - (float)m_ciOptionsText.getCharacterSize() / 4.0f);
    m_ciHighscoreText.setPosition(m_ciHighscoreCollision.GetSize().x / 2.0f, m_ciHighscoreCollision.GetSize().y / 2.0f - (float)m_ciHighscoreText.getCharacterSize() / 4.0f);
    m_ciQuitText.setPosition(m_ciQuitCollision.GetSize().x / 2.0f, m_ciQuitCollision.GetSize().y / 2.0f - (float)m_ciQuitText.getCharacterSize() / 4.0f);
    m_ciCreditsText.setPosition(m_ciCreditsCollision.GetSize().x / 2.0f, m_ciCreditsCollision.GetSize().y / 2.0f - (float)m_ciCreditsText.getCharacterSize() / 4.0f);
   
    m_ciPlayButton.AddCollisionObject(0, &m_ciPlayCollision);
    m_ciPlayButton.AddDrawableObject(0, &m_ciPlayIdle);
    m_ciPlayButton.AddDrawableObject(1, &m_ciPlayHover);
    m_ciPlayButton.AddDrawableObject(2, &m_ciPlayHover);
    m_ciPlayButton.AddDrawableObject(3, &m_ciPlayText);
    m_ciPlayButton.BindDrawableToState(0, pwGUI::pwButton::Idle);
    m_ciPlayButton.BindDrawableToState(1, pwGUI::pwButton::Hover);
    m_ciPlayButton.BindDrawableToState(2, pwGUI::pwButton::Down);
    m_ciPlayButton.setPosition(TmpIni->GetFloat("x", "play"), TmpIni->GetFloat("y", "play"));

    m_ciLevelButton.AddCollisionObject(0, &m_ciLevelCollision);
    m_ciLevelButton.AddDrawableObject(0, &m_ciLevelIdle);
    m_ciLevelButton.AddDrawableObject(1, &m_ciLevelHover);
    m_ciLevelButton.AddDrawableObject(2, &m_ciLevelHover);
    m_ciLevelButton.AddDrawableObject(3, &m_ciLevelText);
    m_ciLevelButton.BindDrawableToState(0, pwGUI::pwButton::Idle);
    m_ciLevelButton.BindDrawableToState(1, pwGUI::pwButton::Hover);
    m_ciLevelButton.BindDrawableToState(2, pwGUI::pwButton::Down);
    m_ciLevelButton.setPosition(TmpIni->GetFloat("x", "level"), TmpIni->GetFloat("y", "level"));

    m_ciTutorialButton.AddCollisionObject(0, &m_ciTutorialCollision);
    m_ciTutorialButton.AddDrawableObject(0, &m_ciTutorialIdle);
    m_ciTutorialButton.AddDrawableObject(1, &m_ciTutorialHover);
    m_ciTutorialButton.AddDrawableObject(2, &m_ciTutorialHover);
    m_ciTutorialButton.AddDrawableObject(3, &m_ciTutorialText);
    m_ciTutorialButton.BindDrawableToState(0, pwGUI::pwButton::Idle);
    m_ciTutorialButton.BindDrawableToState(1, pwGUI::pwButton::Hover);
    m_ciTutorialButton.BindDrawableToState(2, pwGUI::pwButton::Down);
    m_ciTutorialButton.setPosition(TmpIni->GetFloat("x", "tutorial"), TmpIni->GetFloat("y", "tutorial"));

    m_ciOptionsButton.AddCollisionObject(0, &m_ciOptionsCollision);
    m_ciOptionsButton.AddDrawableObject(0, &m_ciOptionsIdle);
    m_ciOptionsButton.AddDrawableObject(1, &m_ciOptionsHover);
    m_ciOptionsButton.AddDrawableObject(2, &m_ciOptionsHover);
    m_ciOptionsButton.AddDrawableObject(3, &m_ciOptionsText);
    m_ciOptionsButton.BindDrawableToState(0, pwGUI::pwButton::Idle);
    m_ciOptionsButton.BindDrawableToState(1, pwGUI::pwButton::Hover);
    m_ciOptionsButton.BindDrawableToState(2, pwGUI::pwButton::Down);
    m_ciOptionsButton.setPosition(TmpIni->GetFloat("x", "options"), TmpIni->GetFloat("y", "options"));

    m_ciHighscoreButton.AddCollisionObject(0, &m_ciHighscoreCollision);
    m_ciHighscoreButton.AddDrawableObject(0, &m_ciHighscoreIdle);
    m_ciHighscoreButton.AddDrawableObject(1, &m_ciHighscoreHover);
    m_ciHighscoreButton.AddDrawableObject(2, &m_ciHighscoreHover);
    m_ciHighscoreButton.AddDrawableObject(3, &m_ciHighscoreText);
    m_ciHighscoreButton.BindDrawableToState(0, pwGUI::pwButton::Idle);
    m_ciHighscoreButton.BindDrawableToState(1, pwGUI::pwButton::Hover);
    m_ciHighscoreButton.BindDrawableToState(2, pwGUI::pwButton::Down);
    m_ciHighscoreButton.setPosition(TmpIni->GetFloat("x", "highscore"), TmpIni->GetFloat("y", "highscore"));

    m_ciQuitButton.AddCollisionObject(0, &m_ciQuitCollision);
    m_ciQuitButton.AddDrawableObject(0, &m_ciQuitIdle);
    m_ciQuitButton.AddDrawableObject(1, &m_ciQuitHover);
    m_ciQuitButton.AddDrawableObject(2, &m_ciQuitHover);
    m_ciQuitButton.AddDrawableObject(3, &m_ciQuitText);
    m_ciQuitButton.BindDrawableToState(0, pwGUI::pwButton::Idle);
    m_ciQuitButton.BindDrawableToState(1, pwGUI::pwButton::Hover);
    m_ciQuitButton.BindDrawableToState(2, pwGUI::pwButton::Down);
    m_ciQuitButton.setPosition(TmpIni->GetFloat("x", "quit"), TmpIni->GetFloat("y", "quit"));

    m_ciCreditsButton.AddCollisionObject(0, &m_ciCreditsCollision);
    m_ciCreditsButton.AddDrawableObject(0, &m_ciCreditsIdle);
    m_ciCreditsButton.AddDrawableObject(1, &m_ciCreditsHover);
    m_ciCreditsButton.AddDrawableObject(2, &m_ciCreditsHover);
    m_ciCreditsButton.AddDrawableObject(3, &m_ciCreditsText);
    m_ciCreditsButton.BindDrawableToState(0, pwGUI::pwButton::Idle);
    m_ciCreditsButton.BindDrawableToState(1, pwGUI::pwButton::Hover);
    m_ciCreditsButton.BindDrawableToState(2, pwGUI::pwButton::Down);
    m_ciCreditsButton.setPosition(TmpIni->GetFloat("x", "credits"), TmpIni->GetFloat("y", "credits"));
    
    glLogics->RegisterGameObject(&m_ciPlayButton, myID);
    glLogics->RegisterGameObject(&m_ciLevelButton, myID);
    glLogics->RegisterGameObject(&m_ciTutorialButton, myID);
    glLogics->RegisterGameObject(&m_ciOptionsButton, myID);
    glLogics->RegisterGameObject(&m_ciHighscoreButton, myID);
    glLogics->RegisterGameObject(&m_ciQuitButton, myID);
    glLogics->RegisterGameObject(&m_ciCreditsButton, myID);

    m_ciMenuWrapper.AddDrawableObject(0, &m_ciPlayButton);
    m_ciMenuWrapper.AddDrawableObject(1, &m_ciLevelButton);
    m_ciMenuWrapper.AddDrawableObject(2, &m_ciTutorialButton);
    m_ciMenuWrapper.AddDrawableObject(3, &m_ciOptionsButton);
    m_ciMenuWrapper.AddDrawableObject(4, &m_ciHighscoreButton);
    m_ciMenuWrapper.AddDrawableObject(5, &m_ciQuitButton);
    m_ciMenuWrapper.AddDrawableObject(6, &m_ciCreditsButton);
    m_ciMenuWrapper.setOrigin(sf::Vector2f((float)pwEngineOptions::GetInternGraphicsSize().x / 2.0f, (float)pwEngineOptions::GetInternGraphicsSize().y + (float)pwEngineOptions::GetInternGraphicsSize().x / 2.0f));
    m_ciMenuWrapper.setPosition(((float)pwEngineOptions::GetInternGraphicsSize().x / 2.0f), ((float)pwEngineOptions::GetInternGraphicsSize().y + (float)pwEngineOptions::GetInternGraphicsSize().x / 2.0f));
    m_ciMenuWrapper.setRotation(0.0f);

    /*tmp.setAnimation("animation\\coloss_torso_walk\\torso");
    tmp.SetTotalFrames(60);
    tmp.SetFramesPerSecond(10.0f);
    tmp.Load();*/

    Initialized = true;

    return true;
}

unsigned int CMenuMain::Run()
{
    m_ciBackgroundObject00.rotate(m_fBackground00Rotation * glGraphics->GetLastFrameTime());
    m_ciBackgroundObject01.rotate(m_fBackground01Rotation * glGraphics->GetLastFrameTime());

    glGraphics->Draw(&m_ciBackground);
    glGraphics->Draw(&m_ciBackgroundObject00);
    glGraphics->Draw(&m_ciBackgroundObject01);
    glGraphics->Draw(&m_ciTitle);

    //if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
    //    glGraphics->Draw(&tmp);

    /*glGraphics->Draw(&m_ciPlayButton);
    glGraphics->Draw(&m_ciLevelButton);
    glGraphics->Draw(&m_ciTutorialButton);
    glGraphics->Draw(&m_ciQuitButton);*/
    glGraphics->Draw(&m_ciMenuWrapper);

    if (m_ciPlayButton.GetClicked())
    {
        CLevelData::setCurrentLevelString("darkage");
        return ID_GS_TRAILER;
    }
    
    else if (m_ciLevelButton.GetClicked())
    {
        return ID_GS_MENU_LEVEL;
    }

    else if (m_ciTutorialButton.GetClicked())
    {
        glGLog << pwLogType::pwLTInfo << "Tutorial Button Clicked" << pwGraphicsLog::flush;
    }

    else if (m_ciHighscoreButton.GetClicked())
    {
        return ID_GS_HIGHSCORE;
    }

    else if (m_ciOptionsButton.GetClicked())
    {
        return ID_GS_MENU_OPTIONS;
    }

    else if (m_ciCreditsButton.GetClicked())
    {
        return ID_GS_MENU_CREDITS;
    }

    else if (m_ciQuitButton.GetClicked())
    {
        return ID_GS_QUIT;
    }

    return myID;
}

bool CMenuMain::Close()
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

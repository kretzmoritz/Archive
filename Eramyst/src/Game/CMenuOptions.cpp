#include "..\..\include\Game\CMenuOptions.h"

CMenuOptions::CMenuOptions() : pwGameState(ID_GS_MENU_OPTIONS)
{
    Initialized = false;
}

CMenuOptions::~CMenuOptions()
{
}

bool CMenuOptions::Start() 
{
    std::stringstream TmpSStream;
    std::string TmpString;

    if (!Initialized)
    {
        pwIniFile* TmpIni = glDataBox->GetIniFile("menu_options.ini");

        m_ciBackground.setTexture(*glDataBox->GetTexture(TmpIni->GetString("file", "background")));

        m_ciKeyAssignBackground.setTexture(*glDataBox->GetTexture(TmpIni->GetString("file", "keyassign")));
        m_ciKeyAssignBackground.setOrigin((float)m_ciKeyAssignBackground.getTexture()->getSize().x / 2.0f, (float)m_ciKeyAssignBackground.getTexture()->getSize().y / 2.0f);
        m_ciKeyAssignBackground.setPosition((float)pwEngineOptions::GetInternGraphicsSize().x / 2.0f, (float)pwEngineOptions::GetInternGraphicsSize().y / 2.0f);
        m_ciKeyAssignBGShape.setSize(sf::Vector2f((float)pwEngineOptions::GetInternGraphicsSize().x, (float)pwEngineOptions::GetInternGraphicsSize().y));
        m_ciKeyAssignBGShape.setFillColor(sf::Color(0, 0, 0, 210));

        m_ciTitle.setTexture(*glDataBox->GetTexture(TmpIni->GetString("file", "title")));
        m_ciTitle.setOrigin((float)m_ciTitle.getTexture()->getSize().x / 2.0f, (float)m_ciTitle.getTexture()->getSize().y / 2.0f);
        m_ciTitle.setPosition(TmpIni->GetFloat("x", "title"), TmpIni->GetFloat("y", "title"));
        m_ciTitle.setScale(TmpIni->GetFloat("scale", "title"), TmpIni->GetFloat("scale", "title"));
        m_ciTitle.setColor(sf::Color(255, 255, 255, TmpIni->GetInt("opacity", "title")));


        m_aVideoModes.reserve(sf::VideoMode::getFullscreenModes().size());
    
        for (auto i = sf::VideoMode::getFullscreenModes().begin(); i != sf::VideoMode::getFullscreenModes().end(); i++)
        {
            if (i->bitsPerPixel == 32)
            {
                m_aVideoModes.push_back(*i);
            }
        }

        m_ciGraphicsSlider.setTexture(*glDataBox->GetTexture(TmpIni->GetString("file", "graphics")));
        m_ciGraphics.SetSprite(&m_ciGraphicsSlider);
        m_ciGraphics.SetSize(sf::Vector2i(m_ciGraphicsSlider.getTexture()->getSize().x, TmpIni->GetInt("ysize", "graphics")));
        m_ciGraphics.SetSnapSize(sf::Vector2i(TmpIni->GetInt("xsnapsize", "graphics"), TmpIni->GetInt("ysize", "graphics")));
        m_ciGraphics.setPosition(TmpIni->GetFloat("x", "graphics"), TmpIni->GetFloat("y", "graphics"));
        m_ciGraphics.SetIndexCount(m_aVideoModes.size());

        m_ciAudioSlider.setTexture(*glDataBox->GetTexture(TmpIni->GetString("file", "sound")));
        m_ciAudio.SetSprite(&m_ciAudioSlider);
        m_ciAudio.SetSize(sf::Vector2i(m_ciAudioSlider.getTexture()->getSize().x, TmpIni->GetInt("ysize", "sound")));
        m_ciAudio.setPosition(TmpIni->GetFloat("x", "sound"), TmpIni->GetFloat("y", "sound"));

        m_ciMusicSlider.setTexture(*glDataBox->GetTexture(TmpIni->GetString("file", "music")));
        m_ciMusic.SetSprite(&m_ciMusicSlider);
        m_ciMusic.SetSize(sf::Vector2i(m_ciMusicSlider.getTexture()->getSize().x, TmpIni->GetInt("ysize", "music")));
        m_ciMusic.setPosition(TmpIni->GetFloat("x", "music"), TmpIni->GetFloat("y", "music"));

        m_ciAudioText.setFont(*glDataBox->GetFont("averia.ttf"));
        m_ciAudioText.setString(TmpIni->GetString("label", "sound"));
        m_ciAudioText.setColor(sf::Color());
        m_ciAudioText.setOrigin(m_ciAudioText.getGlobalBounds().width, (float)m_ciAudioText.getCharacterSize() / 2.0f);
        m_ciAudioText.setPosition(TmpIni->GetFloat("x", "sound") -20.0f, TmpIni->GetFloat("y", "sound"));

        m_ciMusicText.setFont(*glDataBox->GetFont("averia.ttf"));
        m_ciMusicText.setString(TmpIni->GetString("label", "music"));
        m_ciMusicText.setColor(sf::Color());
        m_ciMusicText.setOrigin(m_ciMusicText.getGlobalBounds().width, (float)m_ciMusicText.getCharacterSize() / 2.0f);
        m_ciMusicText.setPosition(TmpIni->GetFloat("x", "music") -20.0f, TmpIni->GetFloat("y", "music"));

        m_ciGraphicsText.setFont(*glDataBox->GetFont("averia.ttf"));
        m_ciGraphicsText.setString(TmpIni->GetString("label", "graphics"));
        m_ciGraphicsText.setColor(sf::Color());
        m_ciGraphicsText.setOrigin(m_ciGraphicsText.getGlobalBounds().width, (float)m_ciGraphicsText.getCharacterSize() / 2.0f);
        m_ciGraphicsText.setPosition(TmpIni->GetFloat("x", "graphics") -20.0f, TmpIni->GetFloat("y", "graphics"));

        m_ciFullscreenText.setFont(*glDataBox->GetFont("averia.ttf"));
        m_ciFullscreenText.setString(TmpIni->GetString("label", "fullscreen"));
        m_ciFullscreenText.setColor(sf::Color());
        m_ciFullscreenText.setOrigin(m_ciFullscreenText.getGlobalBounds().width, (float)m_ciFullscreenText.getCharacterSize() / 2.0f);
        m_ciFullscreenText.setPosition(TmpIni->GetFloat("x", "fullscreen") -20.0f, TmpIni->GetFloat("y", "fullscreen"));

        m_ciControlsLabel.setFont(*glDataBox->GetFont("averia.ttf"));
        m_ciControlsLabel.setString(TmpIni->GetString("label", "controls"));
        m_ciControlsLabel.setColor(sf::Color());
        m_ciControlsLabel.setOrigin(m_ciControlsLabel.getGlobalBounds().width, (float)m_ciControlsLabel.getCharacterSize() / 2.0f);
        m_ciControlsLabel.setPosition(TmpIni->GetFloat("x", "controls") -20.0f, TmpIni->GetFloat("y", "controls") + ((float)glDataBox->GetTexture(TmpIni->GetString("file", "controls"))->getSize().y-20.0f)/2.0f);

        m_aLayerNames[0].setFont(*glDataBox->GetFont("averia.ttf"));
        m_aLayerNames[1].setFont(*glDataBox->GetFont("averia.ttf"));
        m_aLayerNames[2].setFont(*glDataBox->GetFont("averia.ttf"));
        m_aLayerNames[3].setFont(*glDataBox->GetFont("averia.ttf"));
        m_aLayerNames[4].setFont(*glDataBox->GetFont("averia.ttf"));
        m_aLayerNames[0].setColor(sf::Color(255, 255, 255, 255));
        m_aLayerNames[1].setColor(sf::Color(255, 255, 255, 255));
        m_aLayerNames[2].setColor(sf::Color(255, 255, 255, 255));
        m_aLayerNames[3].setColor(sf::Color(255, 255, 255, 255));
        m_aLayerNames[4].setColor(sf::Color(255, 255, 255, 255));
        m_aLayerNames[0].setString(TmpIni->GetString("0layer", "keyassign"));
        m_aLayerNames[1].setString(TmpIni->GetString("1layer", "keyassign"));
        m_aLayerNames[2].setString(TmpIni->GetString("2layer", "keyassign"));
        m_aLayerNames[3].setString(TmpIni->GetString("3layer", "keyassign"));
        m_aLayerNames[4].setString(TmpIni->GetString("4layer", "keyassign"));
        m_aLayerNames[0].setPosition(655.0f, 405);
        m_aLayerNames[1].setPosition(655.0f, 465);
        m_aLayerNames[2].setPosition(655.0f, 525);
        m_aLayerNames[3].setPosition(655.0f, 585);
        m_aLayerNames[4].setPosition(655.0f, 645);
        m_aKeyNames[0].setFont(*glDataBox->GetFont("averia.ttf"));
        m_aKeyNames[1].setFont(*glDataBox->GetFont("averia.ttf"));
        m_aKeyNames[2].setFont(*glDataBox->GetFont("averia.ttf"));
        m_aKeyNames[3].setFont(*glDataBox->GetFont("averia.ttf"));
        m_aKeyNames[4].setFont(*glDataBox->GetFont("averia.ttf"));
        m_aKeyNames[0].setColor(sf::Color(255, 255, 255, 255));
        m_aKeyNames[1].setColor(sf::Color(255, 255, 255, 255));
        m_aKeyNames[2].setColor(sf::Color(255, 255, 255, 255));
        m_aKeyNames[3].setColor(sf::Color(255, 255, 255, 255));
        m_aKeyNames[4].setColor(sf::Color(255, 255, 255, 255));
        m_aKeyNames[0].setPosition(1122.5f, 405.0f);
        m_aKeyNames[1].setPosition(1122.5f, 465.0f);
        m_aKeyNames[2].setPosition(1122.5f, 525.0f);
        m_aKeyNames[3].setPosition(1122.5f, 585.0f);
        m_aKeyNames[4].setPosition(1122.5f, 645.0f);

        
        m_ciHelpText.setFont(*glDataBox->GetFont("averia.ttf"));
        m_ciHelpText.setColor(sf::Color(255, 255, 255, 255));
        m_ciHelpText.setPosition(960.0f, 752.0f);
        m_ciHelpText.setCharacterSize(42);
        
        m_aHelpString[0] = TmpIni->GetString("0helpstring", "keyassign");
        m_aHelpString[1] = TmpIni->GetString("1helpstring", "keyassign");
        m_aHelpString[2] = TmpIni->GetString("2helpstring", "keyassign");
        m_aHelpString[3] = TmpIni->GetString("3helpstring", "keyassign");
        m_aHelpString[4] = TmpIni->GetString("4helpstring", "keyassign");

        m_ciAudioValue.setFont(*glDataBox->GetFont("averia.ttf"));
        m_ciAudioValue.setColor(sf::Color(0, 0, 0, 255));
        m_ciAudioValue.setOrigin(0.0f, (float)m_ciAudioValue.getCharacterSize() / 2.0f);
        m_ciAudioValue.setPosition(TmpIni->GetFloat("x", "sound") +20.0f + m_ciAudio.GetSize().x, TmpIni->GetFloat("y", "sound"));

        m_ciMusicValue.setFont(*glDataBox->GetFont("averia.ttf"));
        m_ciMusicValue.setColor(sf::Color(0, 0, 0, 255));
        m_ciMusicValue.setOrigin(0.0f, (float)m_ciMusicValue.getCharacterSize() / 2.0f);
        m_ciMusicValue.setPosition(TmpIni->GetFloat("x", "music") +20.0f + m_ciMusic.GetSize().x, TmpIni->GetFloat("y", "music"));

        m_ciGraphicsValue.setFont(*glDataBox->GetFont("averia.ttf"));
        m_ciGraphicsValue.setColor(sf::Color(0, 0, 0, 255));
        m_ciGraphicsValue.setOrigin(0.0f, (float)m_ciGraphicsValue.getCharacterSize() / 2.0f);
        m_ciGraphicsValue.setPosition(TmpIni->GetFloat("x", "graphics") +20.0f + m_ciGraphics.GetSize().x, TmpIni->GetFloat("y", "graphics"));


        m_ciFullscreenIdle.setTexture(*glDataBox->GetTexture(TmpIni->GetString("file", "fullscreen")));
        m_ciFullscreenHover.setTexture(*m_ciFullscreenIdle.getTexture());
        m_ciFullscreenIdle.setTextureRect(sf::IntRect(0, 0, (int)m_ciFullscreenIdle.getTexture()->getSize().x, (int)m_ciFullscreenIdle.getTexture()->getSize().x));
        m_ciFullscreenHover.setTextureRect(sf::IntRect(0, 0, (int)m_ciFullscreenHover.getTexture()->getSize().x, (int)m_ciFullscreenHover.getTexture()->getSize().x));
        m_ciFullscreenIdle.setColor(sf::Color(255, 255, 255, TmpIni->GetInt("opacity", "fullscreen")));
        m_ciFullscreenHover.setColor(sf::Color(255, 255, 255, TmpIni->GetInt("over_opacity", "fullscreen")));
        m_ciFullscreenIdle.setOrigin(m_ciFullscreenIdle.getTextureRect().width / 2.0f, m_ciFullscreenIdle.getTextureRect().height / 2.0f);
        m_ciFullscreenHover.setOrigin(m_ciFullscreenHover.getTextureRect().width / 2.0f, m_ciFullscreenHover.getTextureRect().height / 2.0f);
        m_ciFullscreenIdle.setPosition(m_ciFullscreenIdle.getTextureRect().width / 2.0f, m_ciFullscreenIdle.getTextureRect().height / 2.0f);
        m_ciFullscreenHover.setPosition(m_ciFullscreenHover.getTextureRect().width / 2.0f, m_ciFullscreenHover.getTextureRect().height / 2.0f);
        m_ciFullscreenIdle.setScale(TmpIni->GetFloat("scale", "fullscreen"), TmpIni->GetFloat("scale", "fullscreen"));
        m_ciFullscreenHover.setScale(TmpIni->GetFloat("scale", "fullscreen"), TmpIni->GetFloat("scale", "fullscreen"));
    
        m_ciFullscreenCollision.SetSize((float)m_ciFullscreenIdle.getTextureRect().width, (float)m_ciFullscreenIdle.getTextureRect().height);

        m_ciFullscreen.StatusDrawable()->setTexture(*glDataBox->GetTexture(TmpIni->GetString("file", "fullscreen")));
        m_ciFullscreen.StatusDrawable()->setTextureRect(sf::IntRect(0, (int)m_ciFullscreenIdle.getTexture()->getSize().x, (int)m_ciFullscreenIdle.getTexture()->getSize().x, (int)m_ciFullscreenIdle.getTexture()->getSize().x));
        m_ciFullscreen.StatusDrawable()->setOrigin(m_ciFullscreenIdle.getTextureRect().width / 2.0f, m_ciFullscreenIdle.getTextureRect().height / 2.0f);
        m_ciFullscreen.StatusDrawable()->setPosition(m_ciFullscreenIdle.getTextureRect().width / 2.0f, m_ciFullscreenIdle.getTextureRect().height / 2.0f);
        m_ciFullscreen.StatusDrawable()->setScale(TmpIni->GetFloat("scale", "fullscreen"), TmpIni->GetFloat("scale", "fullscreen"));
        m_ciFullscreen.Button()->AddCollisionObject(0, &m_ciFullscreenCollision);
        m_ciFullscreen.Button()->AddDrawableObject(0, &m_ciFullscreenIdle);
        m_ciFullscreen.Button()->AddDrawableObject(1, &m_ciFullscreenHover);
        m_ciFullscreen.Button()->AddDrawableObject(2, &m_ciFullscreenHover);
        m_ciFullscreen.Button()->BindDrawableToState(0, pwGUI::pwButton::Idle);
        m_ciFullscreen.Button()->BindDrawableToState(1, pwGUI::pwButton::Hover);
        m_ciFullscreen.Button()->BindDrawableToState(2, pwGUI::pwButton::Down);
        m_ciFullscreen.Button()->setPosition(TmpIni->GetFloat("x", "fullscreen"), TmpIni->GetFloat("y", "fullscreen")-10.0f);
        m_ciFullscreen.setPosition(TmpIni->GetFloat("x", "fullscreen"), TmpIni->GetFloat("y", "fullscreen")-10.0f);

        
        m_ciControlsIdle.setTexture(*glDataBox->GetTexture(TmpIni->GetString("file", "controls")));
        m_ciControlsHover.setTexture(*m_ciControlsIdle.getTexture());
        m_ciControlsIdle.setColor(sf::Color(255, 255, 255, TmpIni->GetInt("opacity", "controls")));
        m_ciControlsHover.setColor(sf::Color(255, 255, 255, TmpIni->GetInt("over_opacity", "controls")));
        m_ciControlsIdle.setOrigin(m_ciControlsIdle.getTextureRect().width / 2.0f, m_ciControlsIdle.getTextureRect().height / 2.0f);
        m_ciControlsHover.setOrigin(m_ciControlsHover.getTextureRect().width / 2.0f, m_ciControlsHover.getTextureRect().height / 2.0f);
        m_ciControlsIdle.setPosition(m_ciControlsIdle.getTextureRect().width / 2.0f, m_ciControlsIdle.getTextureRect().height / 2.0f);
        m_ciControlsHover.setPosition(m_ciControlsHover.getTextureRect().width / 2.0f, m_ciControlsHover.getTextureRect().height / 2.0f);
        m_ciControlsIdle.setScale(TmpIni->GetFloat("scale", "controls"), TmpIni->GetFloat("scale", "controls"));
        m_ciControlsHover.setScale(TmpIni->GetFloat("scale", "controls"), TmpIni->GetFloat("scale", "controls"));
    
        m_ciControlsCollision.SetSize((float)m_ciControlsIdle.getTexture()->getSize().x, (float)m_ciControlsIdle.getTexture()->getSize().y);

        m_ciControlsText.setFont(*glDataBox->GetFont("averia.ttf"));
        m_ciControlsText.setString(TmpIni->GetString("title", "controls"));
        m_ciControlsText.setOrigin(m_ciControlsText.getGlobalBounds().width / 2.0f, (float)m_ciControlsText.getCharacterSize() / 2.0f);
        m_ciControlsText.setPosition(m_ciControlsCollision.GetSize().x / 2.0f, m_ciControlsCollision.GetSize().y / 2.0f - (float)m_ciControlsText.getCharacterSize() / 4.0f);

        m_ciControls.AddCollisionObject(0, &m_ciControlsCollision);
        m_ciControls.AddDrawableObject(0, &m_ciControlsIdle);
        m_ciControls.AddDrawableObject(1, &m_ciControlsHover);
        m_ciControls.AddDrawableObject(2, &m_ciControlsHover);
        m_ciControls.AddDrawableObject(3, &m_ciControlsText);
        m_ciControls.BindDrawableToState(0, pwGUI::pwButton::Idle);
        m_ciControls.BindDrawableToState(1, pwGUI::pwButton::Hover);
        m_ciControls.BindDrawableToState(2, pwGUI::pwButton::Down);
        m_ciControls.setPosition(TmpIni->GetFloat("x", "controls"), TmpIni->GetFloat("y", "controls"));


        m_ciCancelIdle.setTexture(*glDataBox->GetTexture(TmpIni->GetString("file", "cancel")));
        m_ciCancelHover.setTexture(*m_ciCancelIdle.getTexture());
        m_ciCancelIdle.setColor(sf::Color(255, 255, 255, TmpIni->GetInt("opacity", "cancel")));
        m_ciCancelHover.setColor(sf::Color(255, 255, 255, TmpIni->GetInt("over_opacity", "cancel")));
        m_ciCancelIdle.setOrigin(m_ciCancelIdle.getTextureRect().width / 2.0f, m_ciCancelIdle.getTextureRect().height / 2.0f);
        m_ciCancelHover.setOrigin(m_ciCancelHover.getTextureRect().width / 2.0f, m_ciCancelHover.getTextureRect().height / 2.0f);
        m_ciCancelIdle.setPosition(m_ciCancelIdle.getTextureRect().width / 2.0f, m_ciCancelIdle.getTextureRect().height / 2.0f);
        m_ciCancelHover.setPosition(m_ciCancelHover.getTextureRect().width / 2.0f, m_ciCancelHover.getTextureRect().height / 2.0f);
        m_ciCancelIdle.setScale(TmpIni->GetFloat("scale", "cancel"), TmpIni->GetFloat("scale", "cancel"));
        m_ciCancelHover.setScale(TmpIni->GetFloat("scale", "cancel"), TmpIni->GetFloat("scale", "cancel"));
    
        m_ciCancelCollision.SetSize((float)m_ciCancelIdle.getTexture()->getSize().x, (float)m_ciCancelIdle.getTexture()->getSize().y);

        m_ciCancelText.setFont(*glDataBox->GetFont("averia.ttf"));
        m_ciCancelText.setString(TmpIni->GetString("title", "cancel"));
        m_ciCancelText.setOrigin(m_ciCancelText.getGlobalBounds().width / 2.0f, (float)m_ciCancelText.getCharacterSize() / 2.0f);
        m_ciCancelText.setPosition(m_ciCancelCollision.GetSize().x / 2.0f, m_ciCancelCollision.GetSize().y / 2.0f - (float)m_ciCancelText.getCharacterSize() / 4.0f);

        m_ciCancelButton.AddCollisionObject(0, &m_ciCancelCollision);
        m_ciCancelButton.AddDrawableObject(0, &m_ciCancelIdle);
        m_ciCancelButton.AddDrawableObject(1, &m_ciCancelHover);
        m_ciCancelButton.AddDrawableObject(2, &m_ciCancelHover);
        m_ciCancelButton.AddDrawableObject(3, &m_ciCancelText);
        m_ciCancelButton.BindDrawableToState(0, pwGUI::pwButton::Idle);
        m_ciCancelButton.BindDrawableToState(1, pwGUI::pwButton::Hover);
        m_ciCancelButton.BindDrawableToState(2, pwGUI::pwButton::Down);
        m_ciCancelButton.setPosition(TmpIni->GetFloat("x", "cancel"), TmpIni->GetFloat("y", "cancel"));


        m_ciOkIdle.setTexture(*glDataBox->GetTexture(TmpIni->GetString("file", "ok")));
        m_ciOkHover.setTexture(*m_ciOkIdle.getTexture());
        m_ciOkIdle.setColor(sf::Color(255, 255, 255, TmpIni->GetInt("opacity", "ok")));
        m_ciOkHover.setColor(sf::Color(255, 255, 255, TmpIni->GetInt("over_opacity", "ok")));
        m_ciOkIdle.setOrigin(m_ciOkIdle.getTextureRect().width / 2.0f, m_ciOkIdle.getTextureRect().height / 2.0f);
        m_ciOkHover.setOrigin(m_ciOkHover.getTextureRect().width / 2.0f, m_ciOkHover.getTextureRect().height / 2.0f);
        m_ciOkIdle.setPosition(m_ciOkIdle.getTextureRect().width / 2.0f, m_ciOkIdle.getTextureRect().height / 2.0f);
        m_ciOkHover.setPosition(m_ciOkHover.getTextureRect().width / 2.0f, m_ciOkHover.getTextureRect().height / 2.0f);
        m_ciOkIdle.setScale(TmpIni->GetFloat("scale", "ok"), TmpIni->GetFloat("scale", "ok"));
        m_ciOkHover.setScale(TmpIni->GetFloat("scale", "ok"), TmpIni->GetFloat("scale", "ok"));
    
        m_ciOkCollision.SetSize((float)m_ciCancelIdle.getTexture()->getSize().x, (float)m_ciCancelIdle.getTexture()->getSize().y);

        m_ciOkText.setFont(*glDataBox->GetFont("averia.ttf"));
        m_ciOkText.setString(TmpIni->GetString("title", "ok"));
        m_ciOkText.setOrigin(m_ciOkText.getGlobalBounds().width / 2.0f, (float)m_ciOkText.getCharacterSize() / 2.0f);
        m_ciOkText.setPosition(m_ciOkCollision.GetSize().x / 2.0f, m_ciOkCollision.GetSize().y / 2.0f - (float)m_ciOkText.getCharacterSize() / 4.0f);

        m_ciOkButton.AddCollisionObject(0, &m_ciOkCollision);
        m_ciOkButton.AddDrawableObject(0, &m_ciOkIdle);
        m_ciOkButton.AddDrawableObject(1, &m_ciOkHover);
        m_ciOkButton.AddDrawableObject(2, &m_ciOkHover);
        m_ciOkButton.AddDrawableObject(3, &m_ciOkText);
        m_ciOkButton.BindDrawableToState(0, pwGUI::pwButton::Idle);
        m_ciOkButton.BindDrawableToState(1, pwGUI::pwButton::Hover);
        m_ciOkButton.BindDrawableToState(2, pwGUI::pwButton::Down);
        m_ciOkButton.setPosition(TmpIni->GetFloat("x", "ok"), TmpIni->GetFloat("y", "ok"));


        glLogics->RegisterGameObject(&m_ciCancelButton, myID);
        glLogics->RegisterGameObject(&m_ciOkButton, myID);
        glLogics->RegisterGameObject(&m_ciMusic, myID);
        glLogics->RegisterGameObject(&m_ciAudio, myID);
        glLogics->RegisterGameObject(&m_ciGraphics, myID);
        glLogics->RegisterGameObject(&m_ciFullscreen, myID);
        glLogics->RegisterGameObject(&m_ciControls, myID);

        Initialized = true;
    }

    m_fOldAudioVolume = CSettings::get()->getSoundVolume();
    m_fOldMusicVolume = CSettings::get()->getMusicVolume();

    for (unsigned int i = 0; i < m_aVideoModes.size(); i++)
    {
        if (pwEngineOptions::GetVideoMode() == m_aVideoModes.at(i))
            m_ciGraphics.SetIndex(m_ciGraphics.GetIndexCount()-(i+1));
    }

    m_ciAudio.SetPercentage(CSettings::get()->getSoundVolume() / 100.0f);
    m_ciMusic.SetPercentage(CSettings::get()->getMusicVolume() / 100.0f);

    TmpSStream << (unsigned int)CSettings::get()->getSoundVolume();
    TmpSStream >> TmpString;
    TmpSStream.clear();
    m_ciAudioValue.setString(TmpString);

    TmpSStream << (unsigned int)CSettings::get()->getMusicVolume();
    TmpSStream >> TmpString;
    TmpSStream.clear();
    m_ciMusicValue.setString(TmpString);

    m_uiVideoWidth = pwEngineOptions::GetVideoMode().width;
    m_uiVideoHeight = pwEngineOptions::GetVideoMode().height;
    TmpSStream << m_uiVideoWidth << "x" << m_uiVideoHeight;
    TmpSStream >> TmpString;
    TmpSStream.clear();
    m_ciGraphicsValue.setString(TmpString);

    m_ciFullscreen.SetStatus((pwEngineOptions::GetWindowStyle() & sf::Style::Fullscreen) == sf::Style::Fullscreen);

    m_aKeys[0] = CSettings::get()->getLayerKey(0);
    m_aKeys[1] = CSettings::get()->getLayerKey(1);
    m_aKeys[2] = CSettings::get()->getLayerKey(2);
    m_aKeys[3] = CSettings::get()->getLayerKey(3);
    m_aKeys[4] = CSettings::get()->getLayerKey(4);
    m_aOldKeys[0] = CSettings::get()->getLayerKey(0);
    m_aOldKeys[1] = CSettings::get()->getLayerKey(1);
    m_aOldKeys[2] = CSettings::get()->getLayerKey(2);
    m_aOldKeys[3] = CSettings::get()->getLayerKey(3);
    m_aOldKeys[4] = CSettings::get()->getLayerKey(4);

    m_uiState = 0;

    return true;
}

unsigned int CMenuOptions::Run()
{
    std::stringstream TmpSStream;
    std::string TmpString;

    TmpSStream << (unsigned int)CSettings::get()->getSoundVolume();
    TmpSStream >> TmpString;
    TmpSStream.clear();
    m_ciAudioValue.setString(TmpString);

    TmpSStream << (unsigned int)CSettings::get()->getMusicVolume();
    TmpSStream >> TmpString;
    TmpSStream.clear();
    m_ciMusicValue.setString(TmpString);

    if (m_ciGraphics.getChanged())
    {
        m_uiVideoWidth = m_aVideoModes.at(m_ciGraphics.GetIndexCount() - (m_ciGraphics.GetIndex()+1)).width;
        m_uiVideoHeight = m_aVideoModes.at(m_ciGraphics.GetIndexCount() - (m_ciGraphics.GetIndex()+1)).height;

        TmpSStream << m_uiVideoWidth << "x" << m_uiVideoHeight;
        TmpSStream >> TmpString;
        TmpSStream.clear();
        m_ciGraphicsValue.setString(TmpString);
    }

    glGraphics->Draw(&m_ciBackground);
    glGraphics->Draw(&m_ciTitle);
    
    glGraphics->Draw(&m_ciMusic);
    glGraphics->Draw(&m_ciAudio);
    glGraphics->Draw(&m_ciGraphics);
    glGraphics->Draw(&m_ciFullscreen);
    glGraphics->Draw(&m_ciControls);

    glGraphics->Draw(&m_ciAudioText);
    glGraphics->Draw(&m_ciMusicText);
    glGraphics->Draw(&m_ciGraphicsText);
    glGraphics->Draw(&m_ciFullscreenText);
    glGraphics->Draw(&m_ciControlsLabel);
    glGraphics->Draw(&m_ciAudioValue);
    glGraphics->Draw(&m_ciMusicValue);
    glGraphics->Draw(&m_ciGraphicsValue);

    glGraphics->Draw(&m_ciCancelButton);
    glGraphics->Draw(&m_ciOkButton);

    CSettings::get()->setSoundVolume((float)(unsigned int)(m_ciAudio.GetPercentage()*100.0f));
    CSettings::get()->setMusicVolume((float)(unsigned int)(m_ciMusic.GetPercentage()*100.0f));
    

    if (m_ciCancelButton.GetClicked())
    {
        CSettings::get()->setSoundVolume(m_fOldAudioVolume);
        CSettings::get()->setMusicVolume(m_fOldMusicVolume);

        return ID_GS_MENU_MAIN;
    }

    if (m_ciOkButton.GetClicked())
    {
        if (m_ciFullscreen.GetStatus())
            pwEngineOptions::SetWindowStyle(pwEngineOptions::GetWindowStyle() | sf::Style::Fullscreen);
        else
            pwEngineOptions::SetWindowStyle(pwEngineOptions::GetWindowStyle() & (sf::Style::Close | sf::Style::Resize | sf::Style::Titlebar));

        pwEngineOptions::SetVideoMode(m_aVideoModes.at(m_ciGraphics.GetIndexCount() - (m_ciGraphics.GetIndex()+1)));

        CSettings::get()->setLayerKey(0, m_aKeys[0]);
        CSettings::get()->setLayerKey(1, m_aKeys[1]);
        CSettings::get()->setLayerKey(2, m_aKeys[2]);
        CSettings::get()->setLayerKey(3, m_aKeys[3]);
        CSettings::get()->setLayerKey(4, m_aKeys[4]);

        return ID_GS_MENU_MAIN;
    }

    
    if (m_uiState > 0 && m_uiState < 6)
    {
        sf::Event e;
        while(1)
        {
            if (glGraphics->GetRenderWindow().waitEvent(e))
            {
                if (e.type == sf::Event::KeyPressed)
                {
                    if (e.key.code == sf::Keyboard::Escape)
                    {
                        m_aKeys[0] = m_aOldKeys[0];
                        m_aKeys[1] = m_aOldKeys[1];
                        m_aKeys[2] = m_aOldKeys[2];
                        m_aKeys[3] = m_aOldKeys[3];
                        m_aKeys[4] = m_aOldKeys[4];
                        m_uiState = 6;
                    }
                    else if (m_uiState == 1)
                    {
                        m_aKeys[0] = e.key.code;

                        m_ciHelpText.setString(m_aHelpString[1]);
                        m_ciHelpText.setOrigin(m_ciHelpText.getGlobalBounds().width / 2.0f, m_ciHelpText.getOrigin().y);
                        m_aKeyNames[0].setColor(sf::Color(255, 255, 255, 255));
                        m_aKeyNames[1].setColor(sf::Color(0, 255, 0, 255));

                        m_uiState++;
                    }
                    else if (m_uiState == 2)
                    {
                        if (m_aKeys[0] != e.key.code)
                        {
                            m_aKeys[1] = e.key.code;

                            m_ciHelpText.setString(m_aHelpString[2]);
                            m_ciHelpText.setOrigin(m_ciHelpText.getGlobalBounds().width / 2.0f, m_ciHelpText.getOrigin().y);
                            m_aKeyNames[1].setColor(sf::Color(255, 255, 255, 255));
                            m_aKeyNames[2].setColor(sf::Color(0, 255, 0, 255));

                            m_uiState++;
                        }
                    }
                    else if (m_uiState == 3)
                    {
                        if (m_aKeys[0] != e.key.code &&
                            m_aKeys[1] != e.key.code)
                        {
                            m_aKeys[2] = e.key.code;

                            m_ciHelpText.setString(m_aHelpString[3]);
                            m_ciHelpText.setOrigin(m_ciHelpText.getGlobalBounds().width / 2.0f, m_ciHelpText.getOrigin().y);
                            m_aKeyNames[2].setColor(sf::Color(255, 255, 255, 255));
                            m_aKeyNames[3].setColor(sf::Color(0, 255, 0, 255));

                            m_uiState++;
                        }
                    }
                    else if (m_uiState == 4)
                    {
                        if (m_aKeys[0] != e.key.code &&
                            m_aKeys[1] != e.key.code &&
                            m_aKeys[2] != e.key.code)
                        {
                            m_aKeys[3] = e.key.code;

                            m_ciHelpText.setString(m_aHelpString[4]);
                            m_ciHelpText.setOrigin(m_ciHelpText.getGlobalBounds().width / 2.0f, m_ciHelpText.getOrigin().y);
                            m_aKeyNames[3].setColor(sf::Color(255, 255, 255, 255));
                            m_aKeyNames[4].setColor(sf::Color(0, 255, 0, 255));

                            m_uiState++;
                        }
                    }
                    else if (m_uiState == 5)
                    {
                        if (m_aKeys[0] != e.key.code &&
                            m_aKeys[1] != e.key.code &&
                            m_aKeys[2] != e.key.code &&
                            m_aKeys[3] != e.key.code)
                        {
                            m_aKeys[4] = e.key.code;

                            m_aKeyNames[4].setColor(sf::Color(255, 255, 255, 255));

                            m_uiState++;
                        }
                    }
                    break;
                }
            }
        }
        glGraphics->Draw(&m_ciKeyAssignBGShape);
        glGraphics->Draw(&m_ciKeyAssignBackground);
        glGraphics->Draw(&m_aLayerNames[0]);
        glGraphics->Draw(&m_aLayerNames[1]);
        glGraphics->Draw(&m_aLayerNames[2]);
        glGraphics->Draw(&m_aLayerNames[3]);
        glGraphics->Draw(&m_aLayerNames[4]);
        glGraphics->Draw(&m_aKeyNames[0]);
        glGraphics->Draw(&m_aKeyNames[1]);
        glGraphics->Draw(&m_aKeyNames[2]);
        glGraphics->Draw(&m_aKeyNames[3]);
        glGraphics->Draw(&m_aKeyNames[4]);
        glGraphics->Draw(&m_ciHelpText);
    }
    else if (m_uiState == 6)
    {
        m_aOldKeys[0] = m_aKeys[0];
        m_aOldKeys[1] = m_aKeys[1];
        m_aOldKeys[2] = m_aKeys[2];
        m_aOldKeys[3] = m_aKeys[3];
        m_aOldKeys[4] = m_aKeys[4];

        m_aKeyNames[0].setColor(sf::Color(255, 255, 255, 255));
        m_aKeyNames[1].setColor(sf::Color(255, 255, 255, 255));
        m_aKeyNames[2].setColor(sf::Color(255, 255, 255, 255));
        m_aKeyNames[3].setColor(sf::Color(255, 255, 255, 255));
        m_aKeyNames[4].setColor(sf::Color(255, 255, 255, 255));

        m_ciCancelButton.setActive(true);
        m_ciOkButton.setActive(true);
        m_ciControls.setActive(true);
        m_ciAudio.setActive(true);
        m_ciMusic.setActive(true);
        m_ciGraphics.setActive(true);
        m_ciFullscreen.setActive(true);
        m_uiState = 0;
    }

    if (m_ciControls.GetClicked())
    {
        m_uiState = 1;
        m_ciCancelButton.setActive(false);
        m_ciOkButton.setActive(false);
        m_ciControls.setActive(false);
        m_ciAudio.setActive(false);
        m_ciMusic.setActive(false);
        m_ciGraphics.setActive(false);
        m_ciFullscreen.setActive(false);
        m_ciHelpText.setString(m_aHelpString[0]);
        m_ciHelpText.setOrigin(m_ciHelpText.getGlobalBounds().width / 2.0f, m_ciHelpText.getOrigin().y);

        m_aKeyNames[0].setColor(sf::Color(0, 255, 0, 255));

        glGraphics->Draw(&m_ciKeyAssignBGShape);
        glGraphics->Draw(&m_ciKeyAssignBackground);
        glGraphics->Draw(&m_aLayerNames[0]);
        glGraphics->Draw(&m_aLayerNames[1]);
        glGraphics->Draw(&m_aLayerNames[2]);
        glGraphics->Draw(&m_aLayerNames[3]);
        glGraphics->Draw(&m_aLayerNames[4]);
        glGraphics->Draw(&m_aKeyNames[0]);
        glGraphics->Draw(&m_aKeyNames[1]);
        glGraphics->Draw(&m_aKeyNames[2]);
        glGraphics->Draw(&m_aKeyNames[3]);
        glGraphics->Draw(&m_aKeyNames[4]);
        glGraphics->Draw(&m_ciHelpText);
    }

    m_aKeyNames[0].setString(pwHelper::key::getKeyName(m_aKeys[0]));
    m_aKeyNames[1].setString(pwHelper::key::getKeyName(m_aKeys[1]));
    m_aKeyNames[2].setString(pwHelper::key::getKeyName(m_aKeys[2]));
    m_aKeyNames[3].setString(pwHelper::key::getKeyName(m_aKeys[3]));
    m_aKeyNames[4].setString(pwHelper::key::getKeyName(m_aKeys[4]));
    m_aKeyNames[0].setOrigin(m_aKeyNames[0].getGlobalBounds().width / 2.0f, m_aKeyNames[0].getOrigin().y);
    m_aKeyNames[1].setOrigin(m_aKeyNames[1].getGlobalBounds().width / 2.0f, m_aKeyNames[1].getOrigin().y);
    m_aKeyNames[2].setOrigin(m_aKeyNames[2].getGlobalBounds().width / 2.0f, m_aKeyNames[2].getOrigin().y);
    m_aKeyNames[3].setOrigin(m_aKeyNames[3].getGlobalBounds().width / 2.0f, m_aKeyNames[3].getOrigin().y);
    m_aKeyNames[4].setOrigin(m_aKeyNames[4].getGlobalBounds().width / 2.0f, m_aKeyNames[4].getOrigin().y);

    return myID;
}

bool CMenuOptions::Close()
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

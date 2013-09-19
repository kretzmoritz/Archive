#include "..\..\include\Game\CHighscore.h"
#include "..\..\include\Game\CSettings.h"

CHighscore::CHighscore() : pwGameState(ID_GS_HIGHSCORE)
{
    Initialized = false;
}

CHighscore::~CHighscore()
{
}

bool CHighscore::Start() 
{
    if (!Initialized)
    {
        pwIniFile* TmpIni = glDataBox->GetIniFile("menu_highscore.ini");

        m_ciBackground.setTexture(*glDataBox->GetTexture(TmpIni->GetString("file", "background")));
    
        m_ciTitle.setTexture(*glDataBox->GetTexture(TmpIni->GetString("file", "title")));
        m_ciTitle.setOrigin((float)m_ciTitle.getTexture()->getSize().x / 2.0f, (float)m_ciTitle.getTexture()->getSize().y / 2.0f);
        m_ciTitle.setPosition(TmpIni->GetFloat("x", "title"), TmpIni->GetFloat("y", "title"));
        m_ciTitle.setScale(TmpIni->GetFloat("scale", "title"), TmpIni->GetFloat("scale", "title"));
        m_ciTitle.setColor(sf::Color(255, 255, 255, TmpIni->GetInt("opacity", "title")));

        m_ciScroll.setTexture(*glDataBox->GetTexture("scroll.png"));
        m_ciScroll.setTextureRect(sf::IntRect(0, 0, 20, 300));
        m_ciScrollIdle.setTexture(*glDataBox->GetTexture("scroll.png"));
        m_ciScrollIdle.setTextureRect(sf::IntRect(20, 0, 20, 5));
        m_ciScrollIdle.setOrigin(m_ciScrollIdle.getTextureRect().width / 2.0f, m_ciScrollIdle.getTextureRect().height / 2.0f);
        m_ciScrollHover.setTexture(*glDataBox->GetTexture("scroll.png"));
        m_ciScrollHover.setTextureRect(sf::IntRect(20, 5, 40, 5));
        m_ciScrollHover.setOrigin(m_ciScrollHover.getTextureRect().width / 2.0f, m_ciScrollHover.getTextureRect().height / 2.0f);
    
        m_ciScrollCollision.SetSize((float)m_ciScrollIdle.getTextureRect().width, (float)m_ciBacklIdle.getTextureRect().height);

        m_ciScrollSlider.AddCollisionObject(0, &m_ciScrollCollision);
        m_ciScrollSlider.AddDrawableObject(0, &m_ciScrollIdle);
        m_ciScrollSlider.AddDrawableObject(1, &m_ciScrollHover);
        m_ciScrollSlider.AddDrawableObject(2, &m_ciScrollHover);
        m_ciScrollSlider.BindDrawableToState(0, pwGUI::pwButton::Idle);
        m_ciScrollSlider.BindDrawableToState(1, pwGUI::pwButton::Hover);
        m_ciScrollSlider.BindDrawableToState(2, pwGUI::pwButton::Down);

        m_ciScrollBar.SetSprite(&m_ciScroll);
        m_ciScrollBar.SetSlider(&m_ciScrollSlider);
        m_ciScrollBar.SetHorizontal(false);
        m_ciScrollBar.setPosition(1260.0f, 415.0f);
        m_ciScrollBar.SetSize(sf::Vector2i(20, 300));

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
        glLogics->RegisterGameObject(&m_ciScrollBar, myID);

        Initialized = true;
    }

    pwIniFile TmpIni;
    TmpIni.LoadFromFile(CSettings::get()->getHighscoreFileName());

    std::stringstream typecast;
    std::string string;
    for (int i = 0; i < TmpIni.GetInt("count"); i++)
    {
        typecast << i;
        typecast >> string;
        typecast.clear();

        m_HighscoreEntryList.push_back(pwHelper::split_data<unsigned int, pwHelper::split_data<std::string, std::string>>(i, pwHelper::split_data<std::string, std::string>(TmpIni.GetString(string + "name"), TmpIni.GetString(string + "score"))));
    }

    if (m_HighscoreEntryList.size() > 10)
    {
        m_ciScrollBar.setActive();
        m_ciScrollBar.setVisible();
        m_ciScrollBar.SetTotalLines(m_HighscoreEntryList.size()-10);
    }
    else
    {
        m_ciScrollBar.setActive(false);
        m_ciScrollBar.setVisible(false);
    }

    for (int i = 0; i < 10; i++)
    {
        m_ciHighscoreEntries[i].first.setFont(*glDataBox->GetFont("averia.ttf"));
        m_ciHighscoreEntries[i].second.setFont(*glDataBox->GetFont("averia.ttf"));
        m_ciHighscoreEntries[i].first.setPosition((float)pwEngineOptions::GetInternGraphicsSize().x / 2.0f+4.0f, 340.0f + i*50.0f);
        m_ciHighscoreEntries[i].second.setPosition((float)pwEngineOptions::GetInternGraphicsSize().x / 2.0f+13.0f, 340.0f + i*50.0f);
        m_ciHighscoreEntries[i].first.setColor(sf::Color());
        m_ciHighscoreEntries[i].second.setColor(sf::Color());
    }
    
    return true;
}

unsigned int CHighscore::Run()
{
    glGraphics->Draw(&m_ciBackground);
    glGraphics->Draw(&m_ciScrollBar);
    glGraphics->Draw(&m_ciTitle);

    for (unsigned int i = 0; i < 10; i++)
    {
        m_ciHighscoreEntries[i].first.setString("");
        m_ciHighscoreEntries[i].second.setString("");

        for (std::list<pwHelper::split_data<unsigned int, pwHelper::split_data<std::string, std::string>>>::iterator j = m_HighscoreEntryList.begin(); j != m_HighscoreEntryList.end(); j++)
        {
            if (j->first == m_ciScrollBar.GetCurrentLine() + i)
            {
                m_ciHighscoreEntries[i].first.setString(j->second.first + " :");
                m_ciHighscoreEntries[i].second.setString(j->second.second);
                m_ciHighscoreEntries[i].first.setOrigin(m_ciHighscoreEntries[i].first.getGlobalBounds().width, (float)m_ciHighscoreEntries[i].first.getCharacterSize() / 2.0f);
                m_ciHighscoreEntries[i].second.setOrigin(0.0f, (float)m_ciHighscoreEntries[i].second.getCharacterSize() / 2.0f);
            }
        }

        glGraphics->Draw(&m_ciHighscoreEntries[i].first);
        glGraphics->Draw(&m_ciHighscoreEntries[i].second);
    }

    glGraphics->Draw(&m_ciBackButton);

    if (m_ciBackButton.GetClicked())
    {
        return ID_GS_MENU_MAIN;
    }

    return myID;
}
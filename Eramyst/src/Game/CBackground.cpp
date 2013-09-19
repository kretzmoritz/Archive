#include "..\..\include\Game\CBackground.h"

#include <iostream>

void CBackground::init()
{
    shutdown();

    std::stringstream typecast;
    std::string sLayerNum;
    std::string sElementNum;

    pwIniFile* TmpIni = glDataBox->GetIniFile("background_" + CLevelData::getCurrentLevelString() + ".ini");

    if (TmpIni == NULL)
        return;

    unsigned int uiLayerCount = TmpIni->GetUInt("layers");
    m_vLayer.resize(uiLayerCount);

    for (unsigned int i = 0; i < uiLayerCount; i++)
    {
        typecast << i;
        typecast >> sLayerNum; 
        typecast.clear();

        sLayerNum = "layer" + sLayerNum;

        m_vLayer[i].layer = TmpIni->GetInt("layer", sLayerNum);
        m_vLayer[i].speed = TmpIni->GetFloat("speed", sLayerNum);
        m_vLayer[i].rate = TmpIni->GetFloat("rate", sLayerNum);
        m_vLayer[i].variance = TmpIni->GetFloat("variance", sLayerNum);
        m_vLayer[i].nextspawn = m_vLayer[i].rate + pwHelper::math::random::getFloatEx(-m_vLayer[i].variance, m_vLayer[i].variance);
        m_vLayer[i].y = TmpIni->GetInt("y", sLayerNum);
        m_vLayer[i].yVariance = TmpIni->GetInt("yvariance", sLayerNum);
        m_vLayer[i].start = TmpIni->GetBool("start", sLayerNum);
        m_vLayer[i].animated = TmpIni->GetBool("animated", sLayerNum);
        m_vLayer[i].max_chance = 0;
        m_vLayer[i].timer = 0.0f;
        
        unsigned int uiElementCount = TmpIni->GetUInt("elements", sLayerNum);
        m_vLayer[i].elements.resize(uiLayerCount);

        for (unsigned int j = 0; j < uiElementCount; j++)
        {
            typecast << j;
            typecast >> sElementNum;
            typecast.clear();

            m_vLayer[i].elements[j].chance = TmpIni->GetUInt(sElementNum + "chance", sLayerNum);
            m_vLayer[i].max_chance += m_vLayer[i].elements[j].chance;

            if (j > 0)
                m_vLayer[i].elements[j].chance += m_vLayer[i].elements[j-1].chance;

            m_vLayer[i].elements[j].texture = glDataBox->GetTexture(TmpIni->GetString(sElementNum + "file", sLayerNum));
        }
    }

    update_this();

    for (std::vector<Layer>::iterator i = m_vLayer.begin(); i != m_vLayer.end(); i++)
    {
        if (i->start == true)
        {
            for (std::list<sf::Sprite*>::iterator j = i->objects.begin(); j != i->objects.end(); j++)
            {
                (*j)->setPosition((*j)->getPosition().x - (float)pwEngineOptions::GetInternGraphicsSize().x, (*j)->getPosition().y);
            }
        }
    }
}

void CBackground::draw()
{
    for (std::vector<Layer>::iterator i = m_vLayer.begin(); i != m_vLayer.end(); i++)
    {
        for (std::list<sf::Sprite*>::iterator j = i->objects.begin(); j != i->objects.end(); j++)
        {
            glGraphics->Draw(*j, i->layer);
        }
    }
}

void CBackground::update_this()
{
    for (std::vector<Layer>::iterator i = m_vLayer.begin(); i != m_vLayer.end(); i++)
    {
        i->timer += glGraphics->GetLastFrameTime();

        if (i->timer  >= i->nextspawn && (i->objects.empty() || (i->objects.back()->getPosition().x + i->objects.back()->getTexture()->getSize().x - 5 < pwEngineOptions::GetInternGraphicsSize().x)))
        {
            // spawn new object
            sf::Sprite* ciSprite = NULL;
            sf::Texture* ciTexture = NULL;

            // what object is to be spawned
            unsigned int iRandom = (unsigned int)pwHelper::math::random::getIntEx(0, (int)i->max_chance);

            bool bTextureFound = false;

            for (std::vector<Element>::iterator j = i->elements.begin(); j != i->elements.end(); j++)
            {
                if (iRandom <= j->chance)
                {
                    if (j->texture != NULL)
                    {
                        ciTexture = j->texture;
                        bTextureFound = true;
                    }
                    break;
                }
            }

            if (bTextureFound)
            {
                ciSprite = new sf::Sprite(*ciTexture);
                ciSprite->setPosition((float)pwEngineOptions::GetInternGraphicsSize().x, (float)i->y - (float)ciSprite->getTexture()->getSize().y + (float)pwHelper::math::random::getIntEx(-i->yVariance, i->yVariance));
                i->objects.push_back(ciSprite);
            }
            i->timer -= i->nextspawn;
            i->nextspawn = i->rate + pwHelper::math::random::getFloatEx(-i->variance, i->variance);
        }

        for (std::list<sf::Sprite*>::iterator j = i->objects.begin(); j != i->objects.end(); j++)
        {
            (*j)->setPosition((*j)->getPosition().x - i->speed*glGraphics->GetLastFrameTime(), (*j)->getPosition().y);


            if ((*j)->getPosition().x + (float)(*j)->getTexture()->getSize().x < 0.0f)
            {
                delete (*j);
                j = i->objects.erase(j);
            }
        }
    }
}

void CBackground::update()
{
    update_this();
    draw();
}

void CBackground::shutdown()
{
    for (std::vector<Layer>::iterator i = m_vLayer.begin(); i != m_vLayer.end(); i++)
    {
        for (std::list<sf::Sprite*>::iterator j = i->objects.begin(); j != i->objects.end(); j++)
        {
            delete (*j);
        }

        i->objects.clear();
    }
}
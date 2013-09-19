#include "..\include\pwGUIButton.hpp"

namespace pwGUI
{
    pwButton::pwButton() :

        myClicked(false),
        myHover(false),

        myInstantClickType(false),
        
        myMouseDown(false)

    {
    }
    
    pwButton::~pwButton() 
    {
    }

    void pwButton::OnUpdate(float FrameTime)
    {
        if (!getActive())
            return;

        this->updateCollisionObjects();
        myHover = false;
        for (std::list<std::pair<unsigned int, pwCollisionObject*>>::iterator k = GetCollisionObjectMap()->begin(); k != GetCollisionObjectMap()->end(); k++)
        {
            // Do pwCollisionObjects collide?
            if (k->second->Collides(pwGraphics::get()->GetCursorCollisionObject()))
            {
                myHover = true;
            }
        }

        if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            myMouseDown = true;

            if (myInstantClickType && myHover)
            {
                myClicked = true;
            }
        }
        else if (myMouseDown && myHover)
        {
            myClicked = true;
            myMouseDown = false;
        }
        else
        {
            myMouseDown = false;
        }
    }

    void pwButton::draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        if (getVisible())
        {
            if (myHover)
            {
                if (myMouseDown)
                {
                    for (std::list<std::pair<unsigned int, sf::Drawable*>>::const_iterator i = myDownDrawableList.begin(); i != myDownDrawableList.end(); i++)
                    {
                        target.draw(*(i->second), sf::RenderStates(states.blendMode, states.transform * getTransform(), states.texture, states.shader));
                    }
                }
                else
                {
                    for (std::list<std::pair<unsigned int, sf::Drawable*>>::const_iterator i = myHoverDrawableList.begin(); i != myHoverDrawableList.end(); i++)
                    {
                        target.draw(*(i->second), sf::RenderStates(states.blendMode, states.transform * getTransform(), states.texture, states.shader));
                    }
                }
            }
            else
            {
                for (std::list<std::pair<unsigned int, sf::Drawable*>>::const_iterator i = myIdleDrawableList.begin(); i != myIdleDrawableList.end(); i++)
                {
                    target.draw(*(i->second), sf::RenderStates(states.blendMode, states.transform * getTransform(), states.texture, states.shader));
                }
            }
            target.draw((pwGameObject)*this, states);
        }
    }

    bool pwButton::GetHover() const
    {
        return myHover;
    }

    bool pwButton::GetClicked(bool resetClicked)
    {
        if (resetClicked = true && myClicked)
        {
            myClicked = false;
            return true;
        }
        else
        {
            return myClicked;
        }
    }

    void pwButton::SetInstantClickType(bool Instant)
    {
        myInstantClickType = Instant;
    }
    
    bool pwButton::GetInstantClickType()
    {
        return myInstantClickType;
    }

    void pwButton::BindDrawableToState(unsigned int DrawableID, pwButtonState State)
    {
        for (std::list<std::pair<unsigned int, sf::Drawable*>>::iterator i = myDrawableMap.begin(); i != myDrawableMap.end(); i++)
        {
            if (i->first == DrawableID)
            {
                if (State == Idle)
                {
                    myIdleDrawableList.push_back(std::make_pair(DrawableID, i->second));
                }
                else if (State == Hover)
                {
                    myHoverDrawableList.push_back(std::make_pair(DrawableID, i->second));
                }
                else
                {
                    myDownDrawableList.push_back(std::make_pair(DrawableID, i->second));
                }

                i = myDrawableMap.erase(i);
                if (i == myDrawableMap.end())
                    break;
            }
        }
    }

    void pwButton::UnbindDrawable(unsigned int DrawableID)
    {
        for (std::list<std::pair<unsigned int, sf::Drawable*>>::iterator i = myIdleDrawableList.begin(); i != myIdleDrawableList.end(); i++)
        {
            if (i->first == DrawableID)
            {
                myDrawableMap.push_back(std::make_pair(DrawableID, i->second));
                
                i = myIdleDrawableList.erase(i);
                if (i == myIdleDrawableList.end())
                    break;
            }
        }
        for (std::list<std::pair<unsigned int, sf::Drawable*>>::iterator i = myHoverDrawableList.begin(); i != myHoverDrawableList.end(); i++)
        {
            if (i->first == DrawableID)
            {
                myDrawableMap.push_back(std::make_pair(DrawableID, i->second));
                
                i = myHoverDrawableList.erase(i);
                if (i == myHoverDrawableList.end())
                    break;
            }
        }
        for (std::list<std::pair<unsigned int, sf::Drawable*>>::iterator i = myDownDrawableList.begin(); i != myDownDrawableList.end(); i++)
        {
            if (i->first == DrawableID)
            {
                myDrawableMap.push_back(std::make_pair(DrawableID, i->second));
                
                i = myDownDrawableList.erase(i);
                if (i == myDownDrawableList.end())
                    break;
            }
        }
    }
}
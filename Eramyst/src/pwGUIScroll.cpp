#include "..\include\pwGUIScroll.hpp"

namespace pwGUI
{
    pwScroll::pwScroll() :

        myFocused(false),
        myHorizontal(true),
        myHover(false),
        
        myMouseDown(false),

        mySlider(NULL),
        myDrawable(NULL),

        myTotalLines(0),
        myCurrentLine(0)

    {
    }
    
    pwScroll::~pwScroll() 
    {
    }

    void pwScroll::OnUpdate(float FrameTime)
    {
        if (!getActive())
            return;

        myCollisionRect.SetAbsTransformable(*this);
        myHover = false;
        
        // Do pwCollisionObjects collide?
        if (myCollisionRect.Collides(pwGraphics::get()->GetCursorCollisionObject()))
        {
            myHover = true;
        }

        if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && myHover)
        {
            myMouseDown = true;

            float Percentage = 0.0f;

            if (myHorizontal)
            {
                Percentage = (pwGraphics::get()->GetCursorCollisionObject()->getPosition().x - getPosition().x) / myCollisionRect.GetSize().x;
            }
            else
            {
                Percentage = (pwGraphics::get()->GetCursorCollisionObject()->getPosition().y - getPosition().y) / myCollisionRect.GetSize().y;
            }

            if (Percentage > 1.0f)
                Percentage = 1.0f;
            else if (Percentage < 0.0f)
                Percentage = 0.0f;

            myCurrentLine = (unsigned int)(Percentage * (float)myTotalLines);
        }
        else
        {
            myMouseDown = false;
        }

        if (mySlider != NULL && myTotalLines != 0)
        {
            mySlider->Update(FrameTime);
            mySlider->OnUpdate(FrameTime);
            
            if (myHorizontal)
            {
                mySlider->setPosition(myCollisionRect.GetSize().x * ((float)myCurrentLine / (float)myTotalLines), myCollisionRect.GetSize().y / 2.0f);
            }
            else
            {
                mySlider->setPosition(myCollisionRect.GetSize().x / 2.0f, myCollisionRect.GetSize().y * ((float)myCurrentLine / (float)myTotalLines));
            }
        }
    }

    void pwScroll::draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        if (getVisible() && myTotalLines > 0)
        {
            if (myDrawable != NULL)
               target.draw(*myDrawable, sf::RenderStates(states.blendMode, states.transform * getTransform(), states.texture, states.shader));
            
            if (mySlider != NULL)
                target.draw(*mySlider, sf::RenderStates(states.blendMode, states.transform * getTransform(), states.texture, states.shader));

            target.draw((pwGameObject)*this, states);
        }
    }

    bool pwScroll::GetHover()
    {
        return myHover;
    }

    bool pwScroll::GetFocus()
    {
        return myFocused;
    }

    void pwScroll::SetFocus(bool Focus)
    {
        myFocused = Focus;
    }

    void pwScroll::SetSize(const sf::Vector2i& Size)
    {
        myCollisionRect.SetSize((float)Size.x, (float)Size.y);
    }
}
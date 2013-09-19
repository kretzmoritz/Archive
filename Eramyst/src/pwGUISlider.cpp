#include "..\include\pwGUISlider.hpp"

namespace pwGUI
{
    pwSlider::pwSlider() :

        myFocused(false),
        myHorizontal(true),
        myHover(false),
        
        myMouseDown(false),

        myPercentage(0.0f),

        mySlider(NULL),
        myDrawable(NULL)

    {
    }
    
    pwSlider::~pwSlider() 
    {
    }

    void pwSlider::OnUpdate(float FrameTime)
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

            if (myHorizontal)
            {
                myPercentage = (pwGraphics::get()->GetCursorCollisionObject()->getPosition().x - getPosition().x) / myCollisionRect.GetSize().x;
            }
            else
            {
                myPercentage = ((getPosition().y + myCollisionRect.GetSize().y) - pwGraphics::get()->GetCursorCollisionObject()->getPosition().y) / myCollisionRect.GetSize().y;
            }

            if (myPercentage > 1.0f)
                myPercentage = 1.0f;
            else if (myPercentage < 0.0f)
                myPercentage = 0.0f;
        }
        else
        {
            myMouseDown = false;
        }

        if (mySlider != NULL)
            mySlider->OnUpdate(FrameTime);
    }

    void pwSlider::draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        if (getVisible())
        {
            if (myDrawable != NULL)
            {
                myDrawable->setTextureRect(myIdleRect);
                target.draw(*myDrawable, sf::RenderStates(states.blendMode, states.transform * getTransform(), states.texture, states.shader));

                if (myHorizontal)
                    myDrawable->setTextureRect(sf::IntRect(myStateRect.left, myStateRect.top, (int)((float)myStateRect.width * myPercentage), myStateRect.height));
                else
                    myDrawable->setTextureRect(sf::IntRect(myStateRect.left, myStateRect.top + myStateRect.height - (int)((float)myStateRect.height * myPercentage), myStateRect.width, (int)((float)myStateRect.height * myPercentage)));

                target.draw(*myDrawable, sf::RenderStates(states.blendMode, states.transform * getTransform(), states.texture, states.shader));
            }
            
            if (mySlider != NULL)
                target.draw(*mySlider, sf::RenderStates(states.blendMode, states.transform * getTransform(), states.texture, states.shader));

            target.draw((pwGameObject)*this, states);
        }
    }

    bool pwSlider::GetHover()
    {
        return myHover;
    }

    float pwSlider::GetPercentage()
    {
        return myPercentage;
    }

    bool pwSlider::GetFocus()
    {
        return myFocused;
    }

    void pwSlider::SetFocus(bool Focus)
    {
        myFocused = Focus;
    }

    void pwSlider::SetSize(const sf::Vector2i& Size)
    {
        myIdleRect.left = 0;
        myIdleRect.top = 0;
        myIdleRect.width = Size.x;
        myIdleRect.height = Size.y;
        myStateRect.left = 0;
        myStateRect.top = Size.y;
        myStateRect.width = Size.x;
        myStateRect.height = Size.y;
        
        myCollisionRect.SetSize((float)Size.x, (float)Size.y);
    }
}
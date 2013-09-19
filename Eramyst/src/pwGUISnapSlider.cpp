#include "..\include\pwGUISnapSlider.hpp"

namespace pwGUI
{
    pwSnapSlider::pwSnapSlider() :

        myFocused(false),
        myHorizontal(true),
        myHover(false),
        
        myMouseDown(false),

        myPercentage(0.0f),

        mySlider(NULL),
        myDrawable(NULL),
        
        myIndex(0),
        myIndexCount(2),
        myPerIndexPercentage(1.0f),
        myChanged(false)

    {
    }
    
    pwSnapSlider::~pwSnapSlider() 
    {
    }

    void pwSnapSlider::OnUpdate(float FrameTime)
    {
        if (!getActive())
            return;

        myCollisionRect.SetAbsTransformable(*this);
        myHover = false;
        myChanged = false;
        
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

            unsigned int TmpIndex = myIndex;
            myIndex = ((unsigned int)(myPercentage / (myPerIndexPercentage/2))+1) / 2;

            if (myIndex != TmpIndex)
                myChanged = true;

            myPercentage = myIndex * myPerIndexPercentage;
        }
        else
        {
            myMouseDown = false;
        }

        if (mySlider != NULL)
            mySlider->OnUpdate(FrameTime);
    }

    void pwSnapSlider::draw(sf::RenderTarget& target, sf::RenderStates states) const
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

                
                myDrawable->setTextureRect(mySnapRect);

                sf::Vector2f TmpV2 = myDrawable->getPosition();
                if (myHorizontal)
                {
                    for (unsigned int i = 1; i < myIndexCount -1; i++)
                    {
                        myDrawable->setPosition(TmpV2.x + i*myPerIndexPercentage*GetSize().x - (mySnapRect.width/2.0f), TmpV2.y);
                        target.draw(*myDrawable, sf::RenderStates(states.blendMode, states.transform * getTransform(), states.texture, states.shader));
                    }
                }
                else
                {
                    for (unsigned int i = 1; i < myIndexCount -1; i++)
                    {
                        myDrawable->setPosition(TmpV2.x, TmpV2.y + i*myPerIndexPercentage*GetSize().y - (mySnapRect.height/2.0f));
                        target.draw(*myDrawable, sf::RenderStates(states.blendMode, states.transform * getTransform(), states.texture, states.shader));
                    }
                }
                myDrawable->setPosition(TmpV2);
            }
            
            if (mySlider != NULL)
                target.draw(*mySlider, sf::RenderStates(states.blendMode, states.transform * getTransform(), states.texture, states.shader));

            target.draw((pwGameObject)*this, states);
        }
    }

    bool pwSnapSlider::GetHover() const
    {
        return myHover;
    }

    bool pwSnapSlider::GetFocus() const
    {
        return myFocused;
    }

    void pwSnapSlider::SetFocus(bool Focus)
    {
        myFocused = Focus;
    }

    void pwSnapSlider::SetSize(const sf::Vector2i& Size)
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
    void pwSnapSlider::SetSnapSize(const sf::Vector2i& Size)
    {
        mySnapRect.left = 0;
        mySnapRect.top = myIdleRect.height * 2;
        mySnapRect.width = Size.x;
        mySnapRect.height = Size.y;
    }
}
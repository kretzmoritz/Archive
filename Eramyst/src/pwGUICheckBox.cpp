#include "..\include\pwGUICheckBox.hpp"

namespace pwGUI
{
    pwCheckBox::pwCheckBox() :

        myStatus(false),
        myChanged(false)

    {
        myButton = new pwButton;
    }
    
    pwCheckBox::~pwCheckBox() 
    {
        if (myButton != NULL)
            delete myButton;
        myButton = NULL;
    }

    void pwCheckBox::OnUpdate(float FrameTime)
    {
        if (!getActive())
            return;

        myButton->OnUpdate(FrameTime);
        myChanged = false;
        

        if (myButton->GetClicked())
        {
            myStatus = !myStatus;
            myChanged = true;
        }
    }

    void pwCheckBox::draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        if (getVisible())
        {
            target.draw(*myButton);

            if (myStatus)
            {
                target.draw(myDrawable, sf::RenderStates(states.blendMode, states.transform * getTransform(), states.texture, states.shader));
            }

            target.draw((pwGameObject)*this, states);
        }
    }

    bool pwCheckBox::GetHover() const
    {
        return myButton->GetHover();
    }

    pwButton* pwCheckBox::Button()
    {
        return myButton;
    }
}
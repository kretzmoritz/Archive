#ifndef PWGUICHECKBOX
#define PWGUICHECKBOX

#include "pwGUI.hpp"

namespace pwGUI
{
    class pwButton;
    class pwCheckBox : public pwGUIObject
    {

    public:

        pwCheckBox();
        ~pwCheckBox();

        void OnUpdate(float FrameTime);
        void draw(sf::RenderTarget& target, sf::RenderStates states) const;

        bool GetHover() const;

        void SetStatus(bool Status)     {myStatus = Status;}
        bool GetStatus() const          {return myStatus;}


        sf::Sprite* StatusDrawable()                {return &myDrawable;}
        pwButton* Button();

        bool getChanged() const                     {return myChanged;}

    private:
        sf::Sprite    myDrawable;
        pwButton*     myButton;

        bool myChanged;

        bool myStatus;
    };
}

#endif
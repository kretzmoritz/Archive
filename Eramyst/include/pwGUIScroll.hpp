#ifndef PWGUISCROLL
#define PWGUISCROLL

#include "pwGUI.hpp"

namespace pwGUI
{
    class pwButton;
    class pwScroll : public pwGUIObject
    {

    public:

        pwScroll();
        ~pwScroll();

        void OnUpdate(float FrameTime);
        void draw(sf::RenderTarget& target, sf::RenderStates states) const;

        bool GetHover();
        bool GetFocus();

        void SetCurrentLine(unsigned int CurrentLine) {myCurrentLine = CurrentLine;}
        unsigned int GetCurrentLine()                 {return myCurrentLine;}

        void SetTotalLines(unsigned int TotalLines)   {myTotalLines = TotalLines;}
        unsigned int GetTotalLines()                  {return myTotalLines;}

        void SetSprite(sf::Sprite* Sprite)          {myDrawable = Sprite;}
        void SetSlider(pwButton* Slider)            {mySlider = Slider;}
        void SetSize(const sf::Vector2i& Size);
        sf::Vector2i GetSize()               {return sf::Vector2i((int)myCollisionRect.GetSize().x, (int)myCollisionRect.GetSize().y);}

        bool GetHorizontal() {return myHorizontal;}
        void SetHorizontal(bool Horizontal) {myHorizontal = Horizontal;}


    protected:
        void SetFocus(bool Focus);


    private:

        sf::Sprite*   myDrawable;
        pwButton*     mySlider;
        pwCollisionRect myCollisionRect;

        unsigned int myTotalLines;
        unsigned int myCurrentLine;

        bool myFocused;
        bool myHover;
        bool myHorizontal;

        bool myMouseDown;
    };
}

#endif
#ifndef PWGUISLIDER
#define PWGUISLIDER

#include "pwGUI.hpp"

namespace pwGUI
{
    class pwButton;
    class pwSlider : public pwGUIObject
    {

    public:

        pwSlider();
        ~pwSlider();

        void OnUpdate(float FrameTime);
        void draw(sf::RenderTarget& target, sf::RenderStates states) const;

        bool GetHover();
        bool GetFocus();

        void SetPercentage(float Percentage)        {myPercentage = Percentage;}
        float GetPercentage();

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
        sf::IntRect   myIdleRect;
        sf::IntRect   myStateRect;
        pwButton*     mySlider;
        pwCollisionRect myCollisionRect;

        float myPercentage;

        bool myFocused;
        bool myHover;
        bool myHorizontal;

        bool myMouseDown;
    };
}

#endif
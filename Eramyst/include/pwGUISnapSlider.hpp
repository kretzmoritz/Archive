#ifndef PWGUISNAPSLIDER
#define PWGUISNAPSLIDER

#include "pwGUI.hpp"

namespace pwGUI
{
    class pwButton;
    class pwSnapSlider : public pwGUIObject
    {

    public:

        pwSnapSlider();
        ~pwSnapSlider();

        void OnUpdate(float FrameTime);
        void draw(sf::RenderTarget& target, sf::RenderStates states) const;

        bool GetHover() const;
        bool GetFocus() const;

        void SetIndex(unsigned int Index)           {myIndex = Index; myPercentage = myIndex * myPerIndexPercentage;}
        unsigned int GetIndex() const               {return myIndex;}

        void SetSprite(sf::Sprite* Sprite)          {myDrawable = Sprite;}
        void SetSlider(pwButton* Slider)            {mySlider = Slider;}
        void SetIndexCount(unsigned int IndexCount) {if (IndexCount > 1) {myIndexCount = IndexCount; myPerIndexPercentage = 1.0f / (float)(myIndexCount-1);}}
        unsigned int GetIndexCount() const          {return myIndexCount;}

        void SetSize(const sf::Vector2i& Size);
        sf::Vector2i GetSize() const                {return sf::Vector2i((int)myCollisionRect.GetSize().x, (int)myCollisionRect.GetSize().y);}
        void SetSnapSize(const sf::Vector2i& Size);

        bool getChanged() const                     {return myChanged;}


    protected:
        void SetFocus(bool Focus);


    private:

        sf::Sprite*   myDrawable;
        sf::IntRect   myIdleRect;
        sf::IntRect   myStateRect;
        sf::IntRect   mySnapRect;
        pwButton*     mySlider;
        pwCollisionRect myCollisionRect;

        float myPercentage;

        unsigned int myIndex;
        unsigned int myIndexCount;
        float myPerIndexPercentage;

        bool myFocused;
        bool myHover;
        bool myHorizontal;
        bool myChanged;

        bool myMouseDown;
    };
}

#endif
#ifndef PWGUIBUTTON
#define PWGUIBUTTON

#include "pwGUI.hpp"

namespace pwGUI
{
    class pwButton : public pwGUIObject
    {
        friend pwGUIObject;

    public:

        enum pwButtonState
        {
            Idle,
            Hover,
            Down
        };

        pwButton();
        ~pwButton();

        void OnUpdate(float FrameTime);
        void draw(sf::RenderTarget& target, sf::RenderStates states) const;

        bool GetHover() const;
        bool GetClicked(bool resetClicked = true);

        void SetInstantClickType(bool Instant);
        bool GetInstantClickType();

        void BindDrawableToState(unsigned int DrawableID, pwButtonState State);
        void UnbindDrawable(unsigned int DrawableID);


    private:

        std::list<std::pair<unsigned int, sf::Drawable*>> myIdleDrawableList;
        std::list<std::pair<unsigned int, sf::Drawable*>> myHoverDrawableList;
        std::list<std::pair<unsigned int, sf::Drawable*>> myDownDrawableList;

        bool myClicked;
        bool myHover;

        bool myInstantClickType;

        bool myMouseDown;
    };
}

#endif
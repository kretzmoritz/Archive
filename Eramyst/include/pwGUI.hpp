#ifndef PWGUI
#define PWGUI

#include <SFML\Graphics.hpp>
#include "pwGameObject.hpp"

class pwGUIObject : public pwGameObject
{

public:
    pwGUIObject() : myVisible(true), myActive(true) {}
    ~pwGUIObject() {}

    void setVisible(bool Visible = true)           {myVisible = Visible;}
    void setActive(bool Active = true)             {myActive = Active;}

    bool getVisible() const                        {return myVisible;}
    bool getActive()  const                        {return myActive;}

    virtual bool takeFocus() const                 {myFocus = false; return true;}
    virtual void giveFocus() const                 {myFocus = true;}

private:

    bool myVisible;
    bool myActive;

    mutable bool myFocus;
};

#include "pwGUIScroll.hpp"
#include "pwGUISlider.hpp"
#include "pwGUISnapSlider.hpp"
#include "pwGUICheckBox.hpp"
#include "pwGUIButton.hpp"

#endif
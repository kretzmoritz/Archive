#include "..\..\include\Game\CDisplayBar.h"

CDisplayBar *CDisplayBar::p;

void CDisplayBar::drawHealth()
{
	setHealth(getHealth());

    if (!CInterface::get()->getPreShutdown())
    {
	    glGraphics->DrawOverlay(&text_health, 2);
	    glGraphics->DrawOverlay(&bar_health, 3);
	    glGraphics->DrawOverlay(&bar_health_border, 2);
    }
}

void CDisplayBar::drawRage()
{
    if (!CInterface::get()->getPreShutdown())
    {
	    glGraphics->DrawOverlay(&text_rage, 2);
	    glGraphics->DrawOverlay(&bar_rage, 3);
	    glGraphics->DrawOverlay(&bar_rage_border, 2);
    }
}

float CDisplayBar::getHealth() const
{
	return CInterface::get()->getCurrentLife();
}

void CDisplayBar::setHealth(float _health)
{
	sf::Vector2f fCurrentSize = sf::Vector2f(_health * fStandardSize.x, fStandardSize.y);
	bar_health.setSize(fCurrentSize);
}

void CDisplayBar::setRage(float _rage)
{
	sf::Vector2f fCurrentSize = sf::Vector2f((_rage * fStandardSize.x) > fStandardSize.x ? fStandardSize.x : (_rage * fStandardSize.x), fStandardSize.y);
	bar_rage.setSize(fCurrentSize);
}

CDisplayBar* CDisplayBar::get()
{
	if(p == NULL)
	{
		p = new CDisplayBar();
	}
			
	return p;
}
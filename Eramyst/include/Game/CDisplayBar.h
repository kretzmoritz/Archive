////////////////////////////////////////////////////////////
// Moritz Kretz
// 2012-08-07
// System implementation of display bar
////////////////////////////////////////////////////////////

#ifndef DISPLAY_BAR
#define DISPLAY_BAR

#define PWUSEGLOBALSINGLETONS

#include "..\pwSystem.hpp"
#include "CInterface.h"

class CDisplayBar
{
	public:
		CDisplayBar()
		{
			fStandardSize.x = 300.0f;
			fStandardSize.y = 15.0f;
			
			text_health.setPosition(25.0f, 5.0f);
			text_health.setCharacterSize(48);
			text_health.setFont(*glDataBox->GetFont("pecita.otf"));
			text_health.setColor(sf::Color(255, 255, 255, 255));
			text_health.setString("Health: ");
			
			bar_health.setSize(fStandardSize);
			bar_health.setPosition(200.0f, 35.0f);
			bar_health.setFillColor(sf::Color(255, 255, 255, 150));
			bar_health_border.setSize(fStandardSize + sf::Vector2f(10.0f, 10.0f));
			bar_health_border.setPosition(200.0f - 5.0f, 35.0f - 5.0f);
			bar_health_border.setFillColor(sf::Color(0, 0, 0, 150));

			text_rage.setPosition(25.0f, 50.0f);
			text_rage.setCharacterSize(48);
			text_rage.setFont(*glDataBox->GetFont("pecita.otf"));
			text_rage.setColor(sf::Color(255, 255, 255, 255));
			text_rage.setString("Rage: ");

			bar_rage.setSize(fStandardSize);
			bar_rage.setPosition(200.0f, 80.0f);
			bar_rage.setFillColor(sf::Color(255, 255, 255, 150));
			bar_rage_border.setSize(fStandardSize + sf::Vector2f(10.0f, 10.0f));
			bar_rage_border.setPosition(200.0f - 5.0f, 80.0f - 5.0f);
			bar_rage_border.setFillColor(sf::Color(0, 0, 0, 150));
		}
		~CDisplayBar() {}

		void drawHealth();
		void drawRage();

		float getHealth() const;
		void setHealth(float _health);

		void setRage(float _health);

		static CDisplayBar* get();

	private:
		static CDisplayBar *p;

		sf::Text text_health;
		sf::RectangleShape bar_health;
		sf::RectangleShape bar_health_border;

		sf::Text text_rage;
		sf::RectangleShape bar_rage;
		sf::RectangleShape bar_rage_border;
		
		sf::Vector2f fStandardSize;
};

#endif
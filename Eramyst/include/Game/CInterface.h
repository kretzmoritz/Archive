////////////////////////////////////////////////////////////
// Philipp Wittershagen
// 2012-07-16
// System implementation of the interface 
// Uses pwGraphicsSystem-Layer 2
////////////////////////////////////////////////////////////

#ifndef INTERFACE
#define INTERFACE

#define PWUSEGLOBALSINGLETONS

#include "..\pwSystem.hpp"

class CInterface
{
public:
    CInterface() {m_bInitialized = false; m_preShutdowned = false;};
    ~CInterface() {};

    void setCurrentLife(float currentLifePercentage);
    float getCurrentLife();

    void setCurrentScore(unsigned int currentScore);
    unsigned int getCurrentScore();

    void setCurrentModifier(unsigned int currentScore);
    unsigned int getCurrentModifier();

    void setCurrentLevelTime(float currentLevelTime);
    float getCurrentLevelTime();

    void setCurrentRageWaveProgress(float currentWaveProgress);
    float getCurrentRageWaveProgress();

    void setCurrentShieldPercentage(float currentShieldPercentage);
    float getCurrentShieldPercentage();
    
    void setCurrentRageBlurY(float currentRageBlurYPosition);
    float getCurrentRageBlurY();

    void setSpaceBlur(bool useSpaceBlur);
    bool getSpaceBlur();

    bool getPreShutdown()   {return m_preShutdowned;}

    const sf::Shader* getLifeFeedbackShader();

    void init();
    void reset();
    void shutdown();
    void pre_shutdown();
    void restart();

    void startRage();
    void endRage();

    void draw();

    static CInterface* get();

private:
    float m_fCurrentYRageBlurPos;
    float m_fCurrentLife;
    bool m_bUseSpaceBlur;
    bool m_bRage;
    bool m_preShutdowned;
    float m_bLifeLostOpacity;
    unsigned int m_iCurrentScore;
	unsigned int m_iCurrentModifier;
	float m_fCurrentLevelTime;
    float m_fCurrentRageWaveProgress;
    float m_fCurrentShieldPercentage;
    sf::Text    m_ciScore;
	sf::Text    m_ciModifier;
	sf::Text    m_ciLevelTime;
    sf::Sprite  m_ciRageOverlay;
    sf::Shader* m_ciLifeFeedbackShader;
    sf::Shader* m_ciRageWaveShader;
    sf::Shader* m_ciRageBlurXShader;
    sf::Shader* m_ciRageBlurYShader;
    sf::Shader* m_ciShieldShader;
    pwAnimation m_ciShieldAnimation;

    bool m_bInitialized;

    static CInterface* m_s_p_ciInstance;
};

#endif
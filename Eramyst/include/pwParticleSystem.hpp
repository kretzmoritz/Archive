#ifndef PWPARTICLESYSTEM
#define PWPARTICLESYSTEM

#include "pwGameObject.hpp"

class pwParticleSystem;

class pwParticle : public pwGameObject
{
    friend pwParticleSystem;

public:

    pwParticle(const sf::Texture* Texture);
    ~pwParticle();

    void OnUpdate(float FrameTime);

protected:
    void SetParticleLifeTime(float ParticleLifeTime = 5.0f);
    
    void SetSpinTransformation(float Start, float End);
    void SetScaleTransformation(float Start, float End);
    void SetColorTransformation(const sf::Color& Start, const sf::Color End);

private:

    float myAge;
    float myParticleLifeTime;
    sf::Sprite mySprite;

    sf::Vector2f myCyclesPerSecond;
    sf::Vector2f myScale;
    sf::Vector2<sf::Color> myParticleColor;
};

class pwParticleSystem : public pwGameObject
{

public:

    pwParticleSystem();
    ~pwParticleSystem();

    void SetEmission(float ParticlesPerSecond = 100.0f);
    float GetEmission() const                               {return myParticlesPerSecond;}

    void SetParticleLifeTime(float ParticleLifeTime = 5.0f);
    float GetParticleLifeTime() const                       {return myParticleLifeTime;}

    void SetMaxParticleCount(unsigned int MaxParticleCount = 500);
    unsigned int GetMaxParticleCount() const                {return myMaxParticleCount;}

    void SetSpreadAngle(float SpreadAngle = 0.0f);
    float GetSpreadAngle() const                            {return myHalfSpreadAngle * 2.0f;}

    void SetParticleStartVelocity(float ParticleStartVelocity = 100.0f)         {myParticleStartVelocity = ParticleStartVelocity;}
    float GetParticleStartVelocity() const                                      {return myParticleStartVelocity;}
    void SetParticleGravityMultiplier(float ParticleGravityMultiplier = 1.0f)   {myParticleGravityMultiplier = ParticleGravityMultiplier;}
    float GetParticleGravityMultiplier() const                                  {return myParticleGravityMultiplier;}
    void SetParticleMass(float ParticleMass = 1.0f)                             {myParticleMass = ParticleMass;}
    float GetParticleMass() const                                               {return myParticleMass;}
    void SetParticleSpinTransformation(float Start, float End, float Variation);
    const sf::Vector3f& GetParticleSpinTransformation() const                   {return myParticleSpinTransformation;}
    void SetParticleScaleTransformation(float Start, float End, float Variation);
    const sf::Vector3f& GetParticleScaleTransformation() const                  {return myParticleScaleTransformation;}
    void SetParticleColorTransformation(const sf::Color& Start, const sf::Color& End, const sf::Color& Variation);
    const sf::Vector3<sf::Color>& GetParticleColorTransformation() const        {return myParticleColorTransformation;}

    void SetTexture(const sf::Texture* Texture)                                 {myTexture = Texture;}

    void OnUpdate(float FrameTime);
    void draw(sf::RenderTarget& target, sf::RenderStates states) const;

    static bool not_alive (pwParticle* value);

private:

    void UpdateDirectionAngle();

    float           myParticlesPerSecond;
    float           myParticlesPerSecondInv;
    float           myDirectionAngle;
    float           myHalfSpreadAngle;
    sf::Vector2f    myDirectionNormalMax;
    sf::Vector2f    myDirectionNormalMin;
    unsigned int    myMaxParticleCount;

    float myParticleLifeTime;

    float myParticleStartVelocity;
    float myParticleGravityMultiplier;
    float myParticleMass;
    sf::Vector3f myParticleSpinTransformation;
    sf::Vector3f myParticleScaleTransformation;
    sf::Vector3<sf::Color> myParticleColorTransformation;

    const sf::Texture* myTexture;

    float myParticleTimeCounter;

    std::list<pwParticle*> myParticleList;
};

#endif
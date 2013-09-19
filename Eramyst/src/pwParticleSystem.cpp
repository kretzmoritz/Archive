#include "..\include\pwParticleSystem.hpp"

#define CUT_UINT8(x) (((x) <= 0) ? 0 : ((x) >= 255) ? 255 : (x))

pwParticle::pwParticle(const sf::Texture* Texture) :
    myAge(0.0f),
    myParticleLifeTime(5.0f),
    
    myCyclesPerSecond(0.0f, 0.0f),
    myScale(1.0f, 1.0f),
    myParticleColor(sf::Color(255, 255, 255), sf::Color(255, 255, 255))
{
    mySprite.setTexture(*Texture);
    mySprite.setOrigin(Texture->getSize().x / 2.0f, Texture->getSize().y / 2.0f);

    AddDrawableObject(0, &mySprite);
}
pwParticle::~pwParticle()
{
}

void pwParticle::SetParticleLifeTime(float ParticleLifeTime)
{
    if (myParticleLifeTime <= 0.0f)
    {
        pwLog::get()->WriteLine("pwParticle::SetPartcleLifeTime: ParticleLifeTime must not be 0.0f or less! [Change is ignored]", pwLogType::pwLTCaution);
        return;
    }

    myParticleLifeTime = ParticleLifeTime;
}

void pwParticle::SetSpinTransformation(float Start, float End)
{
    myCyclesPerSecond.x = Start;
    myCyclesPerSecond.y = End;
}
void pwParticle::SetScaleTransformation(float Start, float End)
{
    myScale.x = Start;
    myScale.y = End;
}
void pwParticle::SetColorTransformation(const sf::Color& Start, const sf::Color End)
{
    myParticleColor.x = Start;
    myParticleColor.y = End;
}

void pwParticle::OnUpdate(float FrameTime)
{
    myAge += FrameTime; 
 
    if (myAge >= myParticleLifeTime) 
    {
        Kill();
    }

    float LifePercentage = myAge / myParticleLifeTime;
    float LifePercentageNeg = 1.0f - LifePercentage;

    this->rotate(360.0f * FrameTime * (myCyclesPerSecond.x * LifePercentageNeg + myCyclesPerSecond.y * LifePercentage));

    float TmpScale = (myScale.x * LifePercentageNeg + myScale.y * LifePercentage);
    this->setScale(TmpScale, TmpScale);

    float TmpR = (float)myParticleColor.x.r * LifePercentageNeg + (float)myParticleColor.y.r * LifePercentage;
    float TmpG = (float)myParticleColor.x.g * LifePercentageNeg + (float)myParticleColor.y.g * LifePercentage;
    float TmpB = (float)myParticleColor.x.b * LifePercentageNeg + (float)myParticleColor.y.b * LifePercentage;
    float TmpA = (float)myParticleColor.x.a * LifePercentageNeg + (float)myParticleColor.y.a * LifePercentage;
    mySprite.setColor(sf::Color((sf::Uint8)TmpR, (sf::Uint8)TmpG, (sf::Uint8)TmpB, (sf::Uint8)TmpA));
}

pwParticleSystem::pwParticleSystem() : 
    myParticlesPerSecond(100.0f),
    myParticlesPerSecondInv(0.01f),
    myDirectionAngle(0.0f),
    myHalfSpreadAngle(0.0f),
    myDirectionNormalMin(0.0f, -1.0f),
    myDirectionNormalMax(0.0f, -1.0f),
    myMaxParticleCount(500),

    myParticleLifeTime(5.0f),

    myParticleStartVelocity(100.0f),
    myParticleGravityMultiplier(1.0f),
    myParticleMass(1.0f),
    myParticleSpinTransformation(0.0f, 0.0f, 0.0f),
    myParticleScaleTransformation(1.0f, 1.0f, 0.0f),
    myParticleColorTransformation(sf::Color(255, 255, 255), sf::Color(255, 255, 255), sf::Color(255, 255, 255)),

    myTexture(NULL),

    myParticleTimeCounter(0.0f)
{
}
pwParticleSystem::~pwParticleSystem()
{
    for (std::list<pwParticle*>::iterator i = myParticleList.begin(); i != myParticleList.end(); i++)
    {
        delete (*i);
    }

    myParticleList.clear();
}

void pwParticleSystem::SetEmission(float ParticlesPerSecond)
{
    myMaxParticleCount = (unsigned int) (ParticlesPerSecond * myParticleLifeTime);
   
    myParticlesPerSecond = ParticlesPerSecond;

    if (myParticlesPerSecond != 0.0f)
        myParticlesPerSecondInv = 1.0f / myParticlesPerSecond;
    else
        myParticlesPerSecondInv = 0.0f;
}

void pwParticleSystem::SetParticleLifeTime(float ParticleLifeTime)
{
    myMaxParticleCount = (unsigned int) (myParticlesPerSecond * ParticleLifeTime);

    myParticleLifeTime = ParticleLifeTime;
}

void pwParticleSystem::SetMaxParticleCount(unsigned int MaxParticleCount)
{
    if ((myParticlesPerSecond * myParticleLifeTime) > MaxParticleCount) 
    {
        myParticlesPerSecond = MaxParticleCount / myParticleLifeTime;
        myParticlesPerSecondInv = 1.0f / myParticlesPerSecond;
    }

    myMaxParticleCount = MaxParticleCount;
}

void pwParticleSystem::UpdateDirectionAngle()
{
    if (myDirectionAngle != getRotation())
    {
        myDirectionAngle = getRotation();
        myDirectionNormalMin.x = pwHelper::math::sind(myDirectionAngle - myHalfSpreadAngle);
        myDirectionNormalMin.y = -pwHelper::math::cosd(myDirectionAngle - myHalfSpreadAngle);
        myDirectionNormalMax.x = pwHelper::math::sind(myDirectionAngle + myHalfSpreadAngle);
        myDirectionNormalMax.y = -pwHelper::math::cosd(myDirectionAngle + myHalfSpreadAngle);
    }

}
void pwParticleSystem::SetSpreadAngle(float SpreadAngle)
{
    UpdateDirectionAngle();
    myHalfSpreadAngle = SpreadAngle / 2.0f;
    myDirectionNormalMin.x = pwHelper::math::sind(myDirectionAngle - myHalfSpreadAngle);
    myDirectionNormalMin.y = -pwHelper::math::cosd(myDirectionAngle - myHalfSpreadAngle);
    myDirectionNormalMax.x = pwHelper::math::sind(myDirectionAngle + myHalfSpreadAngle);
    myDirectionNormalMax.y = -pwHelper::math::cosd(myDirectionAngle + myHalfSpreadAngle);
}

void pwParticleSystem::SetParticleSpinTransformation(float Start, float End, float Variation)
{
    myParticleSpinTransformation.x = Start;
    myParticleSpinTransformation.y = End;
    myParticleSpinTransformation.z = Variation;
}
void pwParticleSystem::SetParticleScaleTransformation(float Start, float End, float Variation)
{
    myParticleScaleTransformation.x = Start;
    myParticleScaleTransformation.y = End;
    myParticleScaleTransformation.z = Variation;
}
void pwParticleSystem::SetParticleColorTransformation(const sf::Color& Start, const sf::Color& End, const sf::Color& Variation)
{
    myParticleColorTransformation.x = Start;
    myParticleColorTransformation.y = End;
    myParticleColorTransformation.z = Variation;
}

void pwParticleSystem::OnUpdate(float FrameTime)
{
    UpdateDirectionAngle();
    myParticleList.remove_if(not_alive);

    myParticleTimeCounter += FrameTime;

    if (myParticlesPerSecond != 0.0f)
    for (; myParticleTimeCounter >= myParticlesPerSecondInv; myParticleTimeCounter -= myParticlesPerSecondInv)
    {
        pwParticle* TmpParticle = new pwParticle(myTexture);
        TmpParticle->setPosition(getPosition());
        TmpParticle->SetStartVelocity(sf::Vector2f(myParticleStartVelocity * pwHelper::math::random::getFloatEx(myDirectionNormalMin.x, myDirectionNormalMax.x),
                                                   myParticleStartVelocity * pwHelper::math::random::getFloatEx(myDirectionNormalMin.y, myDirectionNormalMax.y)));
        
        float X = pwHelper::math::random::getFloatEx(-1.0f, 1.0f);

        TmpParticle->SetSpinTransformation(myParticleSpinTransformation.x + myParticleSpinTransformation.z * pwHelper::math::random::getFloatEx(-1.0f, 1.0f), 
                                           myParticleSpinTransformation.y + myParticleSpinTransformation.z * pwHelper::math::random::getFloatEx(-1.0f, 1.0f));
        TmpParticle->SetColorTransformation(sf::Color((sf::Uint8)CUT_UINT8((float)myParticleColorTransformation.x.r + (float)myParticleColorTransformation.z.r * pwHelper::math::random::getFloatEx(-1.0f, 1.0f)),
                                                      (sf::Uint8)CUT_UINT8((float)myParticleColorTransformation.x.g + (float)myParticleColorTransformation.z.g * pwHelper::math::random::getFloatEx(-1.0f, 1.0f)),
                                                      (sf::Uint8)CUT_UINT8((float)myParticleColorTransformation.x.b + (float)myParticleColorTransformation.z.b * pwHelper::math::random::getFloatEx(-1.0f, 1.0f)),
                                                      (sf::Uint8)CUT_UINT8((float)myParticleColorTransformation.x.a + (float)myParticleColorTransformation.z.a * pwHelper::math::random::getFloatEx(-1.0f, 1.0f))),
                                            sf::Color((sf::Uint8)CUT_UINT8((float)myParticleColorTransformation.y.r + (float)myParticleColorTransformation.z.r * pwHelper::math::random::getFloatEx(-1.0f, 1.0f)),
                                                      (sf::Uint8)CUT_UINT8((float)myParticleColorTransformation.y.g + (float)myParticleColorTransformation.z.g * pwHelper::math::random::getFloatEx(-1.0f, 1.0f)),
                                                      (sf::Uint8)CUT_UINT8((float)myParticleColorTransformation.y.b + (float)myParticleColorTransformation.z.b * pwHelper::math::random::getFloatEx(-1.0f, 1.0f)),
                                                      (sf::Uint8)CUT_UINT8((float)myParticleColorTransformation.y.a + (float)myParticleColorTransformation.z.a * pwHelper::math::random::getFloatEx(-1.0f, 1.0f))));
        TmpParticle->SetScaleTransformation(myParticleScaleTransformation.x + myParticleScaleTransformation.z * pwHelper::math::random::getFloatEx(-1.0f, 1.0f), 
                                            myParticleScaleTransformation.y + myParticleScaleTransformation.z * pwHelper::math::random::getFloatEx(-1.0f, 1.0f));

        TmpParticle->SetGravityMultiplier(myParticleGravityMultiplier);
        TmpParticle->SetParticleLifeTime(myParticleLifeTime);
        myParticleList.push_back(TmpParticle);
    }


    for (std::list<pwParticle*>::iterator i = myParticleList.begin(); i != myParticleList.end(); i++)
    {
        (*i)->ApplyForce(sf::Vector2f(0.0f, (*i)->GetMass() * pwGameLogics::get()->GetGravity() * (*i)->GetGravityMultiplier()));
        (*i)->Update(FrameTime);
        (*i)->OnUpdate(FrameTime);
    }
}

void pwParticleSystem::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    for (std::list<pwParticle*>::const_iterator i = myParticleList.begin(); i != myParticleList.end(); i++)
    {
        (*i)->draw(target, sf::RenderStates(sf::BlendAdd));
    }
}

// a predicate implemented as a function:
bool pwParticleSystem::not_alive (pwParticle* value) 
{
    if (!value->GetAlive())
	{
        delete value;
        return true;
	}
	return false; 
}
#include "..\..\include\Game\CBeatSystem.h"
#include "..\..\include\Game\CPlay.h"

unsigned int CBeatSystem::m_uiCurrentBPM;
float CBeatSystem::m_fCurrentPerBeatTime;

void CBeatSystem::SetBPM(unsigned int BPM)
{
    m_uiCurrentBPM = BPM;
    m_fCurrentPerBeatTime = 60.0f / (float)BPM;

    pwIniFile* TmpIni = glDataBox->GetIniFile("leveldata.ini");

    CSystem::setMovementSpeed(CSystem::getDistance() / ((float)TmpIni->GetInt("EnemyWalkDurationMultiplier", CLevelData::getCurrentLevelString()) * m_fCurrentPerBeatTime));
    CProjectile::setFlightTime(m_fCurrentPerBeatTime * (float)(TmpIni->GetInt("ProjectileFlightDurationMultiplier", CLevelData::getCurrentLevelString()) / 2)); // half beats
    CSystem::setSpawnTime(m_fCurrentPerBeatTime * (float)TmpIni->GetInt("SquadSpawnIntervalMultiplier", CLevelData::getCurrentLevelString()));
    CMusicSystem::get()->SetBPM(BPM);
}

unsigned int CBeatSystem::GetBPM()
{
    return m_uiCurrentBPM;
}

float CBeatSystem::GetPerBeatTime()
{
    return m_fCurrentPerBeatTime;
}
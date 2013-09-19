Scriptname _00E_ExpOnQuest extends Form
{Quest complete: Experience points are given to the player}

Import _00E_LevelSystem

GlobalVariable Property PlayerExp auto
GlobalVariable Property PlayerLevel auto
float Property RewardExp auto

Event OnComplete()
	PlayerExp.Mod(RewardExp)
	CheckForLevelUp(PlayerExp, PlayerLevel)
endEvent
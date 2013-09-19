Scriptname _00E_ExpOnDeath extends ObjectReference
{Monster death: Experience points are given to the player}

Import _00E_LevelSystem

GlobalVariable Property PlayerExp auto
GlobalVariable Property PlayerLevel auto
float Property RewardExp auto

Event OnDeath(Actor akKiller)
	if (akKiller == Game.GetPlayer())
		PlayerExp.Mod(RewardExp)
		CheckForLevelUp(PlayerExp, PlayerLevel)
	endIf
endEvent
Scriptname _00E_LevelSystem   
{Handles all stat changes as well as other things associated with leveling up}

Import Math

function CheckForLevelUp(GlobalVariable CurrentExp, GlobalVariable CurrentLevel) global
	float fCurrentExp = CurrentExp.GetValue()
	int iCurrentLevel = CurrentLevel.GetValueInt()

	float StartExp = 10.0
	float NeededExp
	bool LevelCheckRequired = true

	while LevelCheckRequired
		LevelCheckRequired = false
		NeededExp = (pow(iCurrentLevel, 3) + iCurrentLevel*2) * StartExp
	
		if fCurrentExp >= NeededExp
			LevelCheckRequired = true
			
			iCurrentLevel += 1
			CurrentLevel.SetValueInt(iCurrentLevel)
			Debug.MessageBox(iCurrentLevel)

			NeededExp = (pow(iCurrentLevel, 3) + iCurrentLevel*2) * StartExp
			Debug.MessageBox(NeededExp)

			SetStats()
		endif
	endWhile
EndFunction

function SetStats() global
	Game.GetPlayer().ModActorValue("health", 100)
	Game.GetPlayer().ModActorValue("destruction", 100)
EndFunction
Scriptname _00E_Ability_FireWall extends ObjectReference

ObjectReference[] array_oil
Float fOffset

Function MoveRefToPositionRelativeToPlayer(int akSubject, Float OffsetDistance = 0.0, Float OffsetAngle = 0.0)
	float AngleZ = Game.GetPlayer().GetAngleZ() + OffsetAngle
	float OffsetX = OffsetDistance * Math.Sin(AngleZ)
	float OffsetY = OffsetDistance * Math.Cos(AngleZ)
	array_oil[akSubject].MoveTo(SELF, OffsetX, OffsetY, 0.0)
EndFunction

Event OnInit()
	array_oil = new ObjectReference[11]
	int i = 0
	fOffset = 50.0

	array_oil[i] = SELF.PlaceAtMe(Game.GetForm(0x000862CC))
	MoveRefToPositionRelativeToPlayer(i, 0.0, 0.0)
	array_oil[i].Enable()
	array_oil[i].damageObject(5.0) ;Makes oil start to burn
	i += 1

	while i < 11
		array_oil[i] = SELF.PlaceAtMe(Game.GetForm(0x000862CC))
		MoveRefToPositionRelativeToPlayer(i, fOffset, 90.0)
		array_oil[i].Enable()
		array_oil[i].damageObject(5.0) ;Makes oil start to burn
		i += 1
		array_oil[i] = SELF.PlaceAtMe(Game.GetForm(0x000862CC))
		MoveRefToPositionRelativeToPlayer(i, fOffset, -90.0)
		array_oil[i].Enable()
		array_oil[i].damageObject(5.0) ;Makes oil start to burn
		i += 1

		fOffset += 50.0
	endWhile

	SELF.Delete()
EndEvent
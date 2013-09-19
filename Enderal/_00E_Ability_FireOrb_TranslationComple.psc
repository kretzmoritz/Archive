Scriptname _00E_Ability_FireOrb_TranslationComple extends ObjectReference
{Ends the FireOrb spell with a big boom}

SPELL Property _00E_AbilitySpell_FireExplosion Auto
ObjectReference Property _00E_WeltenbrandxMarker Auto

Event OnTranslationAlmostComplete()
	_00E_WeltenbrandxMarker.MoveTo(_00E_WeltenbrandxMarker, 0, 0, Game.GetPlayer().GetHeight())

	_00E_AbilitySpell_FireExplosion.RemoteCast(_00E_WeltenbrandxMarker, Game.GetPlayer(), _00E_WeltenbrandxMarker)
	SELF.Disable()

	_00E_WeltenbrandxMarker.MoveTo(_00E_WeltenbrandxMarker, 0, 0, -Game.GetPlayer().GetHeight())

	ObjectReference[] array_oil = new ObjectReference[75]
	int i = 0

	while i < 75
		i += 1
		array_oil[i] = _00E_WeltenbrandxMarker.PlaceAtMe(Game.GetForm(0x000862CC))
		array_oil[i].MoveTo(_00E_WeltenbrandxMarker, Utility.RandomInt(-500, 500), Utility.RandomInt(-500, 500), 0)
		array_oil[i].DamageObject(5.0)
		array_oil[i].Enable()
	endWhile
EndEvent
Scriptname _00E_Ability_FireOrb extends ObjectReference
{An orb of fire from the heavens that burns enemies in your path}

ObjectReference Property _00E_WeltenbrandOrb Auto
ObjectReference Property _00E_WeltenbrandxMarker Auto

Event OnInit()
	_00E_WeltenbrandxMarker.MoveTo(SELF, 0, 0, 0)
	_00E_WeltenbrandOrb.MoveTo(SELF, 0, 0, 1000)
	_00E_WeltenbrandOrb.Enable(true) ;true = fadeIn
	RegisterForSingleUpdate(2.0)
endEvent

Event OnUpdate()
	_00E_WeltenbrandOrb.SplineTranslateToRef(_00E_WeltenbrandxMarker, 1.0, 2000.0, 0.01) ;0.01 rotation speed -> no bugs surface
	SELF.Delete()
endEvent

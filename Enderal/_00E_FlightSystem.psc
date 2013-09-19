Scriptname _00E_FlightSystem extends ObjectReference
{Enables mount to follow a predefined path. Be it land, water or air.}

ObjectReference[] Property Path Auto
Float Property Magnitude Auto
Float Property Speed Auto
Float Property RotationSpeed Auto
Float Property StartDelay Auto
Int i = 0

Event OnActivate(ObjectReference akActionRef)
	if i != -1
		Game.DisablePlayerControls(true, true, true, true, true, true, true)

		Utility.Wait(StartDelay)
		Game.ForceFirstPerson()
		SELF.SplineTranslateToRef(Path[i], Magnitude, Speed, RotationSpeed)
	endif
EndEvent

Event OnTranslationAlmostComplete()
	i += 1

	if i < Path.Length
		SELF.SplineTranslateToRef(Path[i], Magnitude, Speed, RotationSpeed)
	else
		i = -1
		SELF.StopTranslation()

		Game.ForceThirdPerson()
		Game.EnablePlayerControls()
	endif
EndEvent
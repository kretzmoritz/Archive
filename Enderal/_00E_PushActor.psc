Scriptname _00E_PushActor extends activemagiceffect

Float Property strength Auto

Event OnEffectStart(Actor akTarget, Actor akCaster)
	akCaster.PushActorAway(akTarget, strength)
endEvent
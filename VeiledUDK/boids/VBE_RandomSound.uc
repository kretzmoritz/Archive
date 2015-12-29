class VBE_RandomSound extends VeiledBoidExtension;

var() const private SoundCue Sound;

var() const private float Min_NextSoundPlaybackTime;
var() const private float Max_NextSoundPlaybackTime;

var private float NextSoundPlaybackTime;

function Constructor()
{
	NextSoundPlaybackTime = WorldInfo.TimeSeconds + RandRange(Min_NextSoundPlaybackTime, Max_NextSoundPlaybackTime);
}

function Update(float DeltaTime)
{
	if (WorldInfo.TimeSeconds >= NextSoundPlaybackTime)
	{
		PlaySound(Sound);
		
		NextSoundPlaybackTime = WorldInfo.TimeSeconds + RandRange(Min_NextSoundPlaybackTime, Max_NextSoundPlaybackTime);
	}
}
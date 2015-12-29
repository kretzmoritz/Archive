class VBE_RandomVelocity extends VeiledBoidExtension;

var() const private float Min_Speed;
var() const private float Max_Speed;

function Constructor()
{
	if (Is2D)
	{
		AddImpulse(Normal(`VVec(RandRange(-1.0f, 1.0f), RandRange(-1.0f, 1.0f), 0.0f)) * RandRange(Min_Speed, Max_Speed));
	}
	else
	{
		AddImpulse(VRand() * RandRange(Min_Speed, Max_Speed));
	}
}

function TimerUpdate()
{
	if (Is2D)
	{
		AddImpulse(Normal(`VVec(RandRange(-1.0f, 1.0f), RandRange(-1.0f, 1.0f), 0.0f)) * RandRange(Min_Speed, Max_Speed));
	}
	else
	{
		AddImpulse(VRand() * RandRange(Min_Speed, Max_Speed));
	}
}

DefaultProperties
{
	IsTimer = true;

	Min_Speed = 150.0f;
	Max_Speed = 350.0f;
}
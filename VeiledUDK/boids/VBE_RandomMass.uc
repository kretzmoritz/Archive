class VBE_RandomMass extends VeiledBoidExtension;

var() const private float Min_Mass;
var() const private float Max_Mass;

function Constructor()
{
	Mass = RandRange(Min_Mass, Max_Mass);
}

function TimerUpdate()
{
	Mass = RandRange(Min_Mass, Max_Mass);
}

DefaultProperties
{
	IsTimer = true;

	Min_Mass = 4.0f;
	Max_Mass = 6.0f;
}
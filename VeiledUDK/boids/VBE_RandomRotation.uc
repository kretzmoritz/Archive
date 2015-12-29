class VBE_RandomRotation extends VeiledBoidExtension;

var() const private float Min_RotationSpeed;
var() const private float Max_RotationSpeed;

var private Rotator RotationDelta;

function Constructor()
{
	RotationDelta = Rotator(VRand());
}

function Update(float DeltaTime)
{
	SetRotation(Rotation + RotationDelta * RandRange(Min_RotationSpeed, Max_RotationSpeed) * DeltaTime);
}

function TimerUpdate()
{
	RotationDelta = Rotator(VRand());
}

DefaultProperties
{
	IsUpdate = true;
	IsTimer = true;

	Min_RotationSpeed = 1.0f;
	Max_RotationSpeed = 3.0f;
}
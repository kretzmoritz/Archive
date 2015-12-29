class VBE_RotationTowardsVelocity extends VeiledBoidExtension;

function Update(float DeltaTime)
{
	SetRotation(Rotator(Velocity));
}

DefaultProperties
{
	IsUpdate = true;
}
class VBE_SnapToGround extends VeiledBoidExtension;

var bool bGrounded;

function Update(float DeltaTime)
{
	local Vector HitLocation;
	local Vector HitNormal;
	local Vector RaycastOffset;
	local Rotator AlignmentRotator;

	RaycastOffset = `VVec(0.0f, 0.0f, BoidRadius);

	if (Trace(HitLocation, HitNormal, Location - RaycastOffset, Location + RaycastOffset) != none)
	{
		AlignmentRotator = Rotator(HitNormal cross vect(0, 1, 0));
		AlignmentRotator.Yaw = Rotation.Yaw;

		SetLocation(HitLocation);
		SetRotation(AlignmentRotator);

		Velocity.Z = 0.0f;
		bGrounded = true;
	}
	else
	{
		bGrounded = false;
	}
}

DefaultProperties
{
	IsUpdate = true;
}
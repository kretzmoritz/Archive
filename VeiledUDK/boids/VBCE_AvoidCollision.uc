class VBCE_AvoidCollision extends VeiledBoidControllerExtension;

var() const private bool SmoothAdjustment;
var() const private float SmoothAdjustmentWeight;
var() const private float Bounce;

function EarlyUpdate(int i)
{
	local Vector HitLocation;
	local Vector HitNormal;
	local Vector RaycastOffset;
	local Vector NormalizedVelocity;
	local float Magnitude;
	local float RayLength;
	local Vector Reflection;

	if (SmoothAdjustment)
	{
		NormalizedVelocity = Boids[i].Velocity;

		if (Boids[i].Is2D)
		{
			NormalizedVelocity.Z = 0.0f;
			RaycastOffset = `VVec(0.0f, 0.0f, Boids[i].BoidRadius);
		}

		Magnitude = VSize(NormalizedVelocity);

		if (Magnitude == 0.0f)
		{
			return;
		}

		NormalizedVelocity /= Magnitude;
		RayLength = Boids[i].BoidRadius * 5.0f;

		if (Trace(HitLocation, HitNormal, 
			Boids[i].Location + RaycastOffset + NormalizedVelocity * RayLength, 
			Boids[i].Location + RaycastOffset) == none)
		{
			return;
		}

		Reflection = NormalizedVelocity - 2.0f * HitNormal * (NormalizedVelocity dot HitNormal);

		if (Boids[i].Is2D)
		{
			Reflection.Z = 0.0f;
		}

		Boids[i].AddForce(Reflection * Magnitude * (1.0f - VSize(Boids[i].Location - HitLocation) / RayLength) * Boids[i].GetMass() * SmoothAdjustmentWeight);
	}
}

function LateUpdate(int i)
{
	local Vector HitLocation;
	local Vector HitNormal;
	local Vector RaycastOffset;
	local Vector NormalizedVelocity;
	local Vector ReactionImpulse;

	NormalizedVelocity = Boids[i].Velocity;

	if (Boids[i].Is2D)
	{
		NormalizedVelocity.Z = 0.0f;
		RaycastOffset = `VVec(0.0f, 0.0f, Boids[i].BoidRadius);
	}

	NormalizedVelocity = Normal(NormalizedVelocity);

	if (Trace(HitLocation, HitNormal, 
		Boids[i].Location + RaycastOffset + NormalizedVelocity * Boids[i].BoidRadius, 
		Boids[i].Location + RaycastOffset) == none)
	{
		return;
	}

	ReactionImpulse = HitNormal * (Boids[i].Velocity dot HitNormal) * (-1.0f - Bounce) * Boids[i].GetMass();
	
	if (Boids[i].Is2D)
	{
		ReactionImpulse.Z = 0.0f;
	}

	Boids[i].SetLocation(HitLocation - RaycastOffset - NormalizedVelocity * Boids[i].BoidRadius);
	Boids[i].AddImpulse(ReactionImpulse);
}

DefaultProperties
{
	IsEarly = true;
	IsLate = true;

	SmoothAdjustment = true;
	SmoothAdjustmentWeight = 25.0f;
	Bounce = 0.3f;
}
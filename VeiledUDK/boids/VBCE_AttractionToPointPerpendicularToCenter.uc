class VBCE_AttractionToPointPerpendicularToCenter extends VeiledBoidControllerExtension;

struct Point
{
	var Vector v;
	var float t;
};

var() const private float Weight;
var() const private float DistanceWeight;

var() const private float Min_NextRecalculationTime;
var() const private float Max_NextRecalculationTime;

var private array<Point> BoidDirection;

function EarlyUpdate(int i)
{
	local Vector Delta;
	local float Magnitude;
	local Vector Force;

	if (BoidDirection.Length != Boids.Length)
	{
		BoidDirection.Length = Boids.Length;
	}

	if (WorldInfo.TimeSeconds >= BoidDirection[i].t)
	{
		BoidDirection[i].v = VRand();
		BoidDirection[i].t = WorldInfo.TimeSeconds + RandRange(Min_NextRecalculationTime, Max_NextRecalculationTime);
	}

	Delta = Location - Boids[i].Location;
	Magnitude = VSize(Delta);

	if (Magnitude <= ControllerRadius && Magnitude > 1.0f)
	{
		Magnitude = (ControllerRadius / Magnitude) * DistanceWeight;
	}
	else
	{
		Magnitude = 1.0f;
	}

	Force = Normal(Delta cross BoidDirection[i].v) * Magnitude * Weight;

	if (Boids[i].Is2D)
	{
		Force.Z = 0.0f;
	}

	Boids[i].AddForce(Force);
}

DefaultProperties
{
	IsEarly = true;

	Weight = 30.0f;
	DistanceWeight = 20.0f;

	Min_NextRecalculationTime = 2.7f;
	Max_NextRecalculationTime = 3.3f;
}
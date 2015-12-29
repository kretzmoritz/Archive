class VBCE_FleeFromPawn extends VeiledBoidControllerExtension;

var() const private float Weight;
var() const private float Radius;

function EarlyUpdate(int i)
{
	local Pawn p;
	local Vector Delta;
	local float Magnitude;

	foreach OverlappingActors(class'Pawn', p, Radius, Boids[i].Location)
	{
		Delta = Boids[i].Location - p.Location;
		Magnitude = VSize(Delta);

		if (Magnitude >= Radius)
		{
			return;
		}
		
		Boids[i].AddForce(Delta * (1.0f - Magnitude / Radius) * Weight);
	}
}

DefaultProperties
{
	IsEarly = true;

	Weight = 20.0f;
	Radius = 500.0f;
}
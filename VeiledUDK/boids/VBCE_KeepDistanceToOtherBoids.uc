class VBCE_KeepDistanceToOtherBoids extends VeiledBoidControllerExtension;

var() const private float Weight;
var() const private float Radius;

function EarlyUpdate(int i)
{
	local Vector Delta;
	local float Magnitude;
	local Vector RepulsionForce;

	local int j;

	for (j = 0; j < i; ++j)
	{
		Delta = Boids[i].Location - Boids[j].Location;
		Magnitude = VSize(Delta);

		if (Magnitude >= Radius)
		{
			return;
		}

		RepulsionForce = Delta * (1.0f - Magnitude / Radius) * Weight;

		if (Boids[i].Is2D)
		{
			RepulsionForce.Z = 0.0f;
		}

		Boids[i].AddForce(RepulsionForce);
	}

	for (j = i + 1; j < Boids.Length; ++j)
	{
		Delta = Boids[i].Location - Boids[j].Location;
		Magnitude = VSize(Delta);

		if (Magnitude >= Radius)
		{
			return;
		}

		RepulsionForce = Delta * (1.0f - Magnitude / Radius) * Weight;

		if (Boids[i].Is2D)
		{
			RepulsionForce.Z = 0.0f;
		}

		Boids[i].AddForce(RepulsionForce);
	}
}

DefaultProperties
{
	IsEarly = true;

	Weight = 20.0f;
	Radius = 500.0f;
}
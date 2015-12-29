class VBCE_AttractionToCenter extends VeiledBoidControllerExtension;

var() const private float Weight;

function EarlyUpdate(int i)
{
	local Vector Delta;

	Delta = (Location - Boids[i].Location) * Weight;

	if (Boids[i].Is2D)
	{
		Delta.Z = 0.0f;
		Boids[i].AddForce(Delta);
	}
	else
	{
		Boids[i].AddForce(Delta);
	}
}

DefaultProperties
{
	IsEarly = true;

	Weight = 0.5f;
}
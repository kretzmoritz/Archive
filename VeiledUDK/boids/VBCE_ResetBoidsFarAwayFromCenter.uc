class VBCE_ResetBoidsFarAwayFromCenter extends VeiledBoidControllerExtension;

var() const private float RemovalDistance;

function EarlyUpdate(int i)
{
	if (VSize(Location - Boids[i].Location) > RemovalDistance)
	{
		Boids[i].SetLocation(Location + VRand() * RandRange(0.0f, ControllerRadius));
	}
}

DefaultProperties
{
	IsEarly = True;

	RemovalDistance = 5000.0f;
}
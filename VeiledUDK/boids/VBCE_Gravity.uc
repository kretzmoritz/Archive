class VBCE_Gravity extends VeiledBoidControllerExtension;

var() const private Vector Gravity;

function EarlyUpdate(int i)
{
	Boids[i].AddAcceleration(Gravity);
}

DefaultProperties
{
	IsEarly = true;
	
	Gravity = (X=0.0f, Y=0.0f, Z=-490.5f);
}
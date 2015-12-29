class VBCE_VelocityCap extends VeiledBoidControllerExtension;

var() const private float Min_Cap;
var() const private float Max_Cap;

function LateUpdate(int i)
{
	local float Magnitude;
	
	Magnitude = VSize(Boids[i].Velocity);
	
	if (Magnitude == 0.0f)
	{
		return;
	}

	if (Magnitude < Min_Cap)
	{
		Boids[i].Velocity = (Boids[i].Velocity / Magnitude) * Min_Cap;
	}
	else if (Magnitude > Max_Cap)
	{
		Boids[i].Velocity = (Boids[i].Velocity / Magnitude) * Max_Cap;
	}
}

DefaultProperties
{
	IsLate = true;

	Min_Cap = 150.0f;
	Max_Cap = 350.0f;
}
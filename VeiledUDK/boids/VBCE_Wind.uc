class VBCE_Wind extends VeiledBoidControllerExtension;

var() const private NxForceFieldGeneric Wind;
var() const private float Weight;
var() const private float SinCurveWeight;

function EarlyUpdate(int i)
{
	Boids[i].AddForce(Vector(Wind.Rotation) * sin(WorldInfo.TimeSeconds * SinCurveWeight)**2 * Weight);
}

DefaultProperties
{
	IsEarly = true;
	
	Weight = 1.0f;
	SinCurveWeight = 1.0f;
}
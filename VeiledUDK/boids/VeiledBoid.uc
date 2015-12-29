class VeiledBoid extends Actor
	dependson(VeiledBoidExtension);

var() const float BoidRadius;
var() const bool Is2D;

var() const private float Min_NextTimerUpdate;
var() const private float Max_NextTimerUpdate;
var private float NextTimerUpdate;

var() privatewrite array<VeiledBoidExtension> Extensions;
var private array<VeiledBoidExtension> TimerExtensions;

var protected bool bNeedsControllerUpdate;

var protected float Mass;
var private Vector Force;
var private Vector Impulse;

function private Initialize()
{
	local int i;

	for (i = 0; i < Extensions.Length; ++i)
	{
		Extensions[i].Constructor();

		if (Extensions[i].IsTimer)
		{
			TimerExtensions.AddItem(Extensions[i]);
		}

		if (!Extensions[i].IsUpdate)
		{
			Extensions.Remove(i, 1);
			--i;
		}
	}

	bNeedsControllerUpdate = true;
	NextTimerUpdate = WorldInfo.TimeSeconds + RandRange(Min_NextTimerUpdate, Max_NextTimerUpdate);
}

function ApplyForceAndAcceleration(float DeltaTime)
{
	Velocity += Force / Mass * DeltaTime;
	Force = vect(0, 0, 0);

	Velocity += Acceleration * DeltaTime;
	Acceleration = vect(0, 0, 0);
}

function Advance(float DeltaTime)
{
	MoveSmooth(Velocity * DeltaTime);
}

function UpdateExtensions(float DeltaTime)
{
	local int i;

	for (i = 0; i < Extensions.Length; ++i)
	{
		Extensions[i].Update(DeltaTime);
	}
}

function private TimerUpdateExtensions()
{
	local int i;

	for (i = 0; i < TimerExtensions.Length; ++i)
	{
		TimerExtensions[i].TimerUpdate();
	}

	NextTimerUpdate = WorldInfo.TimeSeconds + RandRange(Min_NextTimerUpdate, Max_NextTimerUpdate);
}

function bool NeedsControllerUpdate()
{
	return bNeedsControllerUpdate;
}

function float GetMass()
{
	return Mass;
}

function AddForce(Vector _f)
{
	Force += _f;
}

function AddImpulse(Vector _i)
{
	Velocity += _i / Mass;
}

function AddAcceleration(Vector _a)
{
	Acceleration += _a;
}

event PostBeginPlay()
{
	super.PostBeginPlay();

	Initialize();
}

event Tick(float DeltaTime)
{
	super.Tick(DeltaTime);

	if (WorldInfo.TimeSeconds >= NextTimerUpdate)
	{
		TimerUpdateExtensions();
	}
}

DefaultProperties
{
	BoidRadius = 25.0f;
	Is2D = false;

	Min_NextTimerUpdate = 15.0f;
	Max_NextTimerUpdate = 25.0f;
}
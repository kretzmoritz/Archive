class VeiledBoidController extends Actor
	placeable
	dependson(VeiledBoid, VeiledBoidControllerExtension);

var() const private archetype array<VeiledBoid> Archetypes;
var() int BoidCount;
var() const private int MaxActionsPerTick;
var() const float ControllerRadius;

var() const private int MaxUpdatesPerTick;
var private int UpdateIndex;

var protected array<VeiledBoid> Boids;
var privatewrite Vector CenterOfMass;

var() private array<VeiledBoidControllerExtension> Extensions;
var private array<VeiledBoidControllerExtension> EarlyExtensions;
var private array<VeiledBoidControllerExtension> LateExtensions;

function private ForceBoidCountPositive()
{
	if (BoidCount < 0)
	{
		BoidCount = 0;
	}
}

function private KeepPopulationInCheck()
{
	local int Actions;
	local int i;
	local VeiledBoid Boid;

	if (BoidCount == Boids.Length)
	{
		return;
	}

	ForceBoidCountPositive();

	while (BoidCount < Boids.Length)
	{
		Boids[Boids.Length - 1].Destroy();
		Boids.Length = Boids.Length - 1;

		if (Boids.Length > 0 && UpdateIndex == Boids.Length)
		{
			--UpdateIndex;
		}

		++Actions;

		if (Actions >= MaxActionsPerTick)
		{
			return;
		}
	}

	if (Archetypes.Length == 0)
	{
		return;
	}

	while (BoidCount > Boids.Length)
	{
		i = Rand(Archetypes.Length);
		Boid = Spawn(Archetypes[i].Class,,, Location + VRand() * RandRange(0.0f, ControllerRadius),, Archetypes[i], true);

		if (Boid != none)
		{
			Boids.AddItem(Boid);
		}

		++Actions;

		if (Actions >= MaxActionsPerTick)
		{
			return;
		}
	}
}

function private Initialize()
{
	local int i;

	for (i = 0; i < Extensions.Length; ++i)
	{
		Extensions[i].Constructor();

		if (Extensions[i].IsEarly)
		{
			EarlyExtensions.AddItem(Extensions[i]);
		}

		if (Extensions[i].IsLate)
		{
			LateExtensions.AddItem(Extensions[i]);
		}
	}

	Extensions.Length = 0;
}

function private Update(float DeltaTime)
{
	local int Updates;
	local int i;

	while (Updates < MaxUpdatesPerTick)
	{
		if (Boids[UpdateIndex].NeedsControllerUpdate())
		{
			for (i = 0; i < EarlyExtensions.Length; ++i)
			{
				EarlyExtensions[i].EarlyUpdate(UpdateIndex);
			}

			Boids[UpdateIndex].ApplyForceAndAcceleration(DeltaTime);

			for (i = 0; i < LateExtensions.Length; ++i)
			{
				LateExtensions[i].LateUpdate(UpdateIndex);
			}
		}

		++Updates;
		++UpdateIndex;

		if (UpdateIndex == Boids.Length)
		{
			UpdateIndex = 0;
		}
	}

	CenterOfMass = vect(0, 0, 0);

	for (i = 0; i < Boids.Length; ++i)
	{
		if (Boids[i].NeedsControllerUpdate())
		{
			Boids[i].UpdateExtensions(DeltaTime);
			Boids[i].Advance(DeltaTime);
		}

		CenterOfMass += Boids[i].Location;
	}

	CenterOfMass /= Boids.Length;
}

event PostBeginPlay()
{
	super.PostBeginPlay();

	Initialize();
}

event Tick(float DeltaTime)
{
	super.Tick(DeltaTime);

	KeepPopulationInCheck();

	if (Boids.Length > 0)
	{
		Update(DeltaTime);
	}
}

DefaultProperties
{
	Begin Object Class=SpriteComponent Name=Sprite
		HiddenGame = true
	End Object

	Components.Add(Sprite);
	
	BoidCount = 50;
	MaxActionsPerTick = 1;
	ControllerRadius = 1000.0f;

	MaxUpdatesPerTick = 50;
}

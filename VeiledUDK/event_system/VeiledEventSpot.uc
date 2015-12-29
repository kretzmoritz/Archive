class VeiledEventSpot extends Actor
	dependson(VeiledTriggerVolume, VeiledPlayerCanSeeMe) placeable;

var() private SpriteComponent EditorPreview;

var() private name EventName;
var() private VeiledTriggerVolume EventTrigger;
var() private float ForceLoad_Distance;
var() private float AllowLoad_Distance;

var() private array<Actor> SightCheckObjs;
var private array<VeiledPlayerCanSeeMe> PlayerCanSeeMe;

var private bool IsForceLoading;
var private Vector PositionToTurnAwayFrom;

var int EventRef;
var int RunningRef;

/**
 * Initialize PlayerCanSeeMe
 * Set the callback to our TriggerVolume
 */

event PreBeginPlay()
{
	local int i;

	for (i = 0; i < SightCheckObjs.Length; ++i)
	{
		if (SightCheckObjs[i] != none)
		{
			PlayerCanSeeMe.AddItem(new class'VeiledPlayerCanSeeMe');
			PlayerCanSeeMe[PlayerCanSeeMe.Length - 1].obj = SightCheckObjs[i];
			PlayerCanSeeMe[PlayerCanSeeMe.Length - 1].timeObjNeedsToBeInView = 1.0f;
		}
	}

	EventTrigger.cbTouch = cbTouch;
}

/**
 * Resolve all event references pre-runtime
 */

event PostBeginPlay()
{
	local int i;

	for (i = 0; i < `EventManager.RegisteredEvents.Length; ++i)
	{
		if (EventName == `EventManager.RegisteredEvents[i].EventName)
		{
			EventRef = i;
			break;
		}
	}

	if (EventRef == `NULL_REF)
	{
		EventTrigger.SetCollisionType(COLLIDE_NoCollision);
	}
}

event Tick(float DeltaTime)
{
	local array<byte> pcsm_results;

	if (EventTrigger.CollisionType == COLLIDE_NoCollision && RunningRef == `NULL_REF) //This event spot is disabled - discard...
	{
		return;
	}

	CheckSight(pcsm_results);

	if (EventTrigger.CollisionType != COLLIDE_NoCollision)
	{
		if (`EventManager.RegisteredEvents[EventRef].BlockOnSight) //Is the event big enough that our player could notice?
		{
			if (CheckSight(pcsm_results)) //Our player has seen the event spot - we can't spawn anything here now!
			{
				`VLOG("SightCheckObj seen...");
				EventTrigger.SetCollisionType(COLLIDE_NoCollision);

				return;
			}
		}

		if (`EventManager.RegisteredEvents[EventRef].TimeUntilAutomaticExecution >= 0.0f && 
			WorldInfo.TimeSeconds >= `EventManager.RegisteredEvents[EventRef].TimeUntilAutomaticExecution && !CheckSight(pcsm_results)) //Run the event automatically after a certain timeframe
		{
			LoadEvent();
		}
	}
	else if (RunningRef != `NULL_REF)
	{
		if (CheckSight(pcsm_results))
		{
			if (`EventManager.RegisteredEvents[EventRef].ActiveSpot == none && !IsForceLoading && 
				VSize(Location - WorldInfo.GetALocalPlayerController().Pawn.Location) <= ForceLoad_Distance) //Panic! Our player could notice the missing event...
			{
				ForceLoad();
			}

			return;
		}

		if (`EventManager.RegisteredEvents[EventRef].ActiveSpot == self)
		{
			if (VSize(Location - WorldInfo.GetALocalPlayerController().Pawn.Location) > AllowLoad_Distance) //We can safely remove the event
			{
				`EventManager.RequestRemoval(self);
			}
		}
		else if (`EventManager.RegisteredEvents[EventRef].ActiveSpot == none)
		{
			if (VSize(Location - WorldInfo.GetALocalPlayerController().Pawn.Location) <= AllowLoad_Distance) //We can safely load the event
			{
				LoadEvent();
			}
		}
	}
}

/**
 * Trials PlayerCanSeeMe if it hasn't happened yet this frame
 * Otherwise checks if any SightCheckObj is visible (true or false)
 */

function private bool CheckSight(out array<byte> _pcsm_results)
{
	local int i;

	if (_pcsm_results.Length == 0 && PlayerCanSeeMe.Length > 0)
	{
		_pcsm_results.Length = PlayerCanSeeMe.Length;

		for (i = 0; i < PlayerCanSeeMe.Length; ++i)
		{
			_pcsm_results[i] = byte(PlayerCanSeeMe[i].Trial());
		}

		return true;
	}
	else
	{
		for (i = 0; i < _pcsm_results.Length; ++i)
		{
			if (_pcsm_results[i] != 0)
			{
				PositionToTurnAwayFrom = PlayerCanSeeMe[i].obj.Location;
				return true;
			}
		}

		return false;
	}
}

/**
 * Touch callback function
 * Loads our event for the first time if we reach the TriggerVolume
 */

event private cbTouch(Actor Other, PrimitiveComponent OtherComp, Vector HitLocation, Vector HitNormal)
{
	if (VeiledPlayerPawn(Other) != none || (`EventManager.RegisteredEvents[EventRef].CanBeTriggeredByAI && VeiledPawn(Other) != none))
	{
		`VLOG("Touch");

		LoadEvent();
	}
}

/**
 * Requests the event from the EventManager; then...
 * Disables the TriggerVolume
 * Disables automatic execution of our event (if needed)
 * Makes sure the area is clean and ready for our event to spawn
 */

function private LoadEvent()
{
	if (`EventManager.RequestEvent(self))
	{
		EventTrigger.SetCollisionType(COLLIDE_NoCollision);
		`EventManager.RegisteredEvents[EventRef].TimeUntilAutomaticExecution = `NULL_REF;
	}
}

/**
 * If our player is really close to the event we need to make sure it's ready once he reaches the location
 * What do we do? We turn his head and FORCE SPAWN it...
 */

function private ForceLoad()
{
	VeiledPlayerPawn(WorldInfo.GetALocalPlayerController().Pawn).RequestTurnAway(PositionToTurnAwayFrom, cbTurnAwayFinished);
	IsForceLoading = true;
}

function private cbTurnAwayFinished()
{
	IsForceLoading = false;
}

DefaultProperties
{
	Begin Object Class=SpriteComponent Name=Sprite
		HiddenGame = true
	End Object

	EditorPreview = Sprite;
	Components.Add(Sprite)
	
	ForceLoad_Distance = 750.0f;
	AllowLoad_Distance = 1000.0f;

	EventRef = `NULL_REF;
	RunningRef = `NULL_REF;

	IsForceLoading = false;
}
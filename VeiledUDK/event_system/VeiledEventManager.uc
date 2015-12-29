class VeiledEventManager extends Actor
	dependson(VeiledEvent_Container, VeiledEventControl);

var() array<VeiledEvent> RegisteredEvents;

var private array<string> PreBeginErrorLog;

/**
 * Resolve all event references pre-runtime
 */

event PreBeginPlay()
{
	local name PackageName;

	local int i;
	local int j;
	local int k;

	////////////////////////////////////////////////////////////////////
	// Do all packages exist?
	////////////////////////////////////////////////////////////////////

	for (i = 0; i < RegisteredEvents.Length; ++i)
	{
		PackageName = class'WorldInfo'.static.IsPlayInEditor() ? name("UEDPIE"$RegisteredEvents[i].EventName) : RegisteredEvents[i].EventName;

		for (j = 0; j < WorldInfo.StreamingLevels.Length; ++j)
		{
			if (PackageName == WorldInfo.StreamingLevels[j].PackageName)
			{
				RegisteredEvents[i].PackageRef = j;
				break;
			}
		}

		if (RegisteredEvents[i].PackageRef == `NULL_REF)
		{
			PreBeginErrorLog.AddItem(RegisteredEvents[i].EventName$": WARNING! Event not found.");

			RegisteredEvents.Remove(i, 1);
			--i;
		}
	}

	////////////////////////////////////////////////////////////////////
	// Do all required and blocking events exist?
	////////////////////////////////////////////////////////////////////

	for (i = 0; i < RegisteredEvents.Length; ++i)
	{
		for (j = 0; j < RegisteredEvents[i].RequiredEvents.Length; ++j)
		{
			for (k = 0; k < RegisteredEvents.Length; ++k)
			{
				if (i != k && RegisteredEvents[i].RequiredEvents[j].EventName == RegisteredEvents[k].EventName)
				{
					RegisteredEvents[i].RequiredEvents[j].EventRef = k;
					break;
				}
			}

			if (RegisteredEvents[i].RequiredEvents[j].EventRef == `NULL_REF)
			{
				PreBeginErrorLog.AddItem(RegisteredEvents[i].EventName$": WARNING! RequiredEvent "$RegisteredEvents[i].RequiredEvents[j].EventName$" not found.");

				RegisteredEvents[i].RequiredEvents.Remove(j, 1);
				--j;
			}
		}

		for (j = 0; j < RegisteredEvents[i].BlockingEvents.Length; ++j)
		{
			for (k = 0; k < RegisteredEvents.Length; ++k)
			{
				if (i != k && RegisteredEvents[i].BlockingEvents[j].EventName == RegisteredEvents[k].EventName)
				{
					RegisteredEvents[i].BlockingEvents[j].EventRef = k;
					break;
				}
			}

			if (RegisteredEvents[i].BlockingEvents[j].EventRef == `NULL_REF)
			{
				PreBeginErrorLog.AddItem(RegisteredEvents[i].EventName$": WARNING! BlockingEvent "$RegisteredEvents[i].BlockingEvents[j].EventName$" not found.");

				RegisteredEvents[i].BlockingEvents.Remove(j, 1);
				--j;
			}
		}
	}
}

/**
 * Output all error messages now that the HUD has loaded
 * StartAutomaticExecutionTimer() should later be called once the player leaves the office (for now leave it here)
 */

event PostBeginPlay()
{
	SetTimer(2.5f,, 'OutputErrorLog');

	StartAutomaticExecutionTimer();
}

event Tick(float DeltaTime)
{
	UpdateState();
}

function private OutputErrorLog()
{
	local int i;

	for (i = 0; i < PreBeginErrorLog.Length; ++i)
	{
		`VLOG(PreBeginErrorLog[i]);
	}

	PreBeginErrorLog.Length = 0;
}

/**
 * Start the automatic execution timer for all events
 */

function StartAutomaticExecutionTimer()
{
	local int i;

	for (i = 0; i < RegisteredEvents.Length; ++i)
	{
		if (RegisteredEvents[i].TimeUntilAutomaticExecution >= 0.0f)
		{
			RegisteredEvents[i].TimeUntilAutomaticExecution = WorldInfo.TimeSeconds + RegisteredEvents[i].TimeUntilAutomaticExecution;
		}
	}
}

/**
 * Update all event instances (switch between states)
 * Activate the corresponding kismet outputs
 */

function private UpdateState()
{
	local array<SequenceObject> SeqObjs;

	local int i;
	local int j;

	WorldInfo.GetGameSequence().FindSeqObjectsByClass(class'VeiledEventControl', true, SeqObjs);
	
	for (i = 0; i < RegisteredEvents.Length; ++i)
	{
		if (RegisteredEvents[i].ActiveSpot != none)
		{
			j = RegisteredEvents[i].ActiveSpot.RunningRef;

			if (RegisteredEvents[i].RunningEvents[j] != ES_RUNNING && RegisteredEvents[i].RunningEvents[j] != ES_MAX)
			{
				if (FireKismetEvent(i, SeqObjs))
				{
					RegisteredEvents[i].RunningEvents[j] = EventState(RegisteredEvents[i].RunningEvents[j] + 1);
				}
			}
		}
	}
}

/**
 * If we have an EventControl node placed within the correct package this function notifies it of any pending state changes
 */

function private bool FireKismetEvent(int _eventRef, array<SequenceObject> _seqObjs)
{
	local array<int> OutputToActivate;
	local bool fired;
	
	local int i;
	local int j;
	local int k;

	fired = false;

	for (i = 0; i < _seqObjs.Length; ++i)
	{
		j = RegisteredEvents[_eventRef].PackageRef;

		if (_seqObjs[i].GetPackageName() == WorldInfo.StreamingLevels[j].PackageName)
		{
			k = RegisteredEvents[_eventRef].ActiveSpot.RunningRef;

			OutputToActivate.Length = 1;
			OutputToActivate[0] = RegisteredEvents[_eventRef].RunningEvents[k];
			
			VeiledEventControl(_seqObjs[i]).CheckActivate(WorldInfo, none,, OutputToActivate);

			fired = true;
		}
	}

	return fired;
}

/**
 * All logic used in requesting an event!
 * Is an event of this type already active?;
 * Can it spawn more than once? Was there already one running previously?
 * Did all the required events complete?
 * Did none of the blocking events complete? 
 * -> STREAM IT IN
 */

event bool RequestEvent(VeiledEventSpot _eventSpot)
{
	local int i;
	local int j;
	local int k;

	i = _eventSpot.EventRef;

	if (RegisteredEvents[i].ActiveSpot != none)
	{
		return false;
	}

	if (_eventSpot.RunningRef == `NULL_REF)
	{
		if (!RegisteredEvents[i].CanSpawnMultipleTimes && RegisteredEvents[i].RunningEvents.Length > 0)
		{
			return false;
		}

		for (j = 0; j < RegisteredEvents[i].RequiredEvents.Length; ++j)
		{
			if (RegisteredEvents[RegisteredEvents[i].RequiredEvents[j].EventRef].RunningEvents.Length < RegisteredEvents[i].RequiredEvents[j].Count)
			{
				return false;
			}
		}
			
		for (j = 0; j < RegisteredEvents[i].BlockingEvents.Length; ++k)
		{
			if (RegisteredEvents[RegisteredEvents[i].BlockingEvents[j].EventRef].RunningEvents.Length >= RegisteredEvents[i].RequiredEvents[j].Count)
			{
				return false;
			}
		}

		RegisteredEvents[i].RunningEvents.AddItem(ES_SPAWNED);
		_eventSpot.RunningRef = RegisteredEvents[i].RunningEvents.Length - 1;
	}

	PositionAndWakeMoveables(RegisteredEvents[i].AffectedArea, _eventSpot);

	RegisteredEvents[i].ActiveSpot = _eventSpot;
	k = RegisteredEvents[i].PackageRef;

	WorldInfo.StreamingLevels[k].OldOffset = vect(0, 0, 0);
	WorldInfo.StreamingLevels[k].Offset = _eventSpot.Location;

	WorldInfo.GetALocalPlayerController().ClientUpdateLevelStreamingStatus(WorldInfo.StreamingLevels[k].PackageName, true, true, false);

	return true;
}

/**
 * All logic used in requesting removal!
 * Is the requesting spot currently active?
 * Did the event complete?
 * -> STREAM IT OUT
 */

event RequestRemoval(VeiledEventSpot _eventSpot)
{
	local int i;
	local int j;
	local int k;

	i = _eventSpot.EventRef;

	if (RegisteredEvents[i].ActiveSpot == _eventSpot)
	{
		j = _eventSpot.RunningRef;

		if (RegisteredEvents[i].RunningEvents[j] == ES_MAX)
		{
			k = RegisteredEvents[i].PackageRef;

			WorldInfo.GetALocalPlayerController().ClientUpdateLevelStreamingStatus(WorldInfo.StreamingLevels[k].PackageName, false, false, false);
			WorldInfo.GetALocalPlayerController().ClientFlushLevelStreaming();

			RegisteredEvents[i].RunningEvents[j] = ES_RESULT;
			RegisteredEvents[i].ActiveSpot = none;
		}
	}
}

/**
 * Moves all actors currently in the area to a previously specified location
 * Wakes their rigidbodies so they physics system will dynamically reposition them
 */

function private PositionAndWakeMoveables(float _affectedArea, VeiledEventSpot _eventSpot)
{
	local Actor a;
	local bool NoEncroachCheck;

	foreach _eventSpot.OverlappingActors(class'Actor', a, _affectedArea)
	{
		if (KActor(a) != none)
		{
			KActor(a).StaticMeshComponent.SetRBPosition(_eventSpot.Location);
			KActor(a).StaticMeshComponent.WakeRigidBody();
		}
		else if (KAsset(a) != none)
		{
			KAsset(a).SkeletalMeshComponent.SetRBPosition(_eventSpot.Location);
			KAsset(a).SkeletalMeshComponent.WakeRigidBody();
		}
		else if (VeiledPawn(a) != none && VeiledPlayerPawn(a) == none)
		{
			NoEncroachCheck = a.bNoEncroachCheck;
			a.bNoEncroachCheck = true;
			a.SetLocation(_eventSpot.Location);
			a.bNoEncroachCheck = NoEncroachCheck;
		}
	}
}

DefaultProperties
{

}
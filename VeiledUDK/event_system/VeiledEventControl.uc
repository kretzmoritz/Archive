class VeiledEventControl extends VeiledSequenceEvent;

var private int EventRef;
var private Vector Rotation;

/**
 * Resolve all event references pre-runtime
 */

event Constructor()
{
	local int i;
	local int j;

	for (i = 0; i < `EventManager.RegisteredEvents.Length; ++i)
	{
		j = `EventManager.RegisteredEvents[i].PackageRef;

		if (GetPackageName() == `WorldInfo.StreamingLevels[j].PackageName)
		{
			EventRef = i;
			break;
		}
	}
}

/**
 * 0 = Sync
 * 1 = Query
 * 2 = SetFinished
 */

event Activated()
{
	if (InputLinks[0].bHasImpulse)
	{
		`EventManager.RegisteredEvents[EventRef].ActiveSpot.SetRotation(`VRot(Rotation.X, Rotation.Y, Rotation.Z, `DegreeToUnreal));
	}
	else if (InputLinks[1].bHasImpulse)
	{
		Rotation = `VVec(   `EventManager.RegisteredEvents[EventRef].ActiveSpot.Rotation.Pitch, 
							`EventManager.RegisteredEvents[EventRef].ActiveSpot.Rotation.Yaw, 
							`EventManager.RegisteredEvents[EventRef].ActiveSpot.Rotation.Roll, 
							`UnrealToDegree);
	}
	else if (InputLinks[2].bHasImpulse)
	{
		SetFinished();
	}
}

/**
 * Tells the EventManager to mark this event instance as ES_FINISHED
 */

function SetFinished()
{
	local int i;
	
	i = `EventManager.RegisteredEvents[EventRef].ActiveSpot.RunningRef;
	
	if (`EventManager.RegisteredEvents[EventRef].RunningEvents[i] < ES_FINISHED)
	{
		`EventManager.RegisteredEvents[EventRef].RunningEvents[i] = ES_FINISHED;
	}
}

defaultproperties
{
	ObjName="EventControl"
	ObjCategory="VeiledSystem"
	
	InputLinks.Empty
	InputLinks[0] = (LinkDesc="Synch")
	InputLinks[1] = (LinkDesc="Query")
	InputLinks[2] = (LinkDesc="SetFinished")

	OutputLinks.Empty
	OutputLinks[ES_SPAWNED] = (LinkDesc="Spawned")
	OutputLinks[ES_RUNNING] = (bHidden=true)
	OutputLinks[ES_FINISHED] = (LinkDesc="Finished")
	OutputLinks[ES_RESULT] = (LinkDesc="Result")

	VariableLinks.Empty
	VariableLinks[0] = (ExpectedType=class'SeqVar_Vector', LinkDesc="Rotation", PropertyName=Rotation)

	bPlayerOnly=false
	MaxTriggerCount=0
}
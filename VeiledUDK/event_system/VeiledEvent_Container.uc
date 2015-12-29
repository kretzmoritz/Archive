class VeiledEvent_Container extends Object
	dependson(VeiledEventSpot);

enum EventState
{
	ES_SPAWNED,
	ES_RUNNING,
	ES_FINISHED,
	ES_RESULT
};

struct VeiledEvent
{
	struct EventDependance
	{
		var() name EventName;
		var() int Count;
	
		var int EventRef;

		StructDefaultProperties
		{
			Count = 1
			EventRef = `NULL_REF
		}
	};

	var() name EventName;
	var() bool BlockOnSight;
	var() bool CanSpawnMultipleTimes;
	var() bool CanBeTriggeredByAI;
	var() float AffectedArea;
	var() float TimeUntilAutomaticExecution;
	var() array<EventDependance> RequiredEvents;
	var() array<EventDependance> BlockingEvents;

	var int PackageRef;
	
	var VeiledEventSpot ActiveSpot;
	var array<EventState> RunningEvents;

	StructDefaultProperties
	{
		AffectedArea = 500.0f
		TimeUntilAutomaticExecution = `NULL_REF
		PackageRef = `NULL_REF
		ActiveSpot = none
	}
};
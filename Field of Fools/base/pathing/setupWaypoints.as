package base.pathing
{
	import flash.geom.Point;
	import flash.display.MovieClip;
	import base.pathing.pathfinding;

	public class setupWaypoints extends MovieClip
	{
		private var cPathfindingHandler : pathfinding;
		private var vLaneCount:Vector.<int> = new Vector.<int>();
		private static var vWaypoints:Vector.< Vector.<MovieClip> > = new Vector.< Vector.<MovieClip> >();
		
		public function setupWaypoints() : void
		{
			cPathfindingHandler = new pathfinding();
		}
		
		public function calculate(_input:Vector.<MovieClip>) : void
		{
			for(var i:int = 0; i < _input.length; i++) // Count number of lanes
			{
				if(vLaneCount.indexOf(_input[i].lane) < 0)
				{
					vLaneCount.push(_input[i].lane);
				}
			}
			
			for(i = 0; i < vLaneCount.length; i++) // Resize vector to correct lane count
			{
				vWaypoints.push(new Vector.<MovieClip>());
			}
			
			vLaneCount.splice(0, vLaneCount.length);
			
			for(i = 0; i < _input.length; i++) // Add waypoints unsorted
			{
				vWaypoints[_input[i].lane].push(_input[i]);
				addChild(vWaypoints[_input[i].lane][vWaypoints[_input[i].lane].length - 1])
			}
			
			// Sort waypoints
			cPathfindingHandler.sort(vWaypoints);
		}
		
		public static function getWaypoints() : Vector.< Vector.<MovieClip> >
		{
			return vWaypoints;
		}
	}
}
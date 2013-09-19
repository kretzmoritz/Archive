package base.ui
{
	import base.pathing.setupWaypoints;
	
	public class lane_choice
	{
		public static var iCurrentLane:int = 0;
		
		public function decide(_xMouse:int, _yMouse:int)
		{
			for(var i:int = 0; i < setupWaypoints.getWaypoints().length; i++)
			{
				var fDistance:Number = Math.sqrt(Math.pow(((setupWaypoints.getWaypoints()[i][0].x - 75.0) - _xMouse), 2) + Math.pow((setupWaypoints.getWaypoints()[i][0].y - _yMouse), 2));
				
				if(fDistance < 25.0)
				{
					iCurrentLane = i;
					break;
				}
			}
		}
	}
}
package base.pathing
{
	import flash.net.URLLoader;
	import flash.net.URLRequest;
	import flash.events.Event;
	import flash.display.MovieClip;
	import flash.utils.getDefinitionByName;
	import base.pathing.setupWaypoints;
	
	public class setupLane extends MovieClip
	{
		private var iLaneDifficulty = 1;
		private var cWaypointHandler : setupWaypoints;
		private var vLanes:Vector.<MovieClip> = new Vector.<MovieClip>();
		
		public function setupLane() : void
		{
			cWaypointHandler = new setupWaypoints();
			addChild(cWaypointHandler); // Functions as container for tiles
			
			var xmlLoader:URLLoader = new URLLoader();
			xmlLoader.addEventListener(Event.COMPLETE, onLoaded, false, 0, true);
			
			switch(iLaneDifficulty)
			{
				case 1:
					xmlLoader.load(new URLRequest("data/info/lanes1.xml")); // 1 lane
					break;
				case 2:
					xmlLoader.load(new URLRequest("data/info/lanes2.xml")); // 2 lanes
					break;
				case 3:
					xmlLoader.load(new URLRequest("data/info/lanes3.xml")); // 3 or more lanes
					break;
			}
		}
		
		private function onLoaded(e:Event) : void
		{
			var laneData:XML = new XML(e.target.data);
			XML.ignoreWhitespace = true;
			
			var iLow:int = 0;
			var iHigh:int = laneData.level.length() - 1;
			var iRandomLevel:int = Math.floor(Math.random() * (1 + iHigh - iLow)) + iLow;
			
			for(var i:int = 0; i < laneData.level[iRandomLevel].tile.length(); i++)
			{
				// ─ horizontal
				// │ vertical
				// ┘ left_top
				// ┌ bottom_right
				// ┐ left_bottom
				// └ top_right
				
				vLanes.push(new (getDefinitionByName(laneData.level[iRandomLevel].tile[i].id.text()) as Class)());
				vLanes[vLanes.length - 1].lane = int(laneData.level[iRandomLevel].tile[i].lane.text());
				vLanes[vLanes.length - 1].x = 150.0 + 37.5 + int(laneData.level[iRandomLevel].tile[i].posX.text()) * 75.0; // Grid style system, tile width = 75.0, offset = 150.0
				vLanes[vLanes.length - 1].y = 37.5 + int(laneData.level[iRandomLevel].tile[i].posY.text()) * 75.0; // Grid style system, tile height = 75.0
				vLanes[vLanes.length - 1].path = int(laneData.level[iRandomLevel].tile[i].path.text());
			}
			
			cWaypointHandler.calculate(vLanes);
			vLanes.splice(0, vLanes.length);
		}
	}
}
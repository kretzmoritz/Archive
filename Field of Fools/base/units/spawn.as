package base.units
{
	import flash.net.URLLoader;
	import flash.net.URLRequest;
	import flash.events.Event;
	import flash.utils.Timer;
	import flash.events.TimerEvent;
	import flash.display.MovieClip;
	import base.pathing.setupWaypoints;
	
	public class spawn extends MovieClip
	{
		private var timer:Timer;
		private static var spawn_singleton : spawn;
		private static var bHeaven:Boolean = true;
		private static var vUnitsHeaven:Vector.<unit> = new Vector.<unit>();
		private static var vUnitsHell:Vector.<unit> = new Vector.<unit>();
		
		public function spawn() : void
		{
			spawn_singleton = this;
			
			var xmlLoader:URLLoader = new URLLoader();
			xmlLoader.addEventListener(Event.COMPLETE, onLoaded, false, 0, true);
			xmlLoader.load(new URLRequest("data/info/cf_timer.xml"));
		}
		
		private function onLoaded(e:Event) : void
		{
			var cfSpawnData:XML = new XML(e.target.data);
			XML.ignoreWhitespace = true;
			
			// Disabled: No cannonfodder spawn
			timer = new Timer(cfSpawnData.tick * 1000);
			timer.addEventListener(TimerEvent.TIMER, onTick, false, 0, true);
			timer.start();
		}
		
		private function onTick(e:Event) : void
		{
			for(var i:int = 0; i < setupWaypoints.getWaypoints().length; i++)
			{
				//createUnit(unit_cf_heaven, 1, i);
				//createUnit(unit_cf_hell, 1, i);
				createUnit(unit_charger_heaven, 1, i);
				createUnit(unit_charger_hell, 1, i);
			}
		}
		
		public function createUnit(_class:Class, _upgrade:int, _lane:int) : void
		{
			switch(bHeaven)
			{
				case false:
					if(_class == unit_charger_heaven || _class == unit_melee_heaven || _class == unit_range_heaven) // Cannonfodder disabled
					{
						vUnitsHeaven.push(new _class());
						vUnitsHeaven[vUnitsHeaven.length - 1].init(_upgrade);
						vUnitsHeaven[vUnitsHeaven.length - 1].x = setupWaypoints.getWaypoints()[_lane][setupWaypoints.getWaypoints()[_lane].length - 1].x + 75.0; // Tile width: 75.0
						vUnitsHeaven[vUnitsHeaven.length - 1].y = setupWaypoints.getWaypoints()[_lane][setupWaypoints.getWaypoints()[_lane].length - 1].y;
						vUnitsHeaven[vUnitsHeaven.length - 1].iLane = _lane;
						vUnitsHeaven[vUnitsHeaven.length - 1].iWaypoint = setupWaypoints.getWaypoints()[_lane].length - 1;
						vUnitsHeaven[vUnitsHeaven.length - 1].bPositiveDirection = false;
						vUnitsHeaven[vUnitsHeaven.length - 1].bAxisX = true;
						vUnitsHeaven[vUnitsHeaven.length - 1].scaleX = 1.0;
						vUnitsHeaven[vUnitsHeaven.length - 1].scaleY = 1.0;
						addChildAt(vUnitsHeaven[vUnitsHeaven.length - 1], 0);
						addChildAt(vUnitsHeaven[vUnitsHeaven.length - 1].bSprite, 0);
					}
					else
					{
						vUnitsHell.push(new _class());
						vUnitsHell[vUnitsHell.length - 1].init(_upgrade);
						vUnitsHell[vUnitsHell.length - 1].x = setupWaypoints.getWaypoints()[_lane][0].x - 75.0; // Tile width: 75.0
						vUnitsHell[vUnitsHell.length - 1].y = setupWaypoints.getWaypoints()[_lane][0].y;
						vUnitsHell[vUnitsHell.length - 1].iLane = _lane;
						vUnitsHell[vUnitsHell.length - 1].iWaypoint = 0;
						vUnitsHell[vUnitsHell.length - 1].bPositiveDirection = true;
						vUnitsHell[vUnitsHell.length - 1].bAxisX = true;
						vUnitsHell[vUnitsHell.length - 1].scaleX = -1.0;
						vUnitsHell[vUnitsHell.length - 1].scaleY = 1.0;
						addChildAt(vUnitsHell[vUnitsHell.length - 1], 0);
						addChildAt(vUnitsHell[vUnitsHell.length - 1].bSprite, 0);
					}
					break;
				case true:
					if(_class == unit_charger_heaven || _class == unit_melee_heaven || _class == unit_range_heaven)
					{
						vUnitsHeaven.push(new _class());
						vUnitsHeaven[vUnitsHeaven.length - 1].init(_upgrade);
						vUnitsHeaven[vUnitsHeaven.length - 1].x = setupWaypoints.getWaypoints()[_lane][0].x - 75.0; // Tile width: 75.0
						vUnitsHeaven[vUnitsHeaven.length - 1].y = setupWaypoints.getWaypoints()[_lane][0].y;
						vUnitsHeaven[vUnitsHeaven.length - 1].iLane = _lane;
						vUnitsHeaven[vUnitsHeaven.length - 1].iWaypoint = 0;
						vUnitsHeaven[vUnitsHeaven.length - 1].bPositiveDirection = true;
						vUnitsHeaven[vUnitsHeaven.length - 1].bAxisX = true;
						vUnitsHeaven[vUnitsHeaven.length - 1].scaleX = -1.0;
						vUnitsHeaven[vUnitsHeaven.length - 1].scaleY = 1.0;
						addChildAt(vUnitsHeaven[vUnitsHeaven.length - 1], 0);
						addChildAt(vUnitsHeaven[vUnitsHeaven.length - 1].bSprite, 0);
					}
					else
					{
						vUnitsHell.push(new _class());
						vUnitsHell[vUnitsHell.length - 1].init(_upgrade);
						vUnitsHell[vUnitsHell.length - 1].x = setupWaypoints.getWaypoints()[_lane][setupWaypoints.getWaypoints()[_lane].length - 1].x + 75.0; // Tile width: 75.0
						vUnitsHell[vUnitsHell.length - 1].y = setupWaypoints.getWaypoints()[_lane][setupWaypoints.getWaypoints()[_lane].length - 1].y;
						vUnitsHell[vUnitsHell.length - 1].iLane = _lane;
						vUnitsHell[vUnitsHell.length - 1].iWaypoint = setupWaypoints.getWaypoints()[_lane].length - 1;
						vUnitsHell[vUnitsHell.length - 1].bPositiveDirection = false;
						vUnitsHell[vUnitsHell.length - 1].bAxisX = true;
						vUnitsHell[vUnitsHell.length - 1].scaleX = 1.0;
						vUnitsHell[vUnitsHell.length - 1].scaleY = 1.0;
						addChildAt(vUnitsHell[vUnitsHell.length - 1], 0);
						addChildAt(vUnitsHell[vUnitsHell.length - 1].bSprite, 0);
					}
					break;
			}
		}
		
		public static function isPlayingHeaven() : Boolean
		{
			return bHeaven;
		}
		
		public static function getHeaven() : Vector.<unit>
		{
			return vUnitsHeaven;
		}
		
		public static function getHell() : Vector.<unit>
		{
			return vUnitsHell;
		}
		
		public static function getSingleton() : spawn
		{
			return spawn_singleton;
		}
	}
}

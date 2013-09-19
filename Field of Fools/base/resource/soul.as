package base.resource
{
	import flash.events.Event;
	import flash.utils.Timer;
	import flash.events.TimerEvent;
	import flash.net.URLLoader;
	import flash.net.URLRequest;
	
	public class soul
	{
		private static var timer:Timer;
		private static var iUpgradeStatus:int = 1;
		public static var iSoulCount:int = 0;
		
		private static var bLoadFinished:Boolean = false;
		private static var xmlLoader:URLLoader = new URLLoader();
		private static var soulData:XML;
		
		public static function init()
		{
			xmlLoader.load(new URLRequest("data/info/resource.xml"));
			xmlLoader.addEventListener(Event.COMPLETE, onLoaded, false, 0, true);
			
			timer = new Timer(2000);
			timer.start();
			
			timer.addEventListener(TimerEvent.TIMER, add, false, 0, true);
		}
		
		private static function onLoaded(e:Event) : void
		{
			soulData = new XML(e.target.data);
			XML.ignoreWhitespace = true;
			
			bLoadFinished = true;
		}
		
		public static function upgrade() : void
		{
			if(iUpgradeStatus < 3)
			{
				iUpgradeStatus++;
			}
		}
		
		private static function add(e:TimerEvent) : void
		{
			if(bLoadFinished)
			{
				switch(iUpgradeStatus)
				{
					case 1:
						iSoulCount += int(soulData.soul_upgrade1);
						break;
					case 2:
						iSoulCount += int(soulData.soul_upgrade2);
						break;
					case 3:
						iSoulCount += int(soulData.soul_upgrade3);
						break;
				}
			}
		}
	}
}
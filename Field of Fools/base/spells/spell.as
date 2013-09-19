package base.spells
{
	import flash.events.Event;
	import flash.net.URLLoader;
	import flash.net.URLRequest;
	import base.resource.essence;
	import base.resource.soul;
	import base.units.spawn;

	public class spell
	{
		private static var bLoadFinished:Boolean = false;
		private static var xmlLoader:URLLoader = new URLLoader();
		private static var spellData:XML;
		
		public static function init() : void
		{
			xmlLoader.load(new URLRequest("data/info/spell.xml"));
			xmlLoader.addEventListener(Event.COMPLETE, onLoaded, false, 0, true);
		}
		
		private static function onLoaded(e:Event) : void
		{
			spellData = new XML(e.target.data);
			XML.ignoreWhitespace = true;
			
			bLoadFinished = true;
		}
		
		public static function exchangeEssencesForSouls() : void
		{
			if(bLoadFinished)
			{
				essence.iEssenceCount -= int(spellData.resource[0].essence);
				soul.iSoulCount += int(spellData.resource[0].soul);
			}
		}
		
		public static function damageEnemyInRadius(_x:int, _y:int) : void
		{
			if(bLoadFinished)
			{
				var fDistance:Number;
				
				if(spawn.isPlayingHeaven())
				{
					for(var i:int = 0; i < spawn.getHell().length; i++)
					{
						fDistance = Math.sqrt(Math.pow((spawn.getHell()[i].x - _x), 2) + Math.pow((spawn.getHell()[i].y - _y), 2));
						
						if(fDistance <= int(spellData.damage[0].range))
						{
							spawn.getHell()[i].fHealth = 0.0;
							spawn.getHell()[i].bDead = true;
						}
					}
				}
				else
				{
					for(i = 0; i < spawn.getHeaven().length; i++)
					{
						fDistance = Math.sqrt(Math.pow((spawn.getHeaven()[i].x - _x), 2) + Math.pow((spawn.getHeaven()[i].y - _y), 2));
						
						if(fDistance <= int(spellData.damage[0].range))
						{
							spawn.getHeaven()[i].fHealth = 0.0;
							spawn.getHeaven()[i].bDead = true;
						}
					}
				}
			}
		}
	}
}
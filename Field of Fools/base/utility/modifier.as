package base.utility
{
	import flash.net.URLLoader;
	import flash.net.URLRequest;
	import flash.events.Event;
	import base.units.unit;
	
	public class modifier
	{
		private static var bLoadFinished:Boolean = false;
		private static var xmlLoader:URLLoader = new URLLoader();
		private static var modifierData:XML;
		
		public static function init() : void
		{
			xmlLoader.load(new URLRequest("data/info/modifier.xml"));
			xmlLoader.addEventListener(Event.COMPLETE, onLoaded, false, 0, true);
		}
		
		private static function onLoaded(e:Event) : void
		{
			modifierData = new XML(e.target.data);
			XML.ignoreWhitespace = true;
			
			bLoadFinished = true;
		}
		
		public function modifierCheck(_attack:unit, _defend:unit) : Number
		{
			if(bLoadFinished)
			{
				if(_attack.sAttackType == "normal" && _defend.sArmorType == "normal")
				{
					return modifierData.melee[0].normalnormal;
				}
				else if(_attack.sAttackType == "normal" && _defend.sArmorType == "leicht")
				{
					return modifierData.melee[0].normalleicht;
				}
				else if(_attack.sAttackType == "normal" && _defend.sArmorType == "schild")
				{
					return modifierData.melee[0].normalschild;
				}
				else if(_attack.sAttackType == "stich" && _defend.sArmorType == "normal")
				{
					return modifierData.melee[0].stichnormal;
				}
				else if(_attack.sAttackType == "stich" && _defend.sArmorType == "leicht")
				{
					return modifierData.melee[0].stichleicht;
				}
				else if(_attack.sAttackType == "stich" && _defend.sArmorType == "schild")
				{
					return modifierData.melee[0].stichschild;
				}
				else if(_attack.sAttackType == "sturm" && _defend.sArmorType == "normal")
				{
					return modifierData.melee[0].sturmnormal;
				}
				else if(_attack.sAttackType == "sturm" && _defend.sArmorType == "leicht")
				{
					return modifierData.melee[0].sturmleicht;
				}
				else if(_attack.sAttackType == "sturm" && _defend.sArmorType == "schild")
				{
					return modifierData.melee[0].sturmschild;
				}
				else
				{
					return 1.0;
				}
			}
			else
			{
				return 1.0;
			}
		}
		
		public function modifierCheckRange(_attack:unit, _defend:unit) : Number
		{
			if(bLoadFinished)
			{
				if(_attack.sAttackTypeRange == "normal" && _defend.sArmorType == "normal")
				{
					return modifierData.range[0].normalnormal;
				}
				else if(_attack.sAttackTypeRange == "normal" && _defend.sArmorType == "leicht")
				{
					return modifierData.range[0].normalleicht;
				}
				else if(_attack.sAttackTypeRange == "normal" && _defend.sArmorType == "schild")
				{
					return modifierData.range[0].normalschild;
				}
				else if(_attack.sAttackTypeRange == "stich" && _defend.sArmorType == "normal")
				{
					return modifierData.range[0].stichnormal;
				}
				else if(_attack.sAttackTypeRange == "stich" && _defend.sArmorType == "leicht")
				{
					return modifierData.range[0].stichleicht;
				}
				else if(_attack.sAttackTypeRange == "stich" && _defend.sArmorType == "schild")
				{
					return modifierData.range[0].stichschild;
				}
				else if(_attack.sAttackTypeRange == "sturm" && _defend.sArmorType == "normal")
				{
					return modifierData.range[0].sturmnormal;
				}
				else if(_attack.sAttackTypeRange == "sturm" && _defend.sArmorType == "leicht")
				{
					return modifierData.range[0].sturmleicht;
				}
				else if(_attack.sAttackTypeRange == "sturm" && _defend.sArmorType == "schild")
				{
					return modifierData.range[0].sturmschild;
				}
				else
				{
					return 1.0;
				}
			}
			else
			{
				return 1.0;
			}
		}
	}
}
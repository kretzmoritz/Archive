package base.ui
{
	import flash.display.MovieClip;
	
	public class ingame_creation extends MovieClip
	{
		public function ingame_creation() : void
		{
			var mcTemp:MovieClip;
			
			// Charger buttons
			mcTemp = new unit_button(1, 1);
			mcTemp.x = 30;
			mcTemp.y = 150;
			addChild(mcTemp);
			
			mcTemp = new unit_button(1, 2);
			mcTemp.x = 75;
			mcTemp.y = 150;
			addChild(mcTemp);
			
			mcTemp = new unit_button(1, 3);
			mcTemp.x = 52.5;
			mcTemp.y = 190;
			addChild(mcTemp);
			
			// Melee buttons
			mcTemp = new unit_button(2, 1);
			mcTemp.x = 30;
			mcTemp.y = 265;
			addChild(mcTemp);
			
			mcTemp = new unit_button(2, 2);
			mcTemp.x = 75;
			mcTemp.y = 265;
			addChild(mcTemp);
			
			mcTemp = new unit_button(2, 3);
			mcTemp.x = 52.5;
			mcTemp.y = 305;
			addChild(mcTemp);
			
			// Range buttons
			mcTemp = new unit_button(3, 1);
			mcTemp.x = 30;
			mcTemp.y = 390;
			addChild(mcTemp);
			
			mcTemp = new unit_button(3, 2);
			mcTemp.x = 75;
			mcTemp.y = 390;
			addChild(mcTemp);
			
			mcTemp = new unit_button(3, 3);
			mcTemp.x = 52.5;
			mcTemp.y = 430;
			addChild(mcTemp);
		}
	}
}
package base.ui
{
	import flash.display.MovieClip;
	import base.units.spawn;
	import base.units.unit;

	public class wall
	{
		public static var vWall:Vector.<unit> = new Vector.<unit>(18); // Wall tiles need to be of base type unit so they can be properly z-sorted
		
		public function wall(_container:MovieClip)
		{
			var currentClass:Class;
			var iScale:Number;
			
			if(spawn.isPlayingHeaven())
			{
				iScale = -1.0;
				currentClass = wall_heaven;
			}
			else
			{
				iScale = 1.0;
				currentClass = wall_hell;
			}
			
			var iPosX:int = 135;
			var iPosY:int = -21;
			
			for(var i:int = 0; i < 18; i++)
			{
				vWall[i] = new currentClass();
				vWall[i].x = iPosX;
				vWall[i].y = iPosY;
				vWall[i].scaleX = iScale;
				_container.addChild(vWall[i]);
				_container.addChild(vWall[i].bSprite); // Has to be added for consistency; will not be used
				
				iPosY += 75;
				
				if(i == 8)
				{
					iPosX = 765;
					iPosY = -21;
					
					if(currentClass == wall_heaven)
					{
						currentClass = wall_hell;
					}
					else
					{
						currentClass = wall_heaven;
					}
				}
			}
		}
	}
}
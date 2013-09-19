package base.ui
{
	import flash.display.MovieClip;
	import base.units.spawn;
	
	public class toolbar extends MovieClip
	{
		public function toolbar() : void
		{
			if(spawn.isPlayingHeaven())
			{
				this.gotoAndStop(1);
			}
			else
			{
				this.gotoAndStop(2);
			}
		}
	}
}
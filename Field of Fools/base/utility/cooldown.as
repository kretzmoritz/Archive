package base.utility
{
	import flash.display.MovieClip;
	import flash.display.Shape;
	import flash.utils.getTimer;
	import flash.events.Event;
	
	public class cooldown
	{
		private var fDegree:Number = 360;
		private var fRadius:Number;
		private var fCooldownTime:Number;
		private var fTimeDifference:Number;
		private var fCurrentTime:Number;
		private var shCooldown:Shape = new Shape();
		private var mcParent:MovieClip;
		
		public function cooldown(_time:Number, _x:int, _y:int, _radius:Number, _parent:MovieClip) : void
		{
			fRadius = _radius;
			fCooldownTime = getTimer() + _time * 1000.0;
			fTimeDifference = fCooldownTime - getTimer();
			
			mcParent = _parent;
			
			shCooldown.alpha = 0.95;
			shCooldown.x = _x;
			shCooldown.y = _y;
			mcParent.addChild(shCooldown);
			
			shCooldown.addEventListener(Event.ENTER_FRAME, handler, false, 0, true);
		}
		
		private function handler(e:Event) : void
		{
			fCurrentTime = getTimer();
			
			var fCurrentPercentageCompleted:Number = ((fCooldownTime - getTimer()) / fTimeDifference) * 360.0;
			updateCooldown(fCurrentPercentageCompleted);
			
			if(fCurrentPercentageCompleted <= 0.0)
			{
				shCooldown.removeEventListener(Event.ENTER_FRAME, handler);
				mcParent.removeChild(shCooldown);
			}
		}
		
		private function updateCooldown(_angle:Number) : void
		{
			var fRadians:Number = _angle * Math.PI / 180.0;
			
			shCooldown.graphics.clear();
			shCooldown.graphics.moveTo(0,0);
			shCooldown.graphics.beginFill(0x000000,0.7);

			for(var i:int = 90; i <= _angle + 90; i++)
			{
				shCooldown.graphics.lineTo(fRadius * Math.cos(i * Math.PI / 180), -fRadius * Math.sin(i * Math.PI / 180));
			}
			
			shCooldown.graphics.endFill();
		}
	}
}
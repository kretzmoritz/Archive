package base.units
{	
	import flash.display.MovieClip;
	import flash.events.Event;
	import flash.utils.getTimer;
	import base.units.unit;
	import base.utility.animation;

	public class fireball extends MovieClip
	{
		private var target:unit;
		private var xFinal:int;
		private var yFinal:int;
		private var bDead:Boolean = false;
		private var bUpdatePos:Boolean = true;
		private var fDamage:Number;
		private var iType:int;
		
		public function fireball(_target:unit, _damage:Number, _type:int) : void
		{
			target = _target;
			fDamage = _damage;
			iType = _type;
			
			scaleX = -1.0;
			
			this.gotoAndStop(iType);
			this.addEventListener(Event.ENTER_FRAME, update, false, 0, true);
		}
		
		public function update(e:Event) : void
		{
			if(bUpdatePos)
			{
				xFinal = target.x;
				yFinal = target.y - target.height/2;
			}
			
			if(bUpdatePos && target.bDead)
			{
				bUpdatePos = false;
			}
			
			var fDistance:Number = Math.sqrt(Math.pow((this.x - xFinal), 2) + Math.pow((this.y - yFinal), 2));
			if(fDistance > 7.5 && !bDead)
			{
				var iSpeed:int = 150;
				var fRadians:Number = Math.atan2((yFinal - this.y), (xFinal - this.x));
				var fDegrees:Number = fRadians / Math.PI * 180.0;
				this.rotation = fDegrees;
				
				this.x += iSpeed * animation.getLastFrameTime() * Math.cos(fRadians);
				this.y += iSpeed * animation.getLastFrameTime() * Math.sin(fRadians);
			}
			else if(fDistance <= 7.5 && !bDead)
			{
				if(bUpdatePos)
				{
					target.fHealth -= fDamage;
					target.setHealth(target.fHealth);
					
					if(target.fHealth <= 0.0) // Is target unit dead?
					{
						target.bDead = true;
					}
				}
				
				bDead = true;
				this.gotoAndStop(iType + 2); // Enable fading animation
			}
			
			if(bDead)
			{
				if(this.currentFrame == 3)
				{
					if(this.frame3.currentFrame >= this.frame3.totalFrames)
					{
						this.removeEventListener(Event.ENTER_FRAME, update);
						this.parent.removeChild(this);
					}
				}
				else if(this.currentFrame == 4)
				{
					if(this.frame4.currentFrame >= this.frame4.totalFrames)
					{
						this.removeEventListener(Event.ENTER_FRAME, update);
						this.parent.removeChild(this);
					}
				}
			}
		}
	}
}
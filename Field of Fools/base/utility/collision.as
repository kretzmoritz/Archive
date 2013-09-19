package base.utility
{
	import flash.display.MovieClip;

	public class collision
	{
		public function circleCollisionEnterHitbox(mcAttack:MovieClip, mcDefend:MovieClip) : Boolean
		{
			var distance:Number = Math.sqrt(Math.pow((mcAttack.x - mcDefend.x), 2) + Math.pow((mcAttack.y - mcDefend.y), 2));
			
			if(distance < mcAttack.iHitbox)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		
		public function circleCollisionEnterRange(mcAttack:MovieClip, mcDefend:MovieClip) : Boolean
		{
			var distance:Number = Math.sqrt(Math.pow((mcAttack.x - mcDefend.x), 2) + Math.pow((mcAttack.y - mcDefend.y), 2));
			
			if(distance < mcAttack.iRange)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		
		public function circleCollisionFight(mcAttack:MovieClip, mcDefend:MovieClip) : Boolean
		{
			var distance:Number = Math.sqrt(Math.pow((mcAttack.x - mcDefend.x), 2) + Math.pow((mcAttack.y - mcDefend.y), 2));
			
			if(distance < mcDefend.iHitbox + mcAttack.iRange)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
	}
}
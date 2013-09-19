package base.utility
{
	import flash.display.MovieClip;
	import flash.utils.getTimer;
	import base.units.spawn;
	
	public class animation
	{
		private var iPreviousTime:int = 0;
		private var iCurrentTime:int;
		private static var fLastFrameTime:Number;
		
		public function update() : void
		{
			// Time step
			iCurrentTime = getTimer();
			fLastFrameTime = (iCurrentTime - iPreviousTime) / 1000.0;
			iPreviousTime = iCurrentTime;
			
			for(var i:int = 0; i < spawn.getHeaven().length; i++)
			{
				if(spawn.getHeaven()[i].iCurrentFrame != spawn.getHeaven()[i].currentFrame)
				{
					spawn.getHeaven()[i].bHasAttacked = true;
					
					if(!spawn.getHeaven()[i].bMelee) // Based on attack mode set animation to correct position (attack speed must be consistent)
					{
						spawn.getHeaven()[i].fCurrentSubFrame = spawn.getHeaven()[i].iFrameToHitRange;
					}
					else
					{
						spawn.getHeaven()[i].fCurrentSubFrame = spawn.getHeaven()[i].iFrameToHit;
					}
					
					if(spawn.getHeaven()[i].bNewTarget)
					{
						spawn.getHeaven()[i].fCurrentSubFrame = 0.0;
						
						spawn.getHeaven()[i].bHasAttacked = false;
						spawn.getHeaven()[i].bNewTarget = false;
					}
					
					spawn.getHeaven()[i].iCurrentFrame = spawn.getHeaven()[i].currentFrame;
				}
				
				if((spawn.getHeaven()[i].getChildAt(0) as MovieClip).currentFrame >= (spawn.getHeaven()[i].getChildAt(0) as MovieClip).totalFrames)
				{
					spawn.getHeaven()[i].fCurrentSubFrame -= (spawn.getHeaven()[i].getChildAt(0) as MovieClip).totalFrames;
					spawn.getHeaven()[i].bHasAttacked = false; // Reset attack state
				}
				
				spawn.getHeaven()[i].fCurrentSubFrame += fLastFrameTime / (1.0 / (spawn.getHeaven()[i].fAttackSpeed * (spawn.getHeaven()[i].getChildAt(0) as MovieClip).totalFrames));
				(spawn.getHeaven()[i].getChildAt(0) as MovieClip).gotoAndStop(int(spawn.getHeaven()[i].fCurrentSubFrame));
				(spawn.getHeaven()[i].getChildAt(0) as MovieClip).stop();
			}
			
			for(i = 0; i < spawn.getHell().length; i++)
			{
				if(spawn.getHell()[i].iCurrentFrame != spawn.getHell()[i].currentFrame)
				{
					spawn.getHell()[i].bHasAttacked = true;
					
					if(!spawn.getHell()[i].bMelee) // Based on attack mode set animation to correct position (attack speed must be consistent)
					{
						spawn.getHell()[i].fCurrentSubFrame = spawn.getHell()[i].iFrameToHitRange;
					}
					else
					{
						spawn.getHell()[i].fCurrentSubFrame = spawn.getHell()[i].iFrameToHit;
					}
					
					if(spawn.getHell()[i].bNewTarget)
					{
						spawn.getHell()[i].fCurrentSubFrame = 0.0;
						
						spawn.getHell()[i].bHasAttacked = false;
						spawn.getHell()[i].bNewTarget = false;
					}
					
					spawn.getHell()[i].iCurrentFrame = spawn.getHell()[i].currentFrame;
				}
				
				if((spawn.getHell()[i].getChildAt(0) as MovieClip).currentFrame >= (spawn.getHell()[i].getChildAt(0) as MovieClip).totalFrames)
				{
					spawn.getHell()[i].fCurrentSubFrame -= (spawn.getHell()[i].getChildAt(0) as MovieClip).totalFrames;
					spawn.getHell()[i].bHasAttacked = false; // Reset attack state
				}
				
				spawn.getHell()[i].fCurrentSubFrame += fLastFrameTime / (1.0 / (spawn.getHell()[i].fAttackSpeed * (spawn.getHell()[i].getChildAt(0) as MovieClip).totalFrames));
				(spawn.getHell()[i].getChildAt(0) as MovieClip).gotoAndStop(int(spawn.getHell()[i].fCurrentSubFrame));
				(spawn.getHell()[i].getChildAt(0) as MovieClip).stop();
			}
		}
		
		public static function getLastFrameTime() : Number
		{
			return fLastFrameTime;
		}
	}
}
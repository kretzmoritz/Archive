package base.utility
{
	import flash.display.MovieClip;
	import flash.text.TextFormat;
	import base.units.spawn;
	
	public class stack_count extends MovieClip
	{
		public function execute() : void
		{
			var fDistance:Number;
			var fCheck:Number = 20.0;
			
			for(var i:int = 0; i < spawn.getHeaven().length; i++)
			{
				spawn.getHeaven()[i].iStackCount = 1;
				spawn.getHeaven()[i].iParentStack = -1;
			}
			
			for(i = 0; i < spawn.getHeaven().length; i++)
			{
				if(spawn.getHeaven()[i].iParentStack == -1)
				{
					for(var k:int = 0; k < spawn.getHeaven().length; k++)
					{
						if(i != k && spawn.getHeaven()[k].iStackCount == 1 && spawn.getHeaven()[k].iParentStack == -1)
						{
							fDistance = Math.sqrt(Math.pow(spawn.getHeaven()[i].x - spawn.getHeaven()[k].x, 2) + Math.pow(spawn.getHeaven()[i].y - spawn.getHeaven()[k].y, 2));
							
							if(fDistance < fCheck)
							{
								spawn.getHeaven()[i].iStackCount++;
								spawn.getHeaven()[k].iParentStack = i;
							}
						}
					}
				}
				else if(spawn.getHeaven()[i].iParentStack != -1)
				{
					for(k = 0; k < spawn.getHeaven().length; k++)
					{
						if(i != k && spawn.getHeaven()[k].iStackCount == 1 && spawn.getHeaven()[k].iParentStack == -1)
						{
							fDistance = Math.sqrt(Math.pow(spawn.getHeaven()[i].x - spawn.getHeaven()[k].x, 2) + Math.pow(spawn.getHeaven()[i].y - spawn.getHeaven()[k].y, 2));
							
							if(fDistance < fCheck)
							{
								spawn.getHeaven()[spawn.getHeaven()[i].iParentStack].iStackCount++;
								spawn.getHeaven()[k].iParentStack = spawn.getHeaven()[i].iParentStack;
							}
						}
					}
				}
			}
			
			for(i = 0; i < spawn.getHell().length; i++)
			{
				spawn.getHell()[i].iStackCount = 1;
				spawn.getHell()[i].iParentStack = -1;
			}
			
			for(i = 0; i < spawn.getHell().length; i++)
			{
				if(spawn.getHell()[i].iParentStack == -1)
				{
					for(k = 0; k < spawn.getHell().length; k++)
					{
						if(i != k && spawn.getHell()[k].iStackCount == 1 && spawn.getHell()[k].iParentStack == -1)
						{
							fDistance = Math.sqrt(Math.pow(spawn.getHell()[i].x - spawn.getHell()[k].x, 2) + Math.pow(spawn.getHell()[i].y - spawn.getHell()[k].y, 2));
							
							if(fDistance < fCheck)
							{
								spawn.getHell()[i].iStackCount++;
								spawn.getHell()[k].iParentStack = i;
							}
						}
					}
				}
				else if(spawn.getHell()[i].iParentStack != -1)
				{
					for(k = 0; k < spawn.getHell().length; k++)
					{
						if(i != k && spawn.getHell()[k].iStackCount == 1 && spawn.getHell()[k].iParentStack == -1)
						{
							fDistance = Math.sqrt(Math.pow(spawn.getHell()[i].x - spawn.getHell()[k].x, 2) + Math.pow(spawn.getHell()[i].y - spawn.getHell()[k].y, 2));
							
							if(fDistance < fCheck)
							{
								spawn.getHell()[spawn.getHell()[i].iParentStack].iStackCount++;
								spawn.getHell()[k].iParentStack = spawn.getHell()[i].iParentStack;
							}
						}
					}
				}
			}
			
			var tfBoldText:TextFormat = new TextFormat();
			tfBoldText.size = 15;
			tfBoldText.bold = true;
			
			for(i = 0; i < spawn.getHeaven().length; i++)
			{
				if(spawn.getHeaven()[i].iStackCount > 1)
				{
					spawn.getHeaven()[i].stackTextField.text = String(spawn.getHeaven()[i].iStackCount);
					spawn.getHeaven()[i].stackTextField.alpha = 1.0;
					spawn.getHeaven()[i].stackTextField.setTextFormat(tfBoldText);
				}
				else
				{
					spawn.getHeaven()[i].stackTextField.alpha = 0.0;
				}
				
				if(!spawn.getHeaven()[i].bTextFieldOnStage)
				{
					addChild(spawn.getHeaven()[i].stackTextField);
					spawn.getHeaven()[i].bTextFieldOnStage = true;
				}
				else
				{
					spawn.getHeaven()[i].stackTextField.x = spawn.getHeaven()[i].x;
					spawn.getHeaven()[i].stackTextField.y = spawn.getHeaven()[i].y;
				}
			}
			
			for(i = 0; i < spawn.getHell().length; i++)
			{
				if(spawn.getHell()[i].iStackCount > 1)
				{
					spawn.getHell()[i].stackTextField.text = String(spawn.getHell()[i].iStackCount);
					spawn.getHell()[i].stackTextField.alpha = 1.0;
					spawn.getHell()[i].stackTextField.setTextFormat(tfBoldText);
				}
				else
				{
					spawn.getHell()[i].stackTextField.alpha = 0.0;
				}
				
				if(!spawn.getHell()[i].bTextFieldOnStage)
				{
					addChild(spawn.getHell()[i].stackTextField);
					spawn.getHell()[i].bTextFieldOnStage = true;
				}
				else
				{
					spawn.getHell()[i].stackTextField.x = spawn.getHell()[i].x;
					spawn.getHell()[i].stackTextField.y = spawn.getHell()[i].y;
				}
			}
		}
	}
}
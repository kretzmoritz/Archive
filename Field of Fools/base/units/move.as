package base.units
{
	import flash.display.MovieClip;
	import flash.utils.getTimer;
	import base.pathing.setupWaypoints;
	import base.units.spawn;
	import base.utility.animation;
	import base.ui.wall;
	
	public class move
	{
		private var mcUnitContainer:MovieClip;
		
		public function move(_input:MovieClip) : void
		{
			mcUnitContainer = _input;
		}
		
		public function execute() : void
		{
			// Move units
			if(spawn.isPlayingHeaven())
			{
				moveUnits(spawn.getHeaven(), true);
				moveUnits(spawn.getHell(), false);
			}
			else
			{
				moveUnits(spawn.getHeaven(), false);
				moveUnits(spawn.getHell(), true);
			}
			
			// Z-sort
			zsort(spawn.getHeaven(), spawn.getHell());
			
			// Healthbars
			updateHealthbars();
		}
		
		private function moveUnits(_units:Vector.<unit>, _leftToRight:Boolean) : void
		{
			for(var i:int = 0; i < _units.length; i++)
			{
				for(var j:int = 0; j < setupWaypoints.getWaypoints().length; j++)
				{
					if(_units[i].iTarget == -1 && _units[i].iLane == j && ((_units[i].iWaypoint < setupWaypoints.getWaypoints()[j].length && _leftToRight) || (_units[i].iWaypoint >= 0 && !_leftToRight)))
					{
						if(_units[i].bAxisX)
						{
							if(_units[i].bPositiveDirection)
							{
								_units[i].x += _units[i].iSpeed * animation.getLastFrameTime();
								
								if(_units[i].x > setupWaypoints.getWaypoints()[j][_units[i].iWaypoint].x)
								{
									calculateNewDirection(i, j, _units, _leftToRight);
								}
							}
							else
							{
								_units[i].x -= _units[i].iSpeed * animation.getLastFrameTime();
								
								if(_units[i].x < setupWaypoints.getWaypoints()[j][_units[i].iWaypoint].x)
								{
									calculateNewDirection(i, j, _units, _leftToRight);
								}
							}
						}
						else
						{
							if(_units[i].bPositiveDirection)
							{
								_units[i].y += _units[i].iSpeed * animation.getLastFrameTime();
								
								if(_units[i].y > setupWaypoints.getWaypoints()[j][_units[i].iWaypoint].y)
								{
									calculateNewDirection(i, j, _units, _leftToRight);
								}
							}
							else
							{
								_units[i].y -= _units[i].iSpeed * animation.getLastFrameTime();
								
								if(_units[i].y < setupWaypoints.getWaypoints()[j][_units[i].iWaypoint].y)
								{
									calculateNewDirection(i, j, _units, _leftToRight);
								}
							}
						}
					}
				}
			}
		}
		
		private function calculateNewDirection(i:int, j:int, _units:Vector.<unit>, _leftToRight:Boolean) : void
		{
			_units[i].x = setupWaypoints.getWaypoints()[j][_units[i].iWaypoint].x;
			_units[i].y = setupWaypoints.getWaypoints()[j][_units[i].iWaypoint].y;
			
			if(_leftToRight)
			{
				_units[i].iWaypoint++;
			}
			else
			{
				_units[i].iWaypoint--;
			}
			
			if((_units[i].iWaypoint < setupWaypoints.getWaypoints()[j].length && _leftToRight) || (_units[i].iWaypoint >= 0 && !_leftToRight))
			{
				var fScaleX;
				
				if(_units[i].x < setupWaypoints.getWaypoints()[j][_units[i].iWaypoint].x)
				{
					_units[i].bAxisX = true;
					_units[i].bPositiveDirection = true;
					
					_units[i].gotoAndStop(_units[i].iType * 6 + 1);
					
					fScaleX = Math.abs(_units[i].scaleX);
					_units[i].scaleX = fScaleX * -1;
				}
				else if(_units[i].x > setupWaypoints.getWaypoints()[j][_units[i].iWaypoint].x)
				{
					_units[i].bAxisX = true;
					_units[i].bPositiveDirection = false;
					
					_units[i].gotoAndStop(_units[i].iType * 6 + 1);
					
					fScaleX = Math.abs(_units[i].scaleX);
					_units[i].scaleX = fScaleX;
				}
				else if(_units[i].y < setupWaypoints.getWaypoints()[j][_units[i].iWaypoint].y)
				{
					_units[i].bAxisX = false;
					_units[i].bPositiveDirection = true;
					
					_units[i].gotoAndStop(_units[i].iType * 6 + 5);
				}
				else if(_units[i].y > setupWaypoints.getWaypoints()[j][_units[i].iWaypoint].y)
				{
					_units[i].bAxisX = false;
					_units[i].bPositiveDirection = false;
					
					_units[i].gotoAndStop(_units[i].iType * 6 + 3);
				}
			}
		}
		
		private function zsort(_unitsHeaven:Vector.<unit>, _unitsHell:Vector.<unit>) : void
		{
			var bSwapped:Boolean = true;
			var iCount:int = 0;
			var temp:unit;

			var vZSort:Vector.<unit> = new Vector.<unit>(_unitsHeaven.length + _unitsHell.length + wall.vWall.length);
			for(var i:int = 0; i < vZSort.length; i++)
			{
				if(i < _unitsHeaven.length)
				{
					vZSort[i] = _unitsHeaven[i];
				}
				else if(i < _unitsHeaven.length + _unitsHell.length)
				{
					vZSort[i] = _unitsHell[i - _unitsHeaven.length];
				}
				else
				{
					vZSort[i] = wall.vWall[i - _unitsHeaven.length - _unitsHell.length];
				}
			}

			while(bSwapped) // Use bubblesort to sort vector by y position
			{
				bSwapped = false;
				iCount++;
				
				for(i = 0; i < vZSort.length - iCount; i++)
				{
					if(vZSort[i].y > vZSort[i + 1].y)
					{
						temp = vZSort[i];
						vZSort[i] = vZSort[i + 1];
						vZSort[i + 1] = temp;
						
						bSwapped = true;
					}
				}
			}
			
			// Z-sort based on sorted vector
			var iOffset:int = 0;
			for(i = vZSort.length - 1; i >= 0; i--)
			{
				mcUnitContainer.setChildIndex(vZSort[i].bSprite, mcUnitContainer.numChildren - (vZSort.length - i) - iOffset);
				mcUnitContainer.setChildIndex(vZSort[i], mcUnitContainer.numChildren - (vZSort.length - i) - iOffset - 1);
				iOffset++;
			}
			
			vZSort.splice(0, vZSort.length);
		}
		
		private function updateHealthbars() : void
		{
			for(var i:int = 0; i < spawn.getHeaven().length; i++)
			{
				spawn.getHeaven()[i].bSprite.x = spawn.getHeaven()[i].x - spawn.getHeaven()[i].bSprite.width/2;
				spawn.getHeaven()[i].bSprite.y = spawn.getHeaven()[i].y - spawn.getHeaven()[i].iSpriteOffset - 20.0; // Different offset until origin fix
			}
			
			for(i = 0; i < spawn.getHell().length; i++)
			{
				spawn.getHell()[i].bSprite.x = spawn.getHell()[i].x - spawn.getHell()[i].bSprite.width/2;
				spawn.getHell()[i].bSprite.y = spawn.getHell()[i].y - spawn.getHell()[i].iSpriteOffset - 20.0;
			}
		}
	}
}

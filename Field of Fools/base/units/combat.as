package base.units
{
	import flash.display.MovieClip;
	import flash.utils.getTimer;
	import base.units.spawn;
	import base.utility.collision;
	import base.pathing.setupWaypoints;
	import base.utility.modifier;
	import base.units.fireball;
	import base.resource.essence;
	
	public class combat extends MovieClip
	{
		private var cCollisionHandler:collision;
		private var cModifierHandler:modifier;
		
		public function combat() : void
		{
			cCollisionHandler = new collision();
			cModifierHandler = new modifier();
			
			modifier.init();
			essence.init();
		}
		
		public function execute() : void
		{
			fight(spawn.getHeaven(), spawn.getHell());
			fight(spawn.getHell(), spawn.getHeaven());
			removeDead();
		}
		
		private function fight(_attack:Vector.<unit>, _defend:Vector.<unit>) : void
		{
			// Melee attack
			for(var i:int = 0; i < _attack.length; i++)
			{
				for(var j:int = 0; j < _defend.length; j++)
				{
					if(_attack[i].iLane == _defend[j].iLane && !_defend[j].bDead)
					{
						if(_attack[i].iTarget == -1 && cCollisionHandler.circleCollisionEnterHitbox(_attack[i], _defend[j])) // Is this unit in melee to fight?
						{
							_attack[i].iTarget = j;
							_attack[i].bMelee = true;
							_attack[i].bNewTarget = true;
							chooseCorrectAnimation(_attack[i], _defend[j]);
						}
					}
				}
			}
			
			// Range attack
			for(i = 0; i < _attack.length; i++)
			{
				for(j = 0; j < _defend.length; j++)
				{
					if(_attack[i].iLane == _defend[j].iLane && !_defend[j].bDead)
					{
						if(_attack[i].iTarget == -1 && cCollisionHandler.circleCollisionEnterRange(_attack[i], _defend[j])) // Is this unit in range to fight?
						{
							_attack[i].iTarget = j;
							_attack[i].bMelee = false;
							_attack[i].bNewTarget = true;
							chooseCorrectAnimation(_attack[i], _defend[j]);
						}
					}
				}
			}
			
			// Combat
			for(i = 0; i < _attack.length; i++)
			{
				if(_defend.length > 0)
				{
					if(_attack[i].iTarget != -1 && !_attack[i].bMelee && cCollisionHandler.circleCollisionEnterHitbox(_attack[i], _defend[_attack[i].iTarget])) // Switch firing range units into melee mode if target walks into hitbox
					{
						_attack[i].bMelee = true;
						chooseCorrectAnimation(_attack[i], _defend[_attack[i].iTarget]);
					}
					else if(_attack[i].iTarget != -1 && !_attack[i].bMelee) // Switch firing range units into melee mode if any enemy walks into hitbox
					{
						for(var k:int; k < _defend.length; k++)
						{
							if(_attack[i].iLane == _defend[k].iLane && !_defend[k].bDead && cCollisionHandler.circleCollisionEnterHitbox(_attack[i], _defend[k]))
							{
								_attack[i].iTarget = k;
								_attack[i].bMelee = true;
								chooseCorrectAnimation(_attack[i], _defend[k]);
								break;
							}
						}
					}
						
					if(_attack[i].iTarget != -1 && !_attack[i].bHasAttacked && cCollisionHandler.circleCollisionFight(_attack[i], _defend[_attack[i].iTarget]))
					{
						switch(_attack[i].bMelee)
						{
							case false:
								if((_attack[i].getChildAt(0) as MovieClip).currentFrame >= _attack[i].iFrameToHitRange)
								{
									var projectile:fireball;
									if(_attack[i].toString() == "[object unit_range_heaven]")
									{
										projectile = new fireball(_defend[_attack[i].iTarget], (Math.floor(Math.random() * (_attack[i].iMaxDamageRange - _attack[i].iMinDamageRange + 1)) + _attack[i].iMinDamageRange) * cModifierHandler.modifierCheckRange(_attack[i], _defend[_attack[i].iTarget]), 1);
									}
									else
									{
										projectile = new fireball(_defend[_attack[i].iTarget], (Math.floor(Math.random() * (_attack[i].iMaxDamageRange - _attack[i].iMinDamageRange + 1)) + _attack[i].iMinDamageRange) * cModifierHandler.modifierCheckRange(_attack[i], _defend[_attack[i].iTarget]), 2);
									}
									
									projectile.x = _attack[i].x;
									projectile.y = _attack[i].y - _attack[i].height/2;
									addChild(projectile);
									
									_attack[i].bHasAttacked = true;
									chooseCorrectAnimation(_attack[i], _defend[_attack[i].iTarget]);
								}
								break;
							case true:
								if((_attack[i].getChildAt(0) as MovieClip).currentFrame >= _attack[i].iFrameToHit)
								{
									_defend[_attack[i].iTarget].fHealth -= (Math.floor(Math.random() * (_attack[i].iMaxDamage - _attack[i].iMinDamage + 1)) + _attack[i].iMinDamage) * cModifierHandler.modifierCheck(_attack[i], _defend[_attack[i].iTarget]);
									_defend[_attack[i].iTarget].setHealth(_defend[_attack[i].iTarget].fHealth);
									_attack[i].bHasAttacked = true;
									chooseCorrectAnimation(_attack[i], _defend[_attack[i].iTarget]);
								}
								break;
						}
							
						if(_defend[_attack[i].iTarget].fHealth <= 0.0) // Is target unit dead?
						{
							_defend[_attack[i].iTarget].bDead = true;
						}
					}
					else if(_attack[i].iTarget != -1 && !cCollisionHandler.circleCollisionFight(_attack[i], _defend[_attack[i].iTarget])) // Target out of range? Reset to non-combat mode
					{
						_attack[i].iTarget = -1;
						resetToCorrectMove(_attack[i]);
					}
				}
			}
		}
		
		private function removeDead() : void
		{
			for(var i:int = 0; i < spawn.getHeaven().length; i++)
			{
				if(spawn.getHeaven()[i].bDead)
				{
					for(var j:int = 0; j < spawn.getHell().length; j++)
					{
						if(spawn.getHell()[j].iTarget == i)
						{
							spawn.getHell()[j].iTarget = -1;
							resetToCorrectMove(spawn.getHell()[j]);
						}
						else if(spawn.getHell()[j].iTarget > i) // Account for new offset
						{
							spawn.getHell()[j].iTarget--;
						}
					}
					
					if(!spawn.isPlayingHeaven()) // Only spawn essence for dead ENEMY units
					{
						var mcEssence:MovieClip = new essence(spawn.getHeaven()[i].x, spawn.getHeaven()[i].y, spawn.getHeaven()[i].iType);
					}
					
					spawn.getHeaven()[i].stackTextField.parent.removeChild(spawn.getHeaven()[i].stackTextField);
					spawn.getHeaven()[i].bSprite.parent.removeChild(spawn.getHeaven()[i].bSprite);
					spawn.getHeaven()[i].parent.removeChild(spawn.getHeaven()[i]);
					spawn.getHeaven().splice(i, 1);
					i--;
				}
			}
			
			for(i = 0; i < spawn.getHell().length; i++)
			{
				if(spawn.getHell()[i].bDead)
				{
					for(j = 0; j < spawn.getHeaven().length; j++)
					{
						if(spawn.getHeaven()[j].iTarget == i)
						{
							spawn.getHeaven()[j].iTarget = -1;
							resetToCorrectMove(spawn.getHeaven()[j]);
						}
						else if(spawn.getHeaven()[j].iTarget > i) // Account for new offset
						{
							spawn.getHeaven()[j].iTarget--;
						}
					}
					
					if(spawn.isPlayingHeaven()) // Only spawn essence for dead ENEMY units
					{
						mcEssence = new essence(spawn.getHell()[i].x, spawn.getHell()[i].y, spawn.getHell()[i].iType);
					}
					
					spawn.getHell()[i].stackTextField.parent.removeChild(spawn.getHell()[i].stackTextField);
					spawn.getHell()[i].bSprite.parent.removeChild(spawn.getHell()[i].bSprite);
					spawn.getHell()[i].parent.removeChild(spawn.getHell()[i]);
					spawn.getHell().splice(i, 1);
					i--;
				}
			}
		}
		
		private function chooseCorrectAnimation(_attack:unit, _defend:unit) : void
		{	
			var fRadians:Number = Math.atan2(_defend.y - _attack.y, _defend.x - _attack.x);
			var fDegrees:Number = fRadians * 180 / Math.PI;
			var fScaleX:Number;
			
			if((_attack.toString() == "[object unit_range_heaven]" || _attack.toString() == "[object unit_range_hell]") && _attack.bMelee)
			{
				if(fDegrees >= 45.0 && fDegrees <= 135.0)
				{
					_attack.gotoAndStop(27 - _attack.iType * 3 - 2);
				}
				else if(fDegrees <= -45.0 && fDegrees >= -135.0)
				{
					_attack.gotoAndStop(27 - _attack.iType * 3 - 1);
				}
				else
				{
					if(_attack.x < _defend.x)
					{
						fScaleX = Math.abs(_attack.scaleX);
						_attack.scaleX = fScaleX * -1;
					}
					else
					{
						fScaleX = Math.abs(_attack.scaleX);
						_attack.scaleX = fScaleX * 1;
					}
					
					_attack.gotoAndStop(27 - _attack.iType * 3 - 0);
				}
			}
			else
			{
				if(fDegrees >= 45.0 && fDegrees <= 135.0)
				{
					_attack.gotoAndStop(_attack.iType * 6 + 6);
				}
				else if(fDegrees <= -45.0 && fDegrees >= -135.0)
				{
					_attack.gotoAndStop(_attack.iType * 6 + 4);
				}
				else
				{
					if(_attack.x < _defend.x)
					{
						fScaleX = Math.abs(_attack.scaleX);
						_attack.scaleX = fScaleX * -1;
					}
					else
					{
						fScaleX = Math.abs(_attack.scaleX);
						_attack.scaleX = fScaleX * 1;
					}
					
					_attack.gotoAndStop(_attack.iType * 6 + 2);
				}
			}
		}
		
		private function resetToCorrectMove(_attack:unit) : void
		{
			var iFinalWaypoint:int = 0;
			
			if((spawn.isPlayingHeaven() && (_attack.toString() == "[object unit_cf_heaven]" || _attack.toString() == "[object unit_charger_heaven]" || _attack.toString() == "[object unit_melee_heaven]" || _attack.toString() == "[object unit_range_heaven]")) || (!spawn.isPlayingHeaven() && (_attack.toString() == "[object unit_cf_hell]" || _attack.toString() == "[object unit_charger_hell]" || _attack.toString() == "[object unit_melee_hell]" || _attack.toString() == "[object unit_range_hell]")))
			{
				iFinalWaypoint = setupWaypoints.getWaypoints()[_attack.iLane].length;
			}
			
			if(iFinalWaypoint != 0)
			{
				if(_attack.iWaypoint < iFinalWaypoint)
				{
					pickMoveAnimation(_attack);
				}
			}
			else
			{
				if(_attack.iWaypoint >= iFinalWaypoint)
				{
					pickMoveAnimation(_attack);
				}
			}
		}
		
		private function pickMoveAnimation(_attack:unit) : void
		{
			var fScaleX;
			
			if(_attack.x < setupWaypoints.getWaypoints()[_attack.iLane][_attack.iWaypoint].x)
			{
				_attack.bAxisX = true;
				_attack.bPositiveDirection = true;
				
				_attack.gotoAndStop(_attack.iType * 6 + 1);
				
				fScaleX = Math.abs(_attack.scaleX);
				_attack.scaleX = fScaleX * -1;
			}
			else if(_attack.x > setupWaypoints.getWaypoints()[_attack.iLane][_attack.iWaypoint].x)
			{
				_attack.bAxisX = true;
				_attack.bPositiveDirection = false;
				
				_attack.gotoAndStop(_attack.iType * 6 + 1);
				
				fScaleX = Math.abs(_attack.scaleX);
				_attack.scaleX = fScaleX;
			}
			else if(_attack.y < setupWaypoints.getWaypoints()[_attack.iLane][_attack.iWaypoint].y)
			{
				_attack.bAxisX = false;
				_attack.bPositiveDirection = true;
				
				_attack.gotoAndStop(_attack.iType * 6 + 5);
			}
			else if(_attack.y > setupWaypoints.getWaypoints()[_attack.iLane][_attack.iWaypoint].y)
			{
				_attack.bAxisX = false;
				_attack.bPositiveDirection = false;
				
				_attack.gotoAndStop(_attack.iType * 6 + 3);
			}
		}
	}
}
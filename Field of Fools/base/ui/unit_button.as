package base.ui
{
	import flash.display.MovieClip;
	import flash.events.MouseEvent;
	import flash.geom.ColorTransform;
	import flash.utils.getTimer;
	import base.units.spawn;
	import base.ui.lane_choice;
	import base.utility.cooldown;

	public class unit_button extends MovieClip
	{
		private var mcIcon:MovieClip;
		private var iUnit:int;
		private var iUpgrade:int;
		private var cCooldown:cooldown;
		private var fCooldownTime:Number = -1.0;
		
		public function unit_button(_unit:int, _upgrade:int) : void
		{
			this.mouseChildren = false; // All events dispatched to children are sent to parent
			
			iUnit = _unit;
			iUpgrade = _upgrade;
			
			if(spawn.isPlayingHeaven())
			{
				this.gotoAndStop(_upgrade);
			}
			else
			{
				this.gotoAndStop(_upgrade + 3);
			}
			
			switch(_unit)
			{
				case 1:
					if(spawn.isPlayingHeaven())
					{
						mcIcon = new unit_charger_heaven();
						
						switch(_upgrade)
						{
							case 1:
								mcIcon.gotoAndStop(5);
								mcIcon.scaleX = 0.55;
								mcIcon.scaleY = 0.55;
								mcIcon.y = mcIcon.height/2;
								break;
							case 2:
								mcIcon.gotoAndStop(11);
								mcIcon.scaleX = 0.5;
								mcIcon.scaleY = 0.5;
								mcIcon.y = mcIcon.height/2;
								break;
							case 3:
								mcIcon.gotoAndStop(17);
								mcIcon.scaleX = 0.5;
								mcIcon.scaleY = 0.5;
								mcIcon.y = mcIcon.height/2;
								break;
						}
					}
					else
					{
						mcIcon = new unit_charger_hell();
						
						switch(_upgrade)
						{
							case 1:
								mcIcon.gotoAndStop(5);
								mcIcon.scaleX = 0.65;
								mcIcon.scaleY = 0.65;
								mcIcon.y = 0;
								break;
							case 2:
								mcIcon.gotoAndStop(11);
								mcIcon.scaleX = 0.65;
								mcIcon.scaleY = 0.65;
								mcIcon.y = 0;
								break;
							case 3:
								mcIcon.gotoAndStop(17);
								mcIcon.scaleX = 0.5;
								mcIcon.scaleY = 0.5;
								mcIcon.y = 0;
								break;
						}
					}
					break;
				case 2:
					if(spawn.isPlayingHeaven())
					{
						mcIcon = new unit_melee_heaven();
						
						switch(_upgrade)
						{
							case 1:
								mcIcon.gotoAndStop(5);
								mcIcon.scaleX = 0.7;
								mcIcon.scaleY = 0.7;
								mcIcon.y = mcIcon.height/2;
								break;
							case 2:
								mcIcon.gotoAndStop(11);
								mcIcon.scaleX = 0.65;
								mcIcon.scaleY = 0.65;
								mcIcon.y = mcIcon.height/2.5;
								break;
							case 3:
								mcIcon.gotoAndStop(17);
								mcIcon.scaleX = 0.4;
								mcIcon.scaleY = 0.4;
								mcIcon.y = mcIcon.height/3;
								break;
						}
					}
					else
					{
						mcIcon = new unit_melee_hell();
						
						switch(_upgrade)
						{
							case 1:
								mcIcon.gotoAndStop(5);
								mcIcon.scaleX = 0.65;
								mcIcon.scaleY = 0.65;
								mcIcon.y = mcIcon.height/2;
								break;
							case 2:
								mcIcon.gotoAndStop(11);
								mcIcon.scaleX = 0.5;
								mcIcon.scaleY = 0.5;
								mcIcon.y = mcIcon.height/2;
								break;
							case 3:
								mcIcon.gotoAndStop(17);
								mcIcon.scaleX = 0.5;
								mcIcon.scaleY = 0.5;
								mcIcon.y = mcIcon.height/2;
								break;
						}
					}
					break;
				case 3:
					if(spawn.isPlayingHeaven())
					{
						mcIcon = new unit_range_heaven();
						
						switch(_upgrade)
						{
							case 1:
								mcIcon.gotoAndStop(5);
								mcIcon.scaleX = 0.65;
								mcIcon.scaleY = 0.65;
								mcIcon.y = mcIcon.height/2;
								break;
							case 2:
								mcIcon.gotoAndStop(11);
								mcIcon.scaleX = 0.5;
								mcIcon.scaleY = 0.5;
								mcIcon.y = mcIcon.height/2;
								break;
							case 3:
								mcIcon.gotoAndStop(17);
								mcIcon.scaleX = 0.5;
								mcIcon.scaleY = 0.5;
								mcIcon.y = mcIcon.height/2;
								break;
						}
					}
					else
					{
						mcIcon = new unit_range_hell();
						
						switch(_upgrade)
						{
							case 1:
								mcIcon.gotoAndStop(5);
								mcIcon.scaleX = 0.5;
								mcIcon.scaleY = 0.5;
								mcIcon.y = mcIcon.height/2;
								break;
							case 2:
								mcIcon.gotoAndStop(11);
								mcIcon.scaleX = 0.5;
								mcIcon.scaleY = 0.5;
								mcIcon.y = mcIcon.height/2;
								break;
							case 3:
								mcIcon.gotoAndStop(17);
								mcIcon.scaleX = 0.5;
								mcIcon.scaleY = 0.5;
								mcIcon.y = mcIcon.height/2;
								break;
						}
					}
					break;
			}
			
			mcIcon.x = 2.5;
			(mcIcon.getChildAt(0) as MovieClip).stop();
			addChild(mcIcon);
			
			addEventListener(MouseEvent.MOUSE_DOWN, spawn_function, false, 0, true);
			addEventListener(MouseEvent.MOUSE_UP, reset_MouseDownState, false, 0, true);
			addEventListener(MouseEvent.MOUSE_OVER, scale_bigger, false, 0, true);
			addEventListener(MouseEvent.MOUSE_OUT, scale_smaller, false, 0, true);
		}
		
		private function spawn_function(e:MouseEvent) : void
		{
			if(getTimer() >= fCooldownTime)
			{
				this.scaleX = 1.0; // So cooldown object will be a constant size
				this.scaleY = 1.0;
				
				fCooldownTime = getTimer() + 1010.0; // So visual cooldown has definitly finished -> 1.01 seconds
				cCooldown = new cooldown(1.0, 0, 0, width/2 - 7.0, this);
				
				this.scaleX = 0.9;
				this.scaleY = 0.9;
				
				(mcIcon.getChildAt(0) as MovieClip).stop();
				
				switch(iUnit)
				{
					case 1:
						switch(iUpgrade)
						{
							case 1:
								if(spawn.isPlayingHeaven())
								{
									spawn.getSingleton().createUnit(unit_charger_heaven, 1, lane_choice.iCurrentLane);
								}
								else
								{
									spawn.getSingleton().createUnit(unit_charger_hell, 1, lane_choice.iCurrentLane);
								}
								break;
							case 2:
								if(spawn.isPlayingHeaven())
								{
									spawn.getSingleton().createUnit(unit_charger_heaven, 2, lane_choice.iCurrentLane);
								}
								else
								{
									spawn.getSingleton().createUnit(unit_charger_hell, 2, lane_choice.iCurrentLane);
								}
								break;
							case 3:
								if(spawn.isPlayingHeaven())
								{
									spawn.getSingleton().createUnit(unit_charger_heaven, 3, lane_choice.iCurrentLane);
								}
								else
								{
									spawn.getSingleton().createUnit(unit_charger_hell, 3, lane_choice.iCurrentLane);
								}
								break;
						}
						break;
					case 2:
						switch(iUpgrade)
						{
							case 1:
								if(spawn.isPlayingHeaven())
								{
									spawn.getSingleton().createUnit(unit_melee_heaven, 1, lane_choice.iCurrentLane);
								}
								else
								{
									spawn.getSingleton().createUnit(unit_melee_hell, 1, lane_choice.iCurrentLane);
								}
								break;
							case 2:
								if(spawn.isPlayingHeaven())
								{
									spawn.getSingleton().createUnit(unit_melee_heaven, 2, lane_choice.iCurrentLane);
								}
								else
								{
									spawn.getSingleton().createUnit(unit_melee_hell, 2, lane_choice.iCurrentLane);
								}
								break;
							case 3:
								if(spawn.isPlayingHeaven())
								{
									spawn.getSingleton().createUnit(unit_melee_heaven, 3, lane_choice.iCurrentLane);
								}
								else
								{
									spawn.getSingleton().createUnit(unit_melee_hell, 3, lane_choice.iCurrentLane);
								}
								break;
						}
						break;
					case 3:
						switch(iUpgrade)
						{
							case 1:
								if(spawn.isPlayingHeaven())
								{
									spawn.getSingleton().createUnit(unit_range_heaven, 1, lane_choice.iCurrentLane);
								}
								else
								{
									spawn.getSingleton().createUnit(unit_range_hell, 1, lane_choice.iCurrentLane);
								}
								break;
							case 2:
								if(spawn.isPlayingHeaven())
								{
									spawn.getSingleton().createUnit(unit_range_heaven, 2, lane_choice.iCurrentLane);
								}
								else
								{
									spawn.getSingleton().createUnit(unit_range_hell, 2, lane_choice.iCurrentLane);
								}
								break;
							case 3:
								if(spawn.isPlayingHeaven())
								{
									spawn.getSingleton().createUnit(unit_range_heaven, 3, lane_choice.iCurrentLane);
								}
								else
								{
									spawn.getSingleton().createUnit(unit_range_hell, 3, lane_choice.iCurrentLane);
								}
								break;
						}
						break;
				}
			}
		}
		
		private function reset_MouseDownState(e:MouseEvent) : void
		{
			this.scaleX = 1.0;
			this.scaleY = 1.0;
			
			(this.getChildAt(0) as MovieClip).gotoAndStop(1);
		}
		
		private function scale_bigger(e:MouseEvent) : void
		{
			if(getTimer() >= fCooldownTime)
			{
				this.scaleX = 1.1;
				this.scaleY = 1.1;
				
				(this.getChildAt(0) as MovieClip).gotoAndStop(2);
				(mcIcon.getChildAt(0) as MovieClip).play();
				
				this.parent.setChildIndex(this, this.parent.numChildren - 1);
			}
		}
		
		private function scale_smaller(e:MouseEvent) : void
		{
			this.scaleX = 1.0;
			this.scaleY = 1.0;
			
			(this.getChildAt(0) as MovieClip).gotoAndStop(1);
			(mcIcon.getChildAt(0) as MovieClip).stop();
		}
	}
}
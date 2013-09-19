package base.units
{
	import flash.display.MovieClip;
	import flash.net.URLLoader;
	import flash.net.URLRequest;
	import flash.events.Event;
	import flash.display.Sprite;
	import flash.display.Graphics;
	import flash.text.TextField;
	import flash.events.MouseEvent;
	import flash.geom.ColorTransform;
	import base.units.spawn;

	public class unit extends MovieClip
	{
		public var iType:int;
		public var iLane:int;
		public var iWaypoint:int;
		public var bPositiveDirection:Boolean;
		public var bAxisX:Boolean;
		
		public var fHealth:Number;
		public var iTarget:int;
		public var iCurrentFrame:int;
		public var fCurrentSubFrame:Number;
		public var bMelee:Boolean = true;
		
		// Const
		public var iHealth:int;
		public var iSpeed:int;
		public var iMinDamage:int;
		public var iMaxDamage:int;
		public var iMinDamageRange:int;
		public var iMaxDamageRange:int;
		public var fAttackSpeed:Number;
		public var iHitbox:int;
		public var iRange:int;
		public var sAttackType:String;
		public var sAttackTypeRange:String;
		public var sArmorType:String;
		public var iSoulCost:int;
		public var iEssenceCost:int;
		public var iFrameToHit:int;
		public var iFrameToHitRange:int;
		
		public var bNewTarget:Boolean = false;
		public var bHasAttacked:Boolean = false;
		public var bDead:Boolean = false;
		
		public var sName:String;
		public var sInfo:String;
		public var sIconType:String;
		public var sIconStrength:String;
		public var sIconWeakness:String;
		
		// Healthbar
		public var iSpriteOffset:int;
		public var bSprite:Sprite;
		private var hSprite:Sprite;
		private var bHealthbarEnabled:Boolean;
		
		// Stack count
		public var iStackCount:int;
		public var iParentStack:int;
		public var stackTextField:TextField;
		public var bTextFieldOnStage:Boolean = false;
		
		// Mouse over
		private var mcMouseOver:MovieClip;
		private var bMouseOverActive:Boolean = false;
		
		public function unit() : void
		{
			healthbar();
			
			stackTextField = new TextField();
			stackTextField.alpha = 0.0;
		}
		
		public function init(_type:int) : void
		{
			iType = _type - 1;
			unit_loader();
			
			addEventListener(MouseEvent.CLICK, unit_info, false, 0, true);
			addEventListener(Event.REMOVED_FROM_STAGE, destructor, false, 0, true);
		}
		
		private function unit_loader() : void
		{
			var xmlLoader:URLLoader = new URLLoader();
			xmlLoader.addEventListener(Event.COMPLETE, onLoaded, false, 0, true);
			
			switch(this.toString())
			{
				case "[object unit_cf_heaven]":
				case "[object unit_cf_hell]":
					xmlLoader.load(new URLRequest("data/info/unit_cf.xml"));
					break;
				case "[object unit_charger_heaven]":
				case "[object unit_charger_hell]":
					xmlLoader.load(new URLRequest("data/info/unit_charger.xml"));
					break;
				case "[object unit_melee_heaven]":
				case "[object unit_melee_hell]":
					xmlLoader.load(new URLRequest("data/info/unit_melee.xml"));
					break;
				case "[object unit_range_heaven]":
				case "[object unit_range_hell]":
					xmlLoader.load(new URLRequest("data/info/unit_range.xml"));
					break;
			}
			
			this.gotoAndStop(iType * 6 + 1); // Cannonfodder has no upgrades, iType = 0
		}
		
		private function onLoaded(e:Event) : void
		{
			var unitData:XML = new XML(e.target.data);
			XML.ignoreWhitespace = true;
			
			fHealth = unitData.upgrade_level[iType].health;
			iTarget = -1;
			iCurrentFrame = 0;
			fCurrentSubFrame = 0.0;
			
			iHealth = unitData.upgrade_level[iType].health;
			iSpeed = unitData.upgrade_level[iType].speed;
			iMinDamage = unitData.upgrade_level[iType].minDamage;
			iMaxDamage = unitData.upgrade_level[iType].maxDamage;
			iMinDamageRange = unitData.upgrade_level[iType].minDamageRange;
			iMaxDamageRange = unitData.upgrade_level[iType].maxDamageRange;
			fAttackSpeed = unitData.upgrade_level[iType].attackSpeed;
			iHitbox = unitData.upgrade_level[iType].hitbox;
			iRange = unitData.upgrade_level[iType].range;
			sAttackType = unitData.upgrade_level[iType].attackType;
			sAttackTypeRange = unitData.upgrade_level[iType].attackTypeRange;
			sArmorType = unitData.upgrade_level[iType].armorType;
			iSoulCost = unitData.upgrade_level[iType].soulCost;
			iEssenceCost = unitData.upgrade_level[iType].essenceCost;
			
			sInfo = unitData.upgrade_level[iType].unitInfo;
			sIconType = unitData.upgrade_level[iType].unitType;
			sIconStrength = unitData.upgrade_level[iType].strength;
			sIconWeakness = unitData.upgrade_level[iType].weakness;
			
			switch(this.toString()) // Choose impact frame
			{
				case "[object unit_cf_heaven]":
				case "[object unit_charger_heaven]":
				case "[object unit_melee_heaven]":
				case "[object unit_range_heaven]":
					sName = unitData.upgrade_level[iType].unitnameHeaven;
					iFrameToHit = unitData.upgrade_level[iType].frameToHitHeaven;
					iFrameToHitRange = unitData.upgrade_level[iType].frameToHitHeavenRange;
					stackTextField.textColor = 0x0276FD;
					break;
				case "[object unit_cf_hell]":
				case "[object unit_charger_hell]":
				case "[object unit_melee_hell]":
				case "[object unit_range_hell]":
					sName = unitData.upgrade_level[iType].unitnameHell;
					iFrameToHit = unitData.upgrade_level[iType].frameToHitHell;
					iFrameToHitRange = unitData.upgrade_level[iType].frameToHitHellRange;
					stackTextField.textColor = 0xFF0000;
					break;
			}
		}
		
		public function setHealth(_amount:Number) : void
		{
			if(_amount != iHealth && !bHealthbarEnabled)
			{
				bSprite.alpha = 1.0;
				hSprite.alpha = 1.0;
				
				bHealthbarEnabled = true;
			}
			
			if(_amount < 0.0) // So we don't have a negative healthbar effect
			{
				_amount = 0.0;
			}
			
			var hSpriteGraphics:Graphics = hSprite.graphics;
			hSpriteGraphics.lineStyle(0, 0x000000);
			hSpriteGraphics.beginFill(combineRGB(255 - ((_amount / iHealth) * 255), (_amount / iHealth) * 255, 0));
			hSpriteGraphics.lineTo(30, 0);
			hSpriteGraphics.lineTo(30, 4);
			hSpriteGraphics.lineTo(0, 4);
			hSpriteGraphics.lineTo(0, 0);
			hSpriteGraphics.endFill();
			
			hSprite.scaleX = _amount / iHealth;
		}
		
		private function healthbar() : void
		{
			iSpriteOffset = this.height;
			
			bSprite = new Sprite();
			bSprite.alpha = 0.0;
			
			hSprite = new Sprite();
			hSprite.alpha = 0.0;
			bSprite.addChild(hSprite);

			var bSpriteGraphics:Graphics = bSprite.graphics;
			bSpriteGraphics.lineStyle(1, 0x000000);
			bSpriteGraphics.beginFill(0x000000);
			bSpriteGraphics.lineTo(30, 0);
			bSpriteGraphics.lineTo(30, 4);
			bSpriteGraphics.lineTo(0, 4);
			bSpriteGraphics.lineTo(0, 0);
			bSpriteGraphics.endFill();
			
			bHealthbarEnabled = false;
			
			// Enable clicking through healthbars
			bSprite.mouseEnabled = false;
			bSprite.mouseChildren = false;
		}
		
		private function unit_info(e:MouseEvent) : void
		{
			if(!bMouseOverActive)
			{
				for(var i:int = 0; i < spawn.getHeaven().length; i++)
				{
					if(spawn.getHeaven()[i].bMouseOverActive)
					{
						parent.parent.removeChild(spawn.getHeaven()[i].mcMouseOver);
						spawn.getHeaven()[i].bMouseOverActive = false;
					}
				}
				
				for(i = 0; i < spawn.getHell().length; i++)
				{
					if(spawn.getHell()[i].bMouseOverActive)
					{
						parent.parent.removeChild(spawn.getHell()[i].mcMouseOver);
						spawn.getHell()[i].bMouseOverActive = false;
					}
				}
				
				mcMouseOver = new bg_mouse_over();
				mcMouseOver.x = this.parent.width/2 - mcMouseOver.width/2;
				mcMouseOver.y = this.parent.height/2 - mcMouseOver.height/2;
				parent.parent.addChild(mcMouseOver);
				
				mcMouseOver.addEventListener(MouseEvent.CLICK, close_mouse_over, false, 0, true);
				mcMouseOver.addEventListener(Event.REMOVED_FROM_STAGE, destruct_mouse_over, false, 0, true);
				
				bMouseOverActive = true;
			}
		}
		
		private function close_mouse_over(e:MouseEvent) : void
		{
			parent.parent.removeChild(mcMouseOver);
			bMouseOverActive = false;
		}
		
		private function destructor(e:Event) : void
		{
			if(bMouseOverActive)
			{
				parent.parent.removeChild(mcMouseOver);
			}
			
			removeEventListener(MouseEvent.CLICK, unit_info);
			removeEventListener(Event.REMOVED_FROM_STAGE, destructor);
		}
		
		private function destruct_mouse_over(e:Event) : void
		{
			mcMouseOver.removeEventListener(MouseEvent.CLICK, close_mouse_over);
			mcMouseOver.removeEventListener(Event.REMOVED_FROM_STAGE, destruct_mouse_over);
		}
		
		private function combineRGB(r:uint, g:uint, b:uint) : uint
		{
			return ( ( r << 16 ) | ( g << 8 ) | b );
		}
	}
}
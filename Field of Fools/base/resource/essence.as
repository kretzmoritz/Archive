package base.resource
{
	import flash.display.MovieClip;
	import flash.events.Event;
	import flash.text.TextField;
	import flash.text.TextFormat;
	import flash.net.URLLoader;
	import flash.net.URLRequest;
	import base.utility.animation;
	
	public class essence extends MovieClip
	{
		private var iTargetX:int = 50;
		private var iTargetY:int = 50;
		private var iType:int;
		private var fSpeed:Number = 150.0;
		private var fWholeDistance:Number;
		private var tfAddition:TextField = new TextField();
		private var bTextFieldAdded:Boolean = false;
		
		public static var mcContainer:MovieClip = new MovieClip();
		public static var iEssenceCount:int = 0;
		
		private static var bLoadFinished:Boolean = false;
		private static var xmlLoader:URLLoader = new URLLoader();
		private static var essenceData:XML;
		
		public static function init() : void
		{
			xmlLoader.load(new URLRequest("data/info/resource.xml"));
			xmlLoader.addEventListener(Event.COMPLETE, onLoaded, false, 0, true);
		}
		
		private static function onLoaded(e:Event) : void
		{
			essenceData = new XML(e.target.data);
			XML.ignoreWhitespace = true;
			
			bLoadFinished = true;
		}
		
		public function essence(_x:int, _y:int, _type:int) : void
		{
			iType = _type;
			this.gotoAndStop(iType + 1);
			
			this.scaleX = 0.05 + iType * 0.025;
			this.scaleY = 0.05 + iType * 0.025;
			
			this.x = _x;
			this.y = _y;
			
			fWholeDistance = Math.sqrt(Math.pow((this.x - iTargetX), 2) + Math.pow((this.y - iTargetY), 2));
			
			mcContainer.addChild(this);
			
			this.addEventListener(Event.ENTER_FRAME, update, false, 0, true);
		}
		
		public function update(e:Event) : void
		{
			var fDistance:Number = Math.sqrt(Math.pow((this.x - iTargetX), 2) + Math.pow((this.y - iTargetY), 2));
			if(fDistance > 15.0)
			{
				var fRadians:Number = Math.atan2((iTargetY - this.y), (iTargetX - this.x));
				var fDegrees:Number = fRadians / Math.PI * 180.0;
				this.rotation = fDegrees;
				
				this.x += fSpeed * animation.getLastFrameTime() * Math.cos(fRadians);
				this.y += fSpeed * animation.getLastFrameTime() * Math.sin(fRadians);
				this.alpha = fDistance / fWholeDistance;
				
				fSpeed += 15.0;
			}
			else if(!bTextFieldAdded)
			{
				this.alpha = 0.0;
				
				var iValue:int;
				if(bLoadFinished)
				{
					switch(iType)
					{
						case 0:
							iValue = int(essenceData.essence_value1);
							break;
						case 1:
							iValue = int(essenceData.essence_value2);
							break;
						case 2:
							iValue = int(essenceData.essence_value3);
							break;
					}
				}
				else
				{
					iValue = 0;
				}
				
				iEssenceCount += iValue;
				
				var tfBoldText:TextFormat = new TextFormat();
				tfBoldText.size = 24;
				tfBoldText.bold = true;
				
				tfAddition.text = String("+"+iValue);
				tfAddition.setTextFormat(tfBoldText);
				tfAddition.x = this.x;
				tfAddition.y = this.y;
				tfAddition.mouseEnabled = false;
				this.parent.addChild(tfAddition);
				
				bTextFieldAdded = true;
			}
			else
			{
				tfAddition.x += 25.0 * animation.getLastFrameTime();
				tfAddition.y += 25.0 * animation.getLastFrameTime();
				tfAddition.alpha -= 0.025;
				
				if(tfAddition.alpha <= 0.0)
				{
					this.removeEventListener(Event.ENTER_FRAME, update);
					tfAddition.parent.removeChild(tfAddition);
					this.parent.removeChild(this);
				}
			}
		}
	}
}
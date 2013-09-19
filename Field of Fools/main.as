package
{
	import flash.display.StageScaleMode;
	import flash.display.MovieClip;
	import flash.events.Event;
	import flash.events.MouseEvent;
	import flash.ui.ContextMenu;
	import base.pathing.setupLane;
	import base.units.spawn;
	import base.units.move;
	import base.units.combat;
	import base.utility.animation;
	import base.utility.stack_count;
	import base.resource.essence;
	import base.resource.soul;
	import base.spells.spell;
	import base.ui.wall;
	import base.ui.lane_choice;
	import base.ui.ingame_creation;
	import base.ui.toolbar;
	
	public class main extends MovieClip
	{
		private var cLaneHandler : setupLane;
		private var cSpawnHandler : spawn;
		private var cMoveHandler : move;
		private var cCombatHandler : combat;
		private var cAnimationHandler : animation;
		private var cStackCountHandler : stack_count;
		private var cWallHandler : wall;
		private var cLaneChoiceHandler : lane_choice;
		private var cIngameCreationHandler : ingame_creation;
		
		public function main() : void
		{
			stage.scaleMode = StageScaleMode.NO_SCALE;
			
			custom_right_click();
			
			cLaneHandler = new setupLane();
			addChild(cLaneHandler); // Functions as container for tiles
			
			cSpawnHandler = new spawn();
			addChild(cSpawnHandler); // Functions as container for units
			
			cMoveHandler = new move(cSpawnHandler);
			
			cWallHandler = new wall(cSpawnHandler);
			
			cCombatHandler = new combat();
			cCombatHandler.mouseEnabled = false;
			cCombatHandler.mouseChildren = false;
			addChild(cCombatHandler); // Functions as container for projectiles
			
			cAnimationHandler = new animation();
			
			cStackCountHandler = new stack_count();
			cStackCountHandler.mouseEnabled = false;
			cStackCountHandler.mouseChildren = false;
			addChild(cStackCountHandler);
			
			var mcToolbar:MovieClip; // Interface background
			mcToolbar = new toolbar();
			mcToolbar.x = 0;
			mcToolbar.y = 0;
			mcToolbar.mouseEnabled = false;
			mcToolbar.mouseChildren = false;
			addChild(mcToolbar);
			
			cIngameCreationHandler = new ingame_creation();
			addChild(cIngameCreationHandler);
			
			essence.mcContainer.mouseEnabled = false;
			essence.mcContainer.mouseChildren = false;
			addChild(essence.mcContainer);
			soul.init();
			
			spell.init();
			
			cLaneChoiceHandler = new lane_choice();
			
			addEventListener(Event.ENTER_FRAME, handler, false, 0, true);
			addEventListener(MouseEvent.CLICK, choose_lane, false, 0, true);
		}
		
		private function handler(e:Event) : void
		{
			cAnimationHandler.update();
			
			cMoveHandler.execute(); // Move units along path
			cCombatHandler.execute();
			
			cStackCountHandler.execute();
		}
		
		private function choose_lane(e:MouseEvent) : void
		{
			cLaneChoiceHandler.decide(e.stageX, e.stageY);
		}
		
		private function custom_right_click() : void
		{
			var custom_menu:ContextMenu = new ContextMenu();
			custom_menu.hideBuiltInItems();
			this.contextMenu = custom_menu;
		}
	}
}
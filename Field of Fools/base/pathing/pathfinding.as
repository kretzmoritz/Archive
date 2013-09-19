package base.pathing
{
	import flash.display.MovieClip;
	
	public class pathfinding
	{
		public function sort(_input:Vector.< Vector.<MovieClip> >) : void
		{
			var bSwapped:Boolean = true;
			var iCount:int = 0;
			var mcTemp:MovieClip;

			while(bSwapped) // Use bubblesort to sort vector by path numbers
			{
				bSwapped = false;
				iCount++;
				
				for(var i:int = 0; i < _input.length; i++)
				{
					for(var j:int  = 0; j < _input[i].length - iCount; j++)
					{
						if(_input[i][j].path > _input[i][j + 1].path)
						{
							mcTemp = _input[i][j];
							_input[i][j] = _input[i][j + 1];
							_input[i][j + 1] = mcTemp;
							
							bSwapped = true;
						}
					}
				}
			}
		}
	}
}
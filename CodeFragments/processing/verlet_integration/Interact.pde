final float mouseRadius = 17.5f;
boolean destroyMode = false;

void keyPressed()
{
  if (key == ' ')
  {
    graphics.constructCloth(96, 45, 25, 6.0f, 30.0f);
  }
  
  if (key == 'w')
  {
    graphics.wireDraw = !graphics.wireDraw;
  }
  
  if (key == 'd')
  {
    destroyMode = !destroyMode;
  }
}

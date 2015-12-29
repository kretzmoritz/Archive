class Physics
{
  Physics()
  {
    previousMillis = millis();
    float timeStepLengthSeconds = timeStepLength / 1000.0f;
    timeStepLengthSecondsSq = timeStepLengthSeconds * timeStepLengthSeconds;
  }
  
  void update()
  {
    int lastFrameTime = millis() - previousMillis;
    previousMillis = millis();
    lastFrameTime += leftOverTime; //Add time that couldn't be used last frame
    int timeSteps = floor(lastFrameTime / (float)timeStepLength); //Calculate amount of simulation steps we can run this frame
    leftOverTime = lastFrameTime - timeSteps * timeStepLength; //Save remainder
    
    for (int i = 0; i < timeSteps; ++i)
    { 
      for (PointMass p : points)
      {
        p.interact();
        p.update(timeStepLengthSecondsSq, gravity); //Fixed delta time
      }
      
      for (int j = 0; j < constraintSteps; ++j)
      {
        for (PointMass p : points)
        {
            p.enforceConstraints();
        }
      }
    }
  }
  
  //--------------------------------------------------//
  
  int previousMillis;
  int leftOverTime = 0;
  final int timeStepLength = 16; //In ms
  float timeStepLengthSecondsSq;
  
  final int constraintSteps = 3;
  
  final float gravity = 980.0f;
}

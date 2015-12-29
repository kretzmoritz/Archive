class Link //Used for handling distance constraints between PointMasses
{
  Link(PointMass _p1, PointMass _p2, float _initialLength, float _tearDistance, int _linkDirection)
  {
    p1 = _p1;
    p2 = _p2;
    
    initialLength = _initialLength;
    tearDistance = _tearDistance;
    
    linkDirection = _linkDirection;
  }
  
  void enforceConstraints()
  {
    float dX = p1.x - p2.x;
    float dY = p1.y - p2.y;
    float d = sqrt(dX * dX + dY * dY);
    
    if (d >= tearDistance)
    {
      p1.removeLink(this);
      return;
    }
    
    float strength = 1.0f;
    
    if (destroyMode && d <= initialLength / 2.0f)
    {
      strength = 5.0f;
    }
    
    float differenceFactor = (initialLength - d) / d;
    
    p1.x += dX * differenceFactor * 0.5f * strength; //Translate PointMasses so the distance between them will equal initialLength (each PointMass moves half the required distance)
    p1.y += dY * differenceFactor * 0.5f * strength;
    
    p2.x -= dX * differenceFactor * 0.5f * strength;
    p2.y -= dY * differenceFactor * 0.5f * strength;
  }
  
  void draw()
  {
    line(p1.x, p1.y, p2.x, p2.y);
  }
  
  //--------------------------------------------------//
  
  PointMass p1;
  PointMass p2;
  
  float initialLength;
  float tearDistance;
  
  int linkDirection;
}

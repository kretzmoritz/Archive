class DistanceToSide
{
  DistanceToSide(float _distance, int _side)
  {
    distance = _distance;
    side = _side;
  }
  
  float distance;
  int side;
}

class Box
{
  Box(float _x, float _y, float _width, float _height)
  {
    x = _x;
    y = _y;
    
    this.width = _width;
    this.height = _height;
  }
  
  void draw()
  {
    if (graphics.wireDraw)
    {
      rect(x, y, this.width, this.height);
    }
    else
    {
      noStroke();
      
      beginShape();
      texture(cupboard_texture);
      vertex(x, y, 0.0f, 0.0f);
      vertex(x, y + this.height, 0.0f, 1.0f);
      vertex(x + this.width, y + this.height, 1.0f, 1.0f);
      vertex(x + this.width, y, 1.0f, 0.0f);
      endShape();
      
      stroke(0);
    }
  }
  
  //--------------------------------------------------//
  
  int contains(PointMass _p)
  {
    if (_p.x >= x && _p.x <= (x + this.width) && _p.y >= y && _p.y <= (y + this.height))
    {
      DistanceToSide[] distances = new DistanceToSide[4];
      
      distances[0] = new DistanceToSide(_p.x - x, 1);
      distances[1] = new DistanceToSide((x + this.width) - _p.x, 2);
      distances[2] = new DistanceToSide(_p.y - y, 3);
      distances[3] = new DistanceToSide((y + this.height) - _p.y, 4);
      
      for (int i = 1; i < 4; ++i)
      {
        if (distances[0].distance > distances[i].distance) //Sort to find the side our PointMass is closest to
        {
          distances[0] = distances[i];
        }
      }
      
      return distances[0].side;
    }
    
    return 0;
  }
  
  //--------------------------------------------------//
  
  float x;
  float y;
  
  float width;
  float height;
}

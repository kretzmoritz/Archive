class Graphics
{
  void clrScr()
  {
    if (wireDraw)
    {
      background(1.0f, 1.0f, 1.0f);
    }
    else
    {
      background(background);
    }
  }
  
  void update()
  {
    for (Box b : boxes)
    {
      b.draw();
    }
    
    for (PointMass p : points)
    {
      p.draw();
    }
    
    drawRail();
  }
  
  //--------------------------------------------------//
  
  void constructCloth(int _width, int _height, int _offsetY, float _linkInitialLength, float _linkTearDistance)
  {
    points.clear();
    
    int offsetX = (int)((width / 2) - (_width * _linkInitialLength) / 2.0f); //Amount to translate along x-axis so our cloth will be centered
    
    railX = offsetX; //Save cloth data so our rail can use the correct size
    railY = _offsetY;
    railWidth = _width * _linkInitialLength;
    
    for (int y = 0; y < _height; ++y)
    {
      for (int x = 0; x < _width; ++x)
      {
        PointMass p = new PointMass(offsetX + x * _linkInitialLength, _offsetY + y * _linkInitialLength, x / float(_width), y / float(_height));
        
        if (x > 0)
        {
          //PointMass p2 = points.get(y * _width + (x - 1));
          PointMass p2 = points.get(points.size() - 1);
          p.addLink(p2, _linkInitialLength, _linkTearDistance, 0);
        }
        
        if (y > 0)
        {
          PointMass p2 = points.get((y - 1) * _width + x);
          p.addLink(p2, _linkInitialLength, _linkTearDistance, 1);
        }
        else //Immobilize topmost PointMasses (every fifth)
        {
          if (x % 5 == 0)
          {
            p.immobilize();
          }
        }
        
        if (p.links.size() == 2) //PointMasses connected to left and up neighbours save all indices needed to create a textured quad
        {
          p.addQuadVertexIndices(points.size() - 1, (y - 1) * _width + (x - 1), (y - 1) * _width + x);
        }
        
        points.add(p);
      }
    }
  }
  
  void constructBox(float _x, float _y, float _width, float _height)
  {
    Box b = new Box(_x, _y, _width, _height);
    boxes.add(b);
  }
  
  void drawRail()
  {
    if (points.size() > 0 && !wireDraw)
    {
      image(rail, railX, railY, railWidth, 10.0f);
    }
  }
  
  //--------------------------------------------------//
  
  float railX;
  float railY;
  float railWidth;
  
  boolean wireDraw = false;
}

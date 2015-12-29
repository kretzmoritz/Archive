class PointMass //Point in space representing an amount of mass
{
  PointMass(float _x, float _y, float _u, float _v)
  {
    previousX = _x;
    previousY = _y;
    
    x = _x;
    y = _y;
    
    u = _u;
    v = _v;
  }
  
  void interact()
  {
    if (mousePressed)
    {
      if (dist(mouseX, mouseY, x, y) <= mouseRadius)
      {
        if (mouseButton == LEFT)
        {
          previousX = x - (mouseX - pmouseX); //Push PointMass away from mouse position (cursor velocity is taken into account)
          previousY = y - (mouseY - pmouseY);
        }
        else
        {
          links.clear();
        }
      }
    }
  }
  
  void update(float _deltaTimeSq, float _gravity)
  {
    applyForce(0.0f, _gravity * mass);
    
    float velX = x - previousX;
    float velY = y - previousY;
    
    velX *= friction;
    velY *= friction;
    
    previousX = x;
    previousY = y;
    
    x += velX + 0.5f * accX * _deltaTimeSq; //Verlet integration: nextX = x + (x - previousX) + 0.5 * accX * dT * dT
    y += velY + 0.5f * accY * _deltaTimeSq; //http://lonesock.net/article/verlet.html
    
    accX = 0.0f;
    accY = 0.0f;
  }
  
  void enforceConstraints()
  {
    for (int i = 0; i < links.size(); ++i)
    {
      Link l = links.get(i);
      l.enforceConstraints();
    }
    
    for (Box b : boxes)
    {
      switch (b.contains(this))
      {
        case 1: //Left
          x = b.x;
          break;
        case 2: //Right
          x = b.x + b.width;
          break;
        case 3: //Up
          y = b.y;
          break;
        case 4: //Down
          y = b.y + b.height;
          break;
      }
    }
    
    if (immobilized)
    {
      x = immobilizedX;
      y = immobilizedY;
    }
  }
  
  void draw()
  {
    if (graphics.wireDraw)
    {
      for (Link l : links)
      {
        l.draw();
      }
    }
    else
    {
      boolean texturedQuad = false;
      
      if (links.size() == 2)
      {
        boolean linkLeftBottomToLeftTop = false;
        
        for (int i = 0; i < points.get(leftBottomIndex).links.size(); ++i)
        {
          if (points.get(leftBottomIndex).links.get(i).linkDirection == 1)
          {
            linkLeftBottomToLeftTop = true;
            break;
          }
        }
        
        boolean linkRightTopToLeftTop = false;
        
        for (int i = 0; i < points.get(rightTopIndex).links.size(); ++i)
        {
          if (points.get(rightTopIndex).links.get(i).linkDirection == 0)
          {
            linkRightTopToLeftTop = true;
            break;
          }
        }
        
        if (linkLeftBottomToLeftTop && linkRightTopToLeftTop)
        {
          texturedQuad = true;
          
          noStroke();
          
          beginShape();
          texture(cloth_texture);
          vertex(x, y, u, v);
          vertex(points.get(leftBottomIndex).x, points.get(leftBottomIndex).y, points.get(leftBottomIndex).u, points.get(leftBottomIndex).v);
          vertex(points.get(leftTopIndex).x, points.get(leftTopIndex).y, points.get(leftTopIndex).u, points.get(leftTopIndex).v);
          vertex(points.get(rightTopIndex).x, points.get(rightTopIndex).y, points.get(rightTopIndex).u, points.get(rightTopIndex).v);
          endShape();
          
          stroke(0);
        }
      }
      
      if (!texturedQuad)
      {
        int pixelPositionX = int(u * cloth_texture.width);
        int pixelPositionY = int(v * cloth_texture.height);
        
        stroke(cloth_texture.get(pixelPositionX, pixelPositionY));
        
        for (Link l : links)
        {
          l.draw();
        }
        
        stroke(0);
      }
    }
  }
  
  //--------------------------------------------------//
  
  void applyForce(float _fX, float _fY)
  {
    accX += _fX / mass;
    accY += _fY / mass;
  }
  
  void immobilize()
  {
    immobilized = true;
    immobilizedX = x;
    immobilizedY = y;
  }
  
  void mobilize()
  {
    immobilized = false;
  }
  
  void addQuadVertexIndices(int _leftBottomIndex, int _leftTopIndex, int _rightTopIndex)
  {
    leftBottomIndex = _leftBottomIndex;
    leftTopIndex = _leftTopIndex;
    rightTopIndex = _rightTopIndex;
  }
  
  void addLink(PointMass _p, float _initialLength, float _tearDistance, int _linkDirection)
  {
    Link l = new Link(this, _p, _initialLength, _tearDistance, _linkDirection);
    links.add(l);
  }
  
  void removeLink(Link _link)
  {
    links.remove(_link);
  }
  
  //--------------------------------------------------//
  
  float previousX;
  float previousY;
  
  float x;
  float y;
  
  float accX = 0.0f;
  float accY = 0.0f;
  
  final float mass = 1.0f;
  final float friction = 0.99f;
  
  boolean immobilized = false;
  float immobilizedX;
  float immobilizedY;
  
  float u;
  float v;
  
  int leftBottomIndex;
  int leftTopIndex;
  int rightTopIndex;
  
  ArrayList<Link> links = new ArrayList<Link>();
}

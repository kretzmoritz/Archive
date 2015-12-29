ArrayList<PointMass> points = new ArrayList<PointMass>();
ArrayList<Box> boxes = new ArrayList<Box>();

PImage background;
PImage rail;
PImage cloth_texture;
PImage cupboard_texture;

Physics physics = new Physics();
Graphics graphics = new Graphics();

//--------------------------------------------------//

void setup()
{
  size(640, 480, P2D);
  colorMode(RGB, 1.0f);
  frameRate(60);
  textureMode(NORMAL);
  
  background = loadImage("media/img/background.jpg");
  rail = loadImage("media/img/rail.jpg");
  cloth_texture = loadImage("media/img/cloth_texture.jpg");
  cupboard_texture = loadImage("media/img/cupboard_texture.jpg");
  
  graphics.constructCloth(96, 45, 25, 6.0f, 30.0f);
  graphics.constructBox(0.0f, height - 100.0f, 181.0f, 100.0f);
  graphics.constructBox(180.0f, height - 75.0f, 190.0f, 110.0f);
}

void draw()
{
  graphics.clrScr();
  
  physics.update();
  graphics.update();
}

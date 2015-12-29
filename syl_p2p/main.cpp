#include <iostream>
#include <conio.h>

#include "irrlicht/irrlicht.h"
#include "overlay/peer.h"
#include "test/boid.h"
#include "test/fps.h"

#pragma comment(lib, "irrlicht.lib")

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace gui;

void run(IrrlichtDevice* _device, Fps& _fps, IGUIStaticText* _fpsStaticText, SylP2P::Peer& _peer, SylP2P::Networkable<SylP2P::Boid>& _networkable, SylP2P::Boid& _boid, IAnimatedMeshSceneNode* _node)
{
	IVideoDriver* driver = _device->getVideoDriver();
	ISceneManager* scene = _device->getSceneManager();
	IGUIEnvironment* gui = _device->getGUIEnvironment();

	std::map<SylUtil::Uuid, IAnimatedMeshSceneNode*> nodes;

	while (_device->run() && !_kbhit())
	{
		_fps.refresh();
		_fpsStaticText->setText(std::to_wstring(_fps.getFps()).c_str());

		_peer.update();

		auto objects = _networkable.getObjects();
		
		_boid.update(_node, objects, _fps.getLastFrameTime());

		for (auto i = nodes.begin(); i != nodes.end();)
		{
			SylUtil::Uuid uuid = i->first;

			if (objects.find(uuid) != objects.end())
			{
				++i;
			}
			else
			{
				IAnimatedMeshSceneNode* node = i->second;
				node->remove();

				nodes.erase(i++);
			}
		}

		for (auto i = objects.begin(); i != objects.end(); ++i)
		{
			SylUtil::Uuid uuid = i->first;
			SylP2P::Boid& boid = i->second;

			auto j = nodes.find(uuid);

			if (j != nodes.end())
			{
				IAnimatedMeshSceneNode* node = j->second;
				
				boid.position(node);
				boid.rotate(node);
			}
			else
			{
				IAnimatedMeshSceneNode* node = nullptr;
				boid.initNode(node, _device, 0.2f, 60.0f);

				if (node)
				{
					nodes.emplace(uuid, node);
				}
			}
		}

		driver->beginScene(true, true);
		
		scene->drawAll();
		gui->drawAll();

		driver->endScene();
	}
}

int main()
{
	IrrlichtDevice* device = 
		createDevice(EDT_OPENGL, dimension2d<u32>(800, 600), 32, false, false, true);

	if (!device)
	{
		return 1;
	}

	device->setWindowCaption(L"SylP2P Boids");

	IVideoDriver* driver = device->getVideoDriver();
	ISceneManager* scene = device->getSceneManager();
	IGUIEnvironment* gui = device->getGUIEnvironment();

	ICameraSceneNode* camera = scene->addCameraSceneNode(0, vector3df(0.0f, 15.0f, -50.0f), vector3df());
	camera->setFarValue(10000.0f);

	driver->setTextureCreationFlag(ETCF_CREATE_MIP_MAPS, false);

	ISceneNode* skybox = scene->addSkyBoxSceneNode(
		driver->getTexture("../res/skyboxes/stormy_days/up.tga"),
		driver->getTexture("../res/skyboxes/stormy_days/down.tga"),
		driver->getTexture("../res/skyboxes/stormy_days/left.tga"),
		driver->getTexture("../res/skyboxes/stormy_days/right.tga"),
		driver->getTexture("../res/skyboxes/stormy_days/front.tga"),
		driver->getTexture("../res/skyboxes/stormy_days/back.tga"));

	driver->setTextureCreationFlag(ETCF_CREATE_MIP_MAPS, true);

	ITerrainSceneNode* terrain = scene->addTerrainSceneNode(
		"../res/textures/terrain-heightmap.bmp", 0, -1,
		vector3df(-4750.f, -2500.f, -1000.0f),
		vector3df(),
		vector3df(40.0f, 4.5f, 40.0f),
		SColor(255, 255, 255, 255),
		5, ETPS_17, 4);

	terrain->setMaterialTexture(0, driver->getTexture("../res/textures/terrain-texture.jpg"));
	terrain->setMaterialTexture(1, driver->getTexture("../res/textures/terrain-detail.jpg"));
	terrain->setMaterialType(EMT_DETAIL_MAP);
	terrain->setMaterialFlag(EMF_LIGHTING, false);
	terrain->scaleTexture(1.0f, 20.0f);

	ILightSceneNode* light = scene->addLightSceneNode();

	SylP2P::Boid boid;
	boid.initBoid(50.0f);
	
	IAnimatedMeshSceneNode* node = nullptr;
	boid.initNode(node, device, 0.2f, 60.0f);

	Fps fps;
	IGUIStaticText* fpsStaticText = gui->addStaticText(std::to_wstring(fps.getFps()).c_str(), rect<s32>(10, 10, 30, 30));

	std::cout << "Bootstrapping..." << std::endl;

	SylP2P::Peer peer;
	peer.bootstrap();

	SylP2P::Networkable<SylP2P::Boid> networkable(boid);
	networkable.share();
	networkable.registerWith(&peer, 0, SylP2P::boid_interpolation);

	run(device, fps, fpsStaticText, peer, networkable, boid, node);

	device->drop();

	return 0;
}
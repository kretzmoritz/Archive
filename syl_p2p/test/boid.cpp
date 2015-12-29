#include <random>

#include "boid.h"
#include "../../syl_util/math/interpolation.h"

using namespace video;

namespace SylP2P
{
	void Boid::initBoid(float _startingAreaSize)
	{
		std::random_device rd;
		std::mt19937 eng(rd());
		std::uniform_real_distribution<float> distr(-_startingAreaSize / 2.0f, _startingAreaSize / 2.0f);

		m_position = vector3df(distr(eng), distr(eng), distr(eng));
	}

	void Boid::initNode(IAnimatedMeshSceneNode*& _node, IrrlichtDevice* _device, float _scale, float _animationSpeed)
	{
		if (_node || !_device)
		{
			return;
		}

		IVideoDriver* driver = _device->getVideoDriver();
		ISceneManager* scene = _device->getSceneManager();

		IAnimatedMesh* mesh = scene->getMesh("../res/models/bat.md2");

		if (!mesh)
		{
			return;
		}

		_node = scene->addAnimatedMeshSceneNode(mesh);

		if (_node)
		{
			_node->setMaterialTexture(0, driver->getTexture("../res/textures/bat.png"));
			_node->setScale(vector3df(_scale));
			_node->setAnimationSpeed(_animationSpeed);
			_node->setMaterialFlag(EMF_NORMALIZE_NORMALS, true);

			position(_node);
			rotate(_node);
		}
	}

	void Boid::update(IAnimatedMeshSceneNode* _node, std::map<SylUtil::Uuid, Boid>& _others, float _deltaTime)
	{
		//http://www.kfish.org/boids/pseudocode.html

		vector3df rule1; //Boids try to fly towards the centre of mass of neighbouring boids.
		vector3df rule2; //Boids try to keep a small distance away from other objects (including other boids).
		vector3df rule3; //Boids try to match velocity with near boids.
		vector3df rule4; //Tendency towards a particular plac

		for (auto i = _others.begin(); i != _others.end(); ++i)
		{
			Boid& boid = i->second;

			rule1 += boid.m_position;

			if (m_position.getDistanceFromSQ(boid.m_position) < 100.0f)
			{
				rule2 -= m_position - boid.m_position;
			}

			rule3 += boid.m_velocity;
		}

		if (!_others.empty())
		{
			rule1 /= static_cast<float>(_others.size());
			rule3 /= static_cast<float>(_others.size());
		}

		rule4 = -m_position;

		vector3df result = (rule1 * 0.0075f) + (rule2 * 0.2f) + (rule3 * 0.004f) + (rule4 * 0.01f);

		m_velocity += result * _deltaTime;
		m_position += m_velocity * _deltaTime;

		position(_node);
		rotate(_node);
	}

	void Boid::position(IAnimatedMeshSceneNode* _node)
	{
		if (!_node)
		{
			return;
		}

		_node->setPosition(m_position);
	}

	void Boid::rotate(IAnimatedMeshSceneNode* _node)
	{
		if (!_node)
		{
			return;
		}

		vector3df tar = m_position + m_velocity;
		vector3df rot, dif = m_position - tar;
		
		rot.Y = atan2(dif.X, dif.Z) * 180.0f / PI;
		rot.X = -atan2(dif.Y, sqrt(dif.X * dif.X + dif.Z * dif.Z)) * 180.0f / PI;

		_node->setRotation(rot);
	}

	void boid_interpolation(Boid& _result, Boid const& _previousValue, Boid const& _currentValue, float _interpolationProgress)
	{
		SylUtil::linear_interpolation(_result.m_position, _previousValue.m_position, _currentValue.m_position, _interpolationProgress);
		SylUtil::linear_interpolation(_result.m_velocity, _previousValue.m_velocity, _currentValue.m_velocity, _interpolationProgress);
	}
};
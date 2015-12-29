///////////////////////////////////////
//  (             (                
//  )\ )     (    )\ )             
// (()/((    )\  (()/(    (   )    
//  /(_))\ )((_)  /(_))  ))\ /((   
// (_))(()/( _   (_))_  /((_|_))\  
// / __|)(_)) |   |   \(_)) _)((_) 
// \__ \ || | |   | |) / -_)\ V /  
// |___/\_, |_|   |___/\___| \_/   
//      |__/                       
//
// 2014 Moritz Kretz
///////////////////////////////////////

#ifndef SYLP2P_BOID_H
#define SYLP2P_BOID_H

#include <map>

#include "irrlicht/irrlicht.h"
#include "../../syl_util/uuid/uuid.h"

using namespace irr;
using namespace core;
using namespace scene;

namespace SylP2P
{
	class Boid
	{
		friend void boid_interpolation(Boid& _result, Boid const& _previousValue, Boid const& _currentValue, float _interpolationProgress);

	public:
		void initBoid(float _startingAreaSize);
		void initNode(IAnimatedMeshSceneNode*& _node, IrrlichtDevice* _device, float _scale, float _animationSpeed);

		void update(IAnimatedMeshSceneNode* _node, std::map<SylUtil::Uuid, Boid>& _others, float _deltaTime);
		void position(IAnimatedMeshSceneNode* _node);
		void rotate(IAnimatedMeshSceneNode* _node);

	private:
		vector3df m_position;
		vector3df m_velocity;
	};

	void boid_interpolation(Boid& _result, Boid const& _previousValue, Boid const& _currentValue, float _interpolationProgress);
};

#endif
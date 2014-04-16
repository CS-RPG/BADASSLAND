//DynamicPhysicsComponent.hpp
#ifndef _DynamicPhysicsComponent_hpp_included_
#define _DynamicPhysicsComponent_hpp_included_

#include <_includes_badass.hpp>
#include <_includes_system.hpp>


//============DynamicPhysicsComponent=======
//
class DynamicPhysicsComponent : public PhysicsComponent {
public:

									DynamicPhysicsComponent(sf::FloatRect, float);
	virtual void					update(GameObject&, World&, int, float);

};
//
//==========================================


#endif
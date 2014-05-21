//DynamicPhysicsComponent.hpp
#ifndef _DynamicPhysicsComponent_hpp_included_
#define _DynamicPhysicsComponent_hpp_included_

#include <_includes_badass.hpp>
#include <_includes_system.hpp>


//============DynamicPhysicsComponent=======
//
class DynamicPhysicsComponent : public PhysicsComponent {
public:

									DynamicPhysicsComponent(sf::FloatRect rect, float speed);
	virtual void					update(GameObject& object, World& world, int tileSize, float deltaTime);

};
//
//==========================================


#endif

//StaticPhysicsComponent.hpp
#ifndef _StaticPhysicsComponent_hpp_included_
#define _StaticPhysicsComponent_hpp_included_

#include <_includes_badass.hpp>
#include <_includes_system.hpp>


//============StaticPhysicsComponent=======
//
class StaticPhysicsComponent : public PhysicsComponent {
public:

									StaticPhysicsComponent(sf::FloatRect rect);
	virtual void					update(GameObject& object, World& world, int tileSize, float deltaTime);

};
//
//==========================================


#endif

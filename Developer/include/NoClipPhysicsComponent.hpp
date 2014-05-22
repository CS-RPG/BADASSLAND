//NoClipPhysicsComponent.hpp
#ifndef _NoClipPhysicsComponent_hpp_included_
#define _NoClipPhysicsComponent_hpp_included_

#include <_includes_badass.hpp>
#include <_includes_system.hpp>


//============NoClipPhysicsComponent========
class NoClipPhysicsComponent : public PhysicsComponent {
public:

									NoClipPhysicsComponent(sf::FloatRect, float);
	virtual void					update(GameObject&, World&, int, float);

};


#endif

//PhysicsComponent.hpp

#ifndef _PhysicsComponent_hpp_included_
#define _PhysicsComponent_hpp_included_

//Prototypes.
class World;
class GameObject;

#include <_includes_system.hpp>


//============PhysicsComponent==============
//
class PhysicsComponent {
public:

	virtual							~PhysicsComponent();
	virtual void					update(GameObject&, World&, int, float) = 0;

	sf::FloatRect					getRect();
	sf::Vector2f					getMovement();
	int								getDirection();
	float							getSpeed();

	void							setRect(sf::FloatRect);
	void							setMovement(sf::Vector2f);
	void							setDirection(int);
	void							setSpeed(float);

private:

	sf::FloatRect					mRect;
	sf::Vector2f					mMovement;
	int								mDirection;
	float							mSpeed;

};
//
//==========================================


#endif
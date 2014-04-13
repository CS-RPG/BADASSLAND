//BotActiveInputComponent.cpp
#include <BotActiveInputComponent.hpp>

float calculateDistance(sf::FloatRect, sf::FloatRect);


//============BotActiveInputComponent=======
//
BotActiveInputComponent::BotActiveInputComponent() {

	setTargeting(false);
	setTarget(NULL);

}

void BotActiveInputComponent::update(GameObject& object, World& world) {
	
	sf::Vector2f movement = object.getPhysics()->getMovement();

	if(getTarget() == &object) {
		setTargeting(false);
		setTarget(NULL);
	}

	if(!isTargeting()) {
		captureTarget(object, world);
		std::cout << "Not targeted.\n";
	}

	if(isTargeting()) {

		std::cout << "Targeted.\n";
		std::cout << getTarget() << '\n';
		float distance = calculateDistance( object.getPhysics()->getRect(),
											getTarget()->getPhysics()->getRect() );

		std::cout << distance << '\n';

		if(distance > object.getCombat()->getAttackRange() * 6) {	//Change to range multiplier.
			setTargeting(false);
			return;
		}

		if(distance > object.getCombat()->getAttackRange()) {

			if(getTarget()->getPhysics()->getRect().left > object.getPhysics()->getRect().left)
				movement.x += object.getPhysics()->getSpeed();
			else
				movement.x -= object.getPhysics()->getSpeed();

			if(getTarget()->getPhysics()->getRect().top > object.getPhysics()->getRect().top)
				movement.y += object.getPhysics()->getSpeed();
			else
				movement.y -= object.getPhysics()->getSpeed();

		} else {

			movement.x = 0;
			movement.y = 0;
			if(object.getCombat()->isReadyToAttack())
				object.getCombat()->attack(getTarget());

		}

	}

	object.getPhysics()->setMovement(movement);

}
//
//==========================================
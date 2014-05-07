//BotActiveInputComponent.cpp
#include <BotActiveInputComponent.hpp>

float calculateDistance(sf::FloatRect, sf::FloatRect);

void outputPath(std::vector<sf::Vector2i>& path);

extern float				gRangeMultiplier;

//============BotActiveInputComponent=======
//
BotActiveInputComponent::BotActiveInputComponent() {

	setBadDirection(0);
	setTargeting(false);
	setTarget(NULL);

	setPathExists(false);

	getPathUpdateClock().restart();

	setUpdateFrequency(1);	///EXTERMINATE!!!!

}

void BotActiveInputComponent::update(GameObject& object, World& world, config& config) {

	if(getTarget() == &object) {
		setTargeting(false);
		setTarget(NULL);
	}

	if(!isTargeting()) {
		captureTarget(object, world);
		//std::cout << "Not targeted.\n";
	}

	if(isTargeting()) {

		sf::FloatRect objectRect = object.getPhysics()->getRect();

		//std::cout << "Targeted.\n";
		//std::cout << getTarget() << '\n';
		float distance = calculateDistance( objectRect,
											getTarget()->getPhysics()->getRect() );

		//std::cout << "Distance to target: " << distance << '\n';

		if(distance > object.getCombat()->getAttackRange() * gRangeMultiplier) {

			setTargeting(false);
			return;

		}

		if(distance > object.getCombat()->getAttackRange()) {

			moveToTarget(object, *getTarget(), world, config.tileSize);

		} else {

			stop(object);
			if(object.getCombat()->isReadyToAttack() && isTargeting())
				object.getCombat()->attack(object, *(getTarget()));

		}

	} else {

		getPath().clear();
		setPathExists(false);

	}

}
//
//==========================================

//BotPassiveInputComponent.cpp
#include <SFML/Graphics.hpp>
#include <World.hpp>
#include <GameObject.hpp>

#include <BotPassiveInputComponent.hpp>


//============BotPassiveInputComponent======
//
BotPassiveInputComponent::BotPassiveInputComponent(float changeDirectionFrequency = 4) {
	mChangeDirectionFrequency = changeDirectionFrequency;
	mDirection = rand() % 4 + 1;
	setBadDirection(0);
}

void BotPassiveInputComponent::update(GameObject& source, World& world) {

	sf::Vector2f movement = source.getPhysics()->getMovement();

	if(mMovementClock.getElapsedTime().asSeconds() > mChangeDirectionFrequency) {
		mDirection = rand() % 4 + 1;
		mMovementClock.restart();
	}

	if(getBadDirection() != 0) {
		mDirection = rand() % 4 + 1;
		setBadDirection(0);
	}

	if(mDirection == 1) {movement.x = 0;								movement.y = -source.getPhysics()->getSpeed();}
	if(mDirection == 2) {movement.x = source.getPhysics()->getSpeed();	movement.y = 0;}
	if(mDirection == 3) {movement.x = 0;								movement.y = source.getPhysics()->getSpeed();}
	if(mDirection == 4) {movement.x = -source.getPhysics()->getSpeed();	movement.y = 0;}

	source.getPhysics()->setMovement(movement);

}

float BotPassiveInputComponent::getChangeDirectionFrequency() {
	return mChangeDirectionFrequency;
}

void BotPassiveInputComponent::setChangeDirectionFrequency(float changeDirectionFrequency) {
	mChangeDirectionFrequency = changeDirectionFrequency;
}
//
//==========================================
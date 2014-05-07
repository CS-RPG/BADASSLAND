//BotPassiveInputComponent.cpp
#include <World.hpp>
#include <GameObject.hpp>

#include <BotPassiveInputComponent.hpp>


//============BotPassiveInputComponent======
//
BotPassiveInputComponent::BotPassiveInputComponent(float changeDirectionFrequency = 4) {

	setChangeDirectionFrequency(changeDirectionFrequency);
	setDirection(rand() % 4 + 1);
	setBadDirection(0);

}

void BotPassiveInputComponent::update(GameObject& object, World& world, config& config) {
	wander(object);
}
//
//==========================================
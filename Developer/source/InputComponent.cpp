//InputComponent.cpp
#include <World.hpp>
#include <GameObject.hpp>

#include <InputComponent.hpp>


float calculateDistance(sf::FloatRect, sf::FloatRect);


//extern float				gRangeMultiplier = 6;

//============InputComponent================
//
InputComponent::~InputComponent() {}

void InputComponent::captureTarget(GameObject& object, World& world) {
	
	float distance = object.getCombat()->getAttackRange() * 6;
	mTarget = NULL;

	for(int i = 0; i < world.getGameObjects().size(); ++i) {

		float tempDistance = calculateDistance(object.getPhysics()->getRect(), world.getGameObjects()[i].getPhysics()->getRect());
		if( (object.getSocial()->getFaction() != world.getGameObjects()[i].getSocial()->getFaction()) && 
			(tempDistance <= distance) && 
			(&(world.getGameObjects()[i]) != &object) ) {

			distance = tempDistance;
			mTarget = new GameObject(world.getGameObjects()[i]);

		}

	}
	
	if(mTarget != NULL) {
		setTargeting(true);
		//std::cout << "WIN\n";
	} else {
		setTargeting(false);
		//mTarget = NULL;
		//std::cout << "FAIL\n";
	}
	
}

void InputComponent::moveToTarget(GameObject& object, GameObject& target) {



}

int InputComponent::getBadDirection() {
	return mBadDirection;
}

GameObject* InputComponent::getTarget() {
	return mTarget;
}

bool InputComponent::isTargeting() {
	return mIsTargeting;
}

void InputComponent::setBadDirection(int direction) {
	mBadDirection = direction;
}

void InputComponent::setTargeting(bool isTargeting) {
	mIsTargeting = isTargeting;
}

void InputComponent::setTarget(GameObject* target) {
	mTarget = target;
}
//
//==========================================
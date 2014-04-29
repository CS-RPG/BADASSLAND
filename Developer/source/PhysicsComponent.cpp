//PhysicsComponent.cpp

#include <PhysicsComponent.hpp>


//============PhysicsComponent==============
//
PhysicsComponent::~PhysicsComponent() {}

sf::FloatRect PhysicsComponent::getRect() {
	return mRect;
}

sf::Vector2f PhysicsComponent::getMovement() {
	return mMovement;
}

int PhysicsComponent::getDirection() {
	return mDirection;
}

float PhysicsComponent::getSpeed() {
	return mSpeed;
}

void PhysicsComponent::setRect(sf::FloatRect rect) {
	mRect = rect;
}

void PhysicsComponent::setMovement(sf::Vector2f movement) {
	mMovement = movement;
}

void PhysicsComponent::setDirection(int direction) {
	mDirection = direction;
}

void PhysicsComponent::setSpeed(float speed) {
	mSpeed = speed;
}
//
//==========================================
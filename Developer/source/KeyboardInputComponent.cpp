//KeyboardInputComponent.cpp

#include <KeyboardInputComponent.hpp>


//============KeyboardInputComponent========
//
KeyboardInputComponent::KeyboardInputComponent(sf::Keyboard::Key* keyBindings) {

	mKeyBindings = new sf::Keyboard::Key[4];
	mKeyBindings[0] = *keyBindings;
	mKeyBindings[1] = *(keyBindings + 1);
	mKeyBindings[2] = *(keyBindings + 2);
	mKeyBindings[3] = *(keyBindings + 3);

}

void KeyboardInputComponent::update(GameObject& player, World& world) {

	sf::Vector2f movement = player.getPhysics()->getMovement();

	if(sf::Keyboard::isKeyPressed(mKeyBindings[0]))	movement.y -= player.getPhysics()->getSpeed();
	if(sf::Keyboard::isKeyPressed(mKeyBindings[1]))	movement.y += player.getPhysics()->getSpeed();
	if(sf::Keyboard::isKeyPressed(mKeyBindings[2]))	movement.x -= player.getPhysics()->getSpeed();
	if(sf::Keyboard::isKeyPressed(mKeyBindings[3]))	movement.x += player.getPhysics()->getSpeed();

	player.getPhysics()->setMovement(movement);

}
//
//==========================================

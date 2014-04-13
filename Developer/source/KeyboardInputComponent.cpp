//KeyboardInputComponent.cpp

#include <KeyboardInputComponent.hpp>
extern int					gControlsCount;

//============KeyboardInputComponent========
//
KeyboardInputComponent::KeyboardInputComponent(sf::Keyboard::Key* keyBindings) {

	mKeyBindings = new sf::Keyboard::Key[gControlsCount];
	for(int i = 0; i < gControlsCount; ++i)
		mKeyBindings[i] = *(keyBindings + i);

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

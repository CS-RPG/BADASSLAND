//KeyboardInputComponent.cpp
#include <SFML/Graphics.hpp>
#include <World.hpp>
#include <GameObject.hpp>

#include <KeyboardInputComponent.hpp>


//============KeyboardInputComponent========
//
void KeyboardInputComponent::update(GameObject& player, World& world) {

	sf::Vector2f movement = player.getPhysics()->getMovement();

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up))	movement.y -= player.getPhysics()->getSpeed();
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down))	movement.y += player.getPhysics()->getSpeed();
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left))	movement.x -= player.getPhysics()->getSpeed();
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right))	movement.x += player.getPhysics()->getSpeed();

	player.getPhysics()->setMovement(movement);

}
//
//==========================================

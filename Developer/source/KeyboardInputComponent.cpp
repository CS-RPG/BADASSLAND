//KeyboardInputComponent.cpp
#include <KeyboardInputComponent.hpp>

extern int					gControlsCount;
extern float				gRangeMultiplier;

float calculateDistance(sf::FloatRect object, sf::FloatRect target);


//============KeyboardInputComponent========
//
KeyboardInputComponent::KeyboardInputComponent(sf::Keyboard::Key* keyBindings) {

	mKeyBindings = new sf::Keyboard::Key[gControlsCount];
	for(int i = 0; i < gControlsCount; ++i)
		mKeyBindings[i] = *(keyBindings + i);

}

void KeyboardInputComponent::update(GameObject& player, World& world, config& config) {

	if(sf::Keyboard::isKeyPressed(mKeyBindings[0]))	moveUp(player);
	if(sf::Keyboard::isKeyPressed(mKeyBindings[1]))	moveDown(player);
	if(sf::Keyboard::isKeyPressed(mKeyBindings[2]))	moveLeft(player);
	if(sf::Keyboard::isKeyPressed(mKeyBindings[3]))	moveRight(player);

	//===============ATTACK=========
	if(sf::Mouse::isButtonPressed(sf::Mouse::Left)) {

		sf::FloatRect rect;
		rect.left = world.getMouseCoordinates().x;
		rect.top = world.getMouseCoordinates().y;
		rect.width = 1;
		rect.height = 1;

		for(int i = 0; i < world.getGameObjects().size(); ++i) {
	
			if(rect.intersects(world.getGameObjects()[i].getPhysics()->getRect())) {
				
				float distance = calculateDistance( player.getPhysics()->getRect(),
											world.getGameObjects()[i].getPhysics()->getRect() );

				//If target is within attack range, targeting not myself and target is hostile, ATTACK!!!1111
				if(distance <= player.getCombat()->getAttackRange() * gRangeMultiplier && 
					&player != &(world.getGameObjects()[i]) && 
						world.areEnemies(player, world.getGameObjects()[i])) {

					if(player.getCombat()->isReadyToAttack())
						player.getCombat()->attack(player, world.getGameObjects()[i]);

				}
			
			}

		}

	}

}
//
//==========================================

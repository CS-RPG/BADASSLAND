//DynamicPhysicsComponent.cpp
#include <DynamicPhysicsComponent.hpp>

extern float				gMapCollisionAccuracy;

//============DynamicPhysicsComponent=======
//
DynamicPhysicsComponent::DynamicPhysicsComponent(sf::FloatRect rect, float speed = 0.1) {

	setRect(rect);
	setSpeed(speed);
	setDirection(0);

}

void DynamicPhysicsComponent::update(GameObject& player, World& world, int tileSize, float deltaTime) {
	
	sf::Vector2f movement = player.getPhysics()->getMovement();

	if((movement.x == 0) && (movement.y == 0))	setDirection(0);
	if((movement.x == 0) && (movement.y < 0))	setDirection(1);
	if((movement.x > 0) && (movement.y < 0))	setDirection(2);
	if((movement.x > 0) && (movement.y == 0))	setDirection(3);
	if((movement.x > 0) && (movement.y > 0))	setDirection(4);
	if((movement.x == 0) && (movement.y > 0))	setDirection(5);
	if((movement.x < 0) && (movement.y > 0))	setDirection(6);
	if((movement.x < 0) && (movement.y == 0))	setDirection(7);
	if((movement.x < 0) && (movement.y < 0))	setDirection(8);

	//Horizontal movement & resolving collisions.
	sf::FloatRect rect = player.getPhysics()->getRect();
	rect.left += movement.x * deltaTime;
	player.getPhysics()->setRect(rect);
	world.resolveObjectCollision(&player, 0);
	world.resolveMapCollision(&player, 0, tileSize);
	
	movement.x = 0;

	//Vertical movement & resolving collisions.
	rect = player.getPhysics()->getRect();
	rect.top += movement.y * deltaTime;
	player.getPhysics()->setRect(rect);
	world.resolveObjectCollision(&player, 1);
	world.resolveMapCollision(&player, 1, tileSize);

	movement.y = 0;

	player.getPhysics()->setMovement(movement);
	
}
//
//==========================================

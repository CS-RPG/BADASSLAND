//InputComponent.cpp
#include <World.hpp>
#include <GameObject.hpp>

#include <InputComponent.hpp>


float calculateDistance(sf::FloatRect, sf::FloatRect);


extern float				gMapCollisionAccuracy;
//extern float				gRangeMultiplier = 6;


//============InputComponent================
//
InputComponent::~InputComponent() {}

void InputComponent::captureTarget(GameObject& object, World& world) {
	
	float distance = object.getCombat()->getAttackRange() * 6;
	mTarget = NULL;

	for(int i = 0; i < world.getGameObjects().size(); ++i) {

		float tempDistance = calculateDistance(object.getPhysics()->getRect(), world.getGameObjects()[i].getPhysics()->getRect());

		if( (world.areEnemies(object, world.getGameObjects()[i])) && 
			(tempDistance <= distance) && 
			(&(world.getGameObjects()[i]) != &object) ) {

			distance = tempDistance;
			mTarget = new GameObject(world.getGameObjects()[i]);

		}

	}
	
	if(mTarget != NULL) {
		
		setTargeting(true);

	} else {
		
		setTargeting(false);
		//mTarget = NULL;
		
	}
	
}

void InputComponent::moveToTarget(GameObject& object, GameObject& target, World& world, int tileSize) {

	sf::FloatRect objectRect = object.getPhysics()->getRect();

	sf::Vector2i source;
	source.x = objectRect.left / tileSize;
	source.y = objectRect.top / tileSize;

	sf::Vector2i destination;
	destination.x = getTarget()->getPhysics()->getRect().left / tileSize;
	destination.y = getTarget()->getPhysics()->getRect().top / tileSize;
	
	if(world.wavePathFind(source, destination, getPath())) {

		//If target is located in another tile, move to the first tile in the path.
		if(getPath().size() != 0/* || getPath().size() > 2*/) {
					
			//std::cout << "Next move: " << getPath()[0].x << ' ' << getPath()[0].y << '\n';
			//outputPath(getPath());
			moveToTile(object, getPath()[0], tileSize);

		//If target is in the same tile, just move towards it.
		} else {
			
			sf::Vector2i targetVector;
			targetVector.x = getTarget()->getPhysics()->getRect().left;
			targetVector.y = getTarget()->getPhysics()->getRect().top;

			moveToTile(object, targetVector, tileSize);

		}

	} else {

		stop(object);

	}

}

void InputComponent::moveToTile(GameObject& object, sf::Vector2i tile, int tileSize) {

	sf::FloatRect objectRect = object.getPhysics()->getRect();
	float speed = object.getPhysics()->getSpeed();

	if(int(objectRect.left) % 120 < speed)
		objectRect.left = floor(objectRect.left);

	if(int(objectRect.top) % 120 < speed)
		objectRect.top = floor(objectRect.top);

	object.getPhysics()->setRect(objectRect);

	if(objectRect.left != tile.x * tileSize) {

		if(objectRect.left > tile.x * tileSize)
			moveLeft(object, objectRect.left - tile.x * tileSize);
		else
			moveRight(object, tile.x * tileSize - objectRect.left);

	}
	
	if (objectRect.top != tile.y * tileSize) {

		if(objectRect.top > tile.y * tileSize)
			moveUp(object, objectRect.top - tile.y * tileSize);
		else
			moveDown(object, tile.y * tileSize - objectRect.top);

	}

}

void InputComponent::stop(GameObject& object) {

	sf::Vector2f movement;

	movement.x = 0;
	movement.y = 0;

	object.getPhysics()->setMovement(movement);

}

void InputComponent::moveLeft(GameObject& object) {

	sf::Vector2f movement = object.getPhysics()->getMovement();

	movement.x -= object.getPhysics()->getSpeed();

	object.getPhysics()->setMovement(movement);

}

void InputComponent::moveRight(GameObject& object) {

	sf::Vector2f movement = object.getPhysics()->getMovement();

	movement.x += object.getPhysics()->getSpeed();

	object.getPhysics()->setMovement(movement);

}

void InputComponent::moveUp(GameObject& object) {

	sf::Vector2f movement = object.getPhysics()->getMovement();

	movement.y -= object.getPhysics()->getSpeed();

	object.getPhysics()->setMovement(movement);

}

void InputComponent::moveDown(GameObject& object) {

	sf::Vector2f movement = object.getPhysics()->getMovement();

	movement.y += object.getPhysics()->getSpeed();

	object.getPhysics()->setMovement(movement);

}

void InputComponent::moveLeft(GameObject& object, float speed) {

	sf::Vector2f movement = object.getPhysics()->getMovement();

	if(speed < object.getPhysics()->getSpeed())
		movement.x -= speed;
	else
		movement.x -= object.getPhysics()->getSpeed();

	object.getPhysics()->setMovement(movement);

}

void InputComponent::moveRight(GameObject& object, float speed) {

	sf::Vector2f movement = object.getPhysics()->getMovement();

	if(speed < object.getPhysics()->getSpeed())
		movement.x += speed;
	else
		movement.x += object.getPhysics()->getSpeed();

	object.getPhysics()->setMovement(movement);

}

void InputComponent::moveUp(GameObject& object, float speed) {

	sf::Vector2f movement = object.getPhysics()->getMovement();

	if(speed < object.getPhysics()->getSpeed())
		movement.y -= speed;
	else
		movement.y -= object.getPhysics()->getSpeed();

	object.getPhysics()->setMovement(movement);

}

void InputComponent::moveDown(GameObject& object, float speed) {

	sf::Vector2f movement = object.getPhysics()->getMovement();

	if(speed < object.getPhysics()->getSpeed())
		movement.y += speed;
	else
		movement.y += object.getPhysics()->getSpeed();

	object.getPhysics()->setMovement(movement);

}

int InputComponent::getBadDirection() {
	return mBadDirection;
}

GameObject* InputComponent::getTarget() {
	return mTarget;
}

std::vector<sf::Vector2i>& InputComponent::getPath() {
	return mPath;
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
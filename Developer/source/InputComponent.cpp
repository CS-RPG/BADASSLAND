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

	std::vector<GameObject>::iterator current = world.getGameObjects().begin();
	std::vector<GameObject>::const_iterator end = world.getGameObjects().end();

	for(; current != end; ++current) {

		float tempDistance = calculateDistance(object.getPhysics()->getRect(), current->getPhysics()->getRect());

		if( (world.areEnemies(object, *current)) && 
			(tempDistance <= distance) && 
			(&(*current) != &object) ) {

			distance = tempDistance;
			mTarget = new GameObject(*current);

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
	
	//If path doesn't exist, update the path.
	if(!mPathExists) {

		mPathExists = world.wavePathFind(source, destination, getPath());
		mPathUpdateClock.restart();

	}

	//If path still doesn't exist, stop.
	//Update path every mUpdateFrequency seconds.
	if(mPathExists && mPathUpdateClock.getElapsedTime().asSeconds() < mUpdateFrequency) {

		sf::Vector2i targetVector;
		targetVector.x = getTarget()->getPhysics()->getRect().left;
		targetVector.y = getTarget()->getPhysics()->getRect().top;

		//getPath()[getPath().size() - 1] = sf::Vector2i(targetVector);

		//If target is located in another tile, move to the first tile in the path.
		if(getPath().size() != 0/* || getPath().size() > 2*/) {
					
			//std::cout << "Next move: " << getPath()[0].x << ' ' << getPath()[0].y << '\n';
			//outputPath(getPath());
			moveToTile(object, getPath()[0], tileSize);

		//If target is in the same tile, just move towards it.
		} else {
			
			sf::Vector2i targetVector;
			targetVector.x = getTarget()->getPhysics()->getRect().left / tileSize;
			targetVector.y = getTarget()->getPhysics()->getRect().top / tileSize;

			moveToTile(object, targetVector, tileSize);

		}

	} else {

		if(!mPathExists)
			stop(object);
		else {

			mPathExists = world.wavePathFind(source, destination, getPath());
			mPathUpdateClock.restart();

		}

	}

}

void InputComponent::moveToTile(GameObject& object, sf::Vector2i tile, int tileSize) {

	sf::FloatRect objectRect = object.getPhysics()->getRect();
	float speed = object.getPhysics()->getSpeed();
	/*
	if(int(objectRect.left) % 120 + (objectRect.left - int(objectRect.left)) < speed)
		objectRect.left = floor(objectRect.left);

	if(int(objectRect.top) % 120 + (objectRect.top - int(objectRect.top)) < speed)
		objectRect.top = floor(objectRect.top);

	object.getPhysics()->setRect(objectRect);
	*/

	//Solving float rounding problems.
	if(abs(objectRect.left - tile.x * tileSize) < speed)
		objectRect.left = tile.x * tileSize;
	if(abs(objectRect.top - tile.y * tileSize) < speed)
		objectRect.top = tile.y * tileSize;

	object.getPhysics()->setRect(objectRect);

	bool reachedX = false;
	bool reachedY = false;

	if(objectRect.left != tile.x * tileSize) {

		if(objectRect.left > tile.x * tileSize)
			moveLeft(object, objectRect.left - tile.x * tileSize);
		else
			moveRight(object, tile.x * tileSize - objectRect.left);

	} else {
		reachedX = true;
	}
	
	if (objectRect.top != tile.y * tileSize) {

		if(objectRect.top > tile.y * tileSize)
			moveUp(object, objectRect.top - tile.y * tileSize);
		else
			moveDown(object, tile.y * tileSize - objectRect.top);

	} else {
		reachedY = true;
	}

	//object.getPhysics()->setRect(objectRect);

	if(reachedX && reachedY)
		getPath().erase(getPath().begin());

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

bool InputComponent::pathExists() {
	return mPathExists;
}

float InputComponent::getUpdateFrequency() {
	return mUpdateFrequency;
}

sf::Clock& InputComponent::getPathUpdateClock() {
	return mPathUpdateClock;
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

void InputComponent::setPathExists(bool pathExists) {
	mPathExists = pathExists;
}

void InputComponent::setUpdateFrequency(float updateTime) {
	mUpdateFrequency = updateTime;
}
//
//==========================================
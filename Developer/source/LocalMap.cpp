//LocalMap.cpp
#include <SFML/Graphics.hpp>
#include <GameObject.hpp>
#include <DataTypes.hpp>
#include <fstream>
#include <sstream>

#include <LocalMap.hpp>

extern sf::Font font;
extern int fontCharacterSize;

//============LocalMap======================
//
LocalMap::LocalMap(sf::Font& font, int fontCharacterSize, int tileSize) {

	setMapHeight(0);
	setMapWidth(0);

	//HUD Health.
	mTextHealth.setFont(font);
	mTextHealth.setString("");
	mTextHealth.setCharacterSize(fontCharacterSize);
	mTextHealth.setStyle(sf::Text::Bold);
	mTextHealth.setColor(sf::Color::Red);
	mTextHealth.setPosition(0, 0);

	//HUD Mana.
	mTextMana.setFont(font);
	mTextMana.setString("");
	mTextMana.setCharacterSize(fontCharacterSize);
	mTextMana.setStyle(sf::Text::Bold);
	mTextMana.setColor(sf::Color::Blue);
	mTextMana.setPosition(0, fontCharacterSize);

	//HUD Enemy count.
	mTextEnemyCount.setFont(font);
	mTextEnemyCount.setString("");
	mTextEnemyCount.setCharacterSize(fontCharacterSize);
	mTextEnemyCount.setStyle(sf::Text::Bold);
	mTextEnemyCount.setColor(sf::Color::Color(184, 138, 0));
	mTextEnemyCount.setPosition(0, fontCharacterSize * 2);

	//HUD Player coordinates.
	mTextPlayerCoordinates.setFont(font);
	mTextPlayerCoordinates.setString("");
	mTextPlayerCoordinates.setCharacterSize(fontCharacterSize);
	mTextPlayerCoordinates.setStyle(sf::Text::Bold);
	mTextPlayerCoordinates.setColor(sf::Color::Color(125, 145, 176));
	mTextPlayerCoordinates.setPosition(0, fontCharacterSize * 3);

	//HUD Mouse coordinates.
	mTextMouseCoordinates.setFont(font);
	mTextMouseCoordinates.setString("");
	mTextMouseCoordinates.setCharacterSize(fontCharacterSize);
	mTextMouseCoordinates.setStyle(sf::Text::Bold);
	mTextMouseCoordinates.setColor(sf::Color::Color(125, 145, 176));
	mTextMouseCoordinates.setPosition(0, fontCharacterSize * 6);

	//Tile object.
	mTile = sf::RectangleShape(sf::Vector2f(tileSize, tileSize));

}

void LocalMap::update(float deltaTime, sf::RenderWindow& window, sf::View& view, config& config) {
	
	//Updating game objects.
	for(int i = 0; i < getGameObjects().size(); ++i)	
		getGameObjects()[i].update(deltaTime, &config, *this);
	
	//HUD.
	std::ostringstream hudHealth;
	//std::ostringstream hudMana;
	std::ostringstream hudEnemyCount;
	std::ostringstream hudPlayerCoordinates;
	std::ostringstream hudMouseCoordinates;

	hudHealth << getGameObjects()[0].getCombat()->getHP();
	//hudMana << player.getMP();
	hudEnemyCount << "Number of game objects: " << getGameObjects().size();
	hudPlayerCoordinates << "X: " << getGameObjects()[0].getPhysics()->getRect().left + config.tileSize / 2 << '\n'
						 << "Y: " << getGameObjects()[0].getPhysics()->getRect().top + config.tileSize / 2;
	hudMouseCoordinates << "X: " << sf::Mouse::getPosition(window).x << '\n'
						<< "Y: " << sf::Mouse::getPosition(window).y;

	//mTextMana.setString(hudMana.str());
	mTextHealth.setString(hudHealth.str());
	mTextEnemyCount.setString(hudEnemyCount.str());
	mTextPlayerCoordinates.setString(hudPlayerCoordinates.str());
	mTextMouseCoordinates.setString(hudMouseCoordinates.str());


	//View.
	sf::Vector2f viewPosition;
	viewPosition.x = getGameObjects()[0].getPhysics()->getRect().left + config.tileSize / 2 - config.screenWidth / 2;
	viewPosition.y = getGameObjects()[0].getPhysics()->getRect().top + config.tileSize / 2 - config.screenHeight / 2;
		
	if(viewPosition.x < 0)														viewPosition.x = 0;
	if(viewPosition.x > getMapWidth() * config.tileSize - config.screenWidth)	viewPosition.x = getMapWidth() * config.tileSize - config.screenWidth;
	if(viewPosition.y < 0)														viewPosition.y = 0;
	if(viewPosition.y > getMapHeight() * config.tileSize - config.screenHeight)	viewPosition.y = getMapHeight() * config.tileSize - config.screenHeight;

	view.reset(sf::FloatRect(viewPosition.x, viewPosition.y, config.screenWidth, config.screenHeight));
	window.setView(view);

	mTextHealth.setPosition(viewPosition.x, viewPosition.y);
	//textMana.setPosition(mViewPosition.x, mViewPosition.y + textMana.getCharacterSize());
	mTextEnemyCount.setPosition(viewPosition.x, viewPosition.y + mTextEnemyCount.getCharacterSize() * 2);
	mTextPlayerCoordinates.setPosition(viewPosition.x, viewPosition.y + mTextPlayerCoordinates.getCharacterSize() * 3);
	mTextMouseCoordinates.setPosition(viewPosition.x, viewPosition.y + mTextPlayerCoordinates.getCharacterSize() * 6);

	//Deleting objects marked for removal.
	for(int i = 0; i < getGameObjects().size(); ++i) {
		if(getGameObjects()[i].getCombat()->isMarkedForRemoval()) {

			getGameObjects().erase(getGameObjects().begin() + i);
			--i;
			//getGameObjects()[i].getInput()->setTargeting(false);

		}
	}


}

void LocalMap::render(sf::RenderWindow& window, sf::View& view, config& config) {

	//Rendering level map tiles.
	for(int i = 0; i < getMapHeight(); ++i) {
		for(int j = 0; j < getMapWidth(); ++j) {
				
			switch(getLevelMap()[i][j]) {
				case 'B': mTile.setFillColor(sf::Color::Black); break;
				case '0': mTile.setFillColor(sf::Color::Green); break;
				default: continue;
			}

			mTile.setPosition(config.tileSize * j, config.tileSize * i);
			window.draw(mTile);
			//tile.setPosition(config.tileSize * j, config.tileSize * i);
			//window.draw(tile);

		}
	}

	//Rendering objects.
	for(int i = 0; i < getGameObjects().size(); ++i) 
		getGameObjects()[i].getGraphics()->draw(window);

	//Rendering HUD.
	window.draw(mTextHealth);
	//window.draw(mTextMana);
	window.draw(mTextEnemyCount);
	window.draw(mTextPlayerCoordinates);
	window.draw(mTextMouseCoordinates);
	
}

void LocalMap::resolveMapCollision(GameObject* object, int direction, int tileSize) {

	sf::FloatRect rect = object->getPhysics()->getRect();
	sf::Vector2f movement = object->getPhysics()->getMovement();

	for(int i = rect.top / tileSize; i < (rect.top + rect.height) / tileSize; ++i)
		for(int j = rect.left / tileSize; j < (rect.left + rect.width) / tileSize; ++j) {

			if(getLevelMap()[i][j] == 'B') {

				if((movement.x > 0) && (direction == 0)) {rect.left = j * tileSize - rect.width;	object->getInput()->setBadDirection(2);}
				if((movement.x < 0) && (direction == 0)) {rect.left = j * tileSize + tileSize;		object->getInput()->setBadDirection(4);}
				if((movement.y > 0) && (direction == 1)) {rect.top = i * tileSize - rect.height;	object->getInput()->setBadDirection(3);}
				if((movement.y < 0) && (direction == 1)) {rect.top = i * tileSize + tileSize;		object->getInput()->setBadDirection(1);}

			}

		}

	object->getPhysics()->setRect(rect);

}

void LocalMap::resolveObjectCollision(GameObject* object, int direction) {

	sf::FloatRect rect = object->getPhysics()->getRect();
	sf::Vector2f movement = object->getPhysics()->getMovement();

	for(int i = 0; i < getGameObjects().size(); ++i) {

		sf::FloatRect currentRect = getGameObjects()[i].getPhysics()->getRect();
		if((rect.intersects(currentRect)) && (object != &getGameObjects()[i])) {

			if((movement.x > 0) && (direction == 0)) {rect.left -= (rect.left + rect.width) - currentRect.left;			object->getInput()->setBadDirection(2);}
			if((movement.x < 0) && (direction == 0)) {rect.left += (currentRect.left + currentRect.width) - rect.left;	object->getInput()->setBadDirection(4);}
			if((movement.y > 0) && (direction == 1)) {rect.top -= (rect.top + rect.height) - currentRect.top;			object->getInput()->setBadDirection(3);}
			if((movement.y < 0) && (direction == 1)) {rect.top += (currentRect.top + currentRect.height) - rect.top;	object->getInput()->setBadDirection(1);}

		}

	}

	object->getPhysics()->setRect(rect);

}

void LocalMap::loadLevelMap(std::string filename) {

	std::ifstream inputFile(filename);

	int height;
	int width;

	inputFile >> height >> width;

	setMapHeight(height);
	setMapWidth(width);

	//Resizing the array.
	getLevelMap().resize(getMapHeight());
	for(int i = 0; i < getMapHeight(); ++i)
		getLevelMap()[i].resize(getMapWidth());

	inputFile.get();
	for(int i = 0; i < getMapHeight(); ++i) {
		for(int j = 0; j < getMapWidth(); ++j) {
			char temp = inputFile.get();
			getLevelMap()[i][j] = int(temp);
		}
		inputFile.get();
	}

	inputFile.close();

}

void LocalMap::deleteLevelMap() {
	getLevelMap().clear();
	getGameObjects().clear();
}
//
//==========================================
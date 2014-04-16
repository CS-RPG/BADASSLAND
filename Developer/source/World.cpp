//World.cpp
#include <World.hpp>


extern sf::Font				gFont;
extern int					gFontSize;

extern float				gMapCollisionAccuracy;
extern float				gMaxZoomRate;

struct SpawnEntry {
	
	int						x;
	int						y;
	std::string				objectID;

};

SpawnEntry*					objects;
int							objectCount = 0;


//============WORLD======================
//
World::World(std::string fileName, config& config) {

	//Creating mObjectMap.
	initializeObjectMap();

	//Initializing mFactionKarma;
	initializeFactionKarmaMap();

	mSpawnClock.restart();

	//===============LEVEL, COLLISION MAP==========
	if(!loadLevelMap(fileName)) {

		std::cout << "Level file error!\n" << "Press space to exit.\n";
		mTerminateGame = true;
		while(!sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {}
		return;

	} else
		mTerminateGame = false;

	buildCollisionMap();


	//===============VIEW==========================
	mViewWidth = config.screenWidth;
	mViewHeight = config.screenHeight;


	//===============FONT==========================
	gFont.loadFromFile("sansation.ttf");
	gFontSize = 30;


	//===============TEXTURES======================
	gTextureHolder.load(Textures::HP_Bar, "./textures/HPBar.png");
	gTextureHolder.load(Textures::Elf_Red, "./textures/red_elf_sprite_list.png");
	gTextureHolder.load(Textures::Elf_Green, "./textures/green_elf_sprite_list.png");
	gTextureHolder.load(Textures::Elf_Yellow, "./textures/yellow_elf_sprite_list.png");
	//gTextureHolder.load(Textures::TileSet, "./textures/testTileSet.png");
	//gTextureHolder.load(Textures::HP_Potion, "./textures/healthPotion.png");

	//sf::Sprite tile(tileSet);


	//===============SOUND=========================
	//sf::SoundBuffer emenyHitSoundBuffer;

	//emenyHitSoundBuffer.loadFromFile("sound1.ogg");

	//sf::Sound emenyHitSound(emenyHitSoundBuffer);


	//===============HUD HEALTH====================
	mTextHealth.setFont(gFont);
	mTextHealth.setString("");
	mTextHealth.setCharacterSize(gFontSize);
	mTextHealth.setStyle(sf::Text::Bold);
	mTextHealth.setColor(sf::Color::Red);
	mTextHealth.setPosition(0, 0);


	//===============HUD MANA======================
	mTextMana.setFont(gFont);
	mTextMana.setString("");
	mTextMana.setCharacterSize(gFontSize);
	mTextMana.setStyle(sf::Text::Bold);
	mTextMana.setColor(sf::Color::Blue);
	mTextMana.setPosition(0, gFontSize);


	//===============HUD ENEMY COUNT===============
	mTextEnemyCount.setFont(gFont);
	mTextEnemyCount.setString("");
	mTextEnemyCount.setCharacterSize(gFontSize);
	mTextEnemyCount.setStyle(sf::Text::Bold);
	mTextEnemyCount.setColor(sf::Color::Color(184, 138, 0));
	mTextEnemyCount.setPosition(0, gFontSize * 2);


	//===============HUD PLAYER COORDINATES========
	mTextObjectCoordinates.setFont(gFont);
	mTextObjectCoordinates.setString("");
	mTextObjectCoordinates.setCharacterSize(gFontSize);
	mTextObjectCoordinates.setStyle(sf::Text::Bold);
	mTextObjectCoordinates.setColor(sf::Color::Color(125, 145, 176));
	mTextObjectCoordinates.setPosition(0, gFontSize * 3);


	//===============HUD MOUSE COORDINATES=========
	mTextMouseCoordinates.setFont(gFont);
	mTextMouseCoordinates.setString("");
	mTextMouseCoordinates.setCharacterSize(gFontSize);
	mTextMouseCoordinates.setStyle(sf::Text::Bold);
	mTextMouseCoordinates.setColor(sf::Color::Color(125, 145, 176));
	mTextMouseCoordinates.setPosition(0, gFontSize * 6);


	//===============HUD OUT CONSOLE===============
	mOutConsole.setFont(gFont);
	mOutConsole.setString("");
	mOutConsole.setCharacterSize(gFontSize);
	mOutConsole.setStyle(sf::Text::Bold);
	mOutConsole.setColor(sf::Color::Color(sf::Color::Red));
	mOutConsole.setPosition(0, gFontSize * 9);


	//===============TILE OBJECT===================
	mTile = sf::RectangleShape(sf::Vector2f(config.tileSize, config.tileSize));


	//Spawning objects.
	//If player exists, last player, written in level-file, will be the view center.
	//If not (and there are any objects), first GameObject will be the view center.
	mCenterObjectN = 0;
	bool playerIsPresent = false;
	for(int i = 0; i < objectCount; ++i)
		spawnObject(mObjectMap[objects[i].objectID], sf::Vector2i(objects[i].x, objects[i].y), config);

	for(int i = 0; i < getGameObjects().size(); ++i) {
		if(getGameObjects()[i].isPlayer()) {
			playerIsPresent = true;
			mCenterObjectN = i;
			std::cout << "Player exists.\n";
		}
	}

	if(!playerIsPresent) 
		if(getGameObjects().size() != 0)
			mCenterObjectN = 0;
	
}

void World::initializeObjectMap() {

	mObjectMap.insert(std::make_pair("Player", Objects::Player));
	mObjectMap.insert(std::make_pair("Elf_Enemy", Objects::Elf_Enemy));
	mObjectMap.insert(std::make_pair("Elf_Minion", Objects::Elf_Minion));
	mObjectMap.insert(std::make_pair("Elf_Friendly", Objects::Elf_Friendly));

}

void World::initializeFactionKarmaMap() {

	mFactionKarmaMap.insert(std::make_pair("players", true));
	mFactionKarmaMap.insert(std::make_pair("gods", true));
	mFactionKarmaMap.insert(std::make_pair("yellow_elves", true));

	mFactionKarmaMap.insert(std::make_pair("devils", false));
	mFactionKarmaMap.insert(std::make_pair("red_elves", false));

}

void World::update(float deltaTime, sf::RenderWindow& window, sf::View& view, config& config) {
	
	if(mTerminateGame)
		window.close();

	//===============UPDATING GAME LOGIC===========
	for(int i = 0; i < getGameObjects().size(); ++i)	
		getGameObjects()[i].update(deltaTime, &config, *this);
	
	bool playerIsAlive = false;

	//Deleting objects marked for removal.
	for(int i = 0; i < getGameObjects().size(); ++i) {
		if(getGameObjects()[i].getCombat()->isMarkedForRemoval()) {

			getGameObjects().erase(getGameObjects().begin() + i);
			--i;

		} else if(getGameObjects()[i].isPlayer())
			playerIsAlive = true;

	}


	//============UPDATING VIEW====================
	sf::Vector2f viewPosition;
	sf::FloatRect screenCenter;
	if(getGameObjects().size() != 0)	
		screenCenter = getGameObjects().at(mCenterObjectN).getPhysics()->getRect();
	else {
		screenCenter.left = mViewWidth / 2;
		screenCenter.top = mViewHeight / 2;
	}

	viewPosition.x = screenCenter.left + screenCenter.width / 2 - mViewWidth / 2;
	viewPosition.y = screenCenter.top + screenCenter.height / 2 - mViewHeight / 2;
	//viewPosition.x = getGameObjects()[0].getPhysics()->getRect().left + config.tileSize / 2 - config.screenWidth / 2;
	//viewPosition.y = getGameObjects()[0].getPhysics()->getRect().top + config.tileSize / 2 - config.screenHeight / 2;
	
	//If focus object is within level borders, do the View optimization stuff.
	if(screenCenter.left > 0 &&	screenCenter.left < getMapWidth() * config.tileSize &&
			screenCenter.top > 0 &&	screenCenter.top < getMapHeight() * config.tileSize) {
	
		if(viewPosition.x < 0)													viewPosition.x = 0;
		if(viewPosition.x > getMapWidth() * config.tileSize - mViewWidth)		viewPosition.x = getMapWidth() * config.tileSize - mViewWidth;
		if(viewPosition.y < 0)													viewPosition.y = 0;
		if(viewPosition.y > getMapHeight() * config.tileSize - mViewHeight)		viewPosition.y = getMapHeight() * config.tileSize - mViewHeight;

		//View at center when whole map fits on a screen.
		if(mViewWidth > getMapWidth() * config.tileSize)
			viewPosition.x = - (mViewWidth - getMapWidth() * config.tileSize) / 2;

		if(mViewHeight> getMapHeight() * config.tileSize)
			viewPosition.y = - (mViewHeight - getMapHeight() * config.tileSize) / 2;
	
	}
	
	view.reset(sf::FloatRect(viewPosition.x, viewPosition.y, mViewWidth, mViewHeight));
	window.setView(view);

	//Updating mMouseCoordinates.
	updateMouseCoordinates(window, config, viewPosition);

	//std::cout << "View position: " << viewPosition.x << " " << viewPosition.y << '\n';


	//===============ZOOM==========================
	if((sf::Keyboard::isKeyPressed(sf::Keyboard::PageDown)) && (mSpawnClock.getElapsedTime().asSeconds() > config.spawnDelay)) {
		
		if(mViewWidth / config.screenWidth <= gMaxZoomRate) {
			mViewWidth *= config.zoomRate;
			mViewHeight *= config.zoomRate;
		}
		mSpawnClock.restart();

	}
	
	if((sf::Keyboard::isKeyPressed(sf::Keyboard::PageUp)) && (mSpawnClock.getElapsedTime().asSeconds() > config.spawnDelay)) {

		if(mViewWidth / config.screenWidth >= 1 / gMaxZoomRate) {
			mViewWidth /= config.zoomRate;
			mViewHeight /= config.zoomRate;
		}
		mSpawnClock.restart();

	}


	//===============SPAWNING OBJECTS==============
	if((sf::Keyboard::isKeyPressed(sf::Keyboard::G)) && (mSpawnClock.getElapsedTime().asSeconds() > config.spawnDelay)) {

		sf::Vector2i coordinates;
		coordinates.x = getMouseCoordinates().x;
		coordinates.y = getMouseCoordinates().y;
		spawnObject(Objects::Elf_Enemy, coordinates, config);
		mSpawnClock.restart();

	}

	if((sf::Keyboard::isKeyPressed(sf::Keyboard::H)) && (mSpawnClock.getElapsedTime().asSeconds() > config.spawnDelay)) {

		sf::Vector2i coordinates;
		coordinates.x = getMouseCoordinates().x;
		coordinates.y = getMouseCoordinates().y;
		spawnObject(Objects::Elf_Friendly, coordinates, config);
		//getGameObjects()[getGameObjects().size() - 1].update(deltaTime, &config, *this);	//
		mSpawnClock.restart();

	}

	if((sf::Keyboard::isKeyPressed(sf::Keyboard::F)) && (mSpawnClock.getElapsedTime().asSeconds() > config.spawnDelay)) {

		sf::Vector2i coordinates;
		coordinates.x = getMouseCoordinates().x;
		coordinates.y = getMouseCoordinates().y;
		spawnObject(Objects::Elf_Minion, coordinates, config);
		mSpawnClock.restart();

	}

	if((sf::Keyboard::isKeyPressed(sf::Keyboard::P)) && (mSpawnClock.getElapsedTime().asSeconds() > config.spawnDelay)) {

		sf::Vector2i coordinates;
		coordinates.x = getMouseCoordinates().x;
		coordinates.y = getMouseCoordinates().y;
		spawnObject(Objects::Player, coordinates, config);
		mSpawnClock.restart();

	}

	//===============NOCLIP CHEAT==================
	if((sf::Keyboard::isKeyPressed(sf::Keyboard::Tilde)) && (mSpawnClock.getElapsedTime().asSeconds() > config.spawnDelay)) {
	
		sf::FloatRect rect = getGameObjects()[mCenterObjectN].getPhysics()->getRect();
		
		getGameObjects()[mCenterObjectN].getSocial()->setName("GOD");
		getGameObjects()[mCenterObjectN].getSocial()->setFaction("gods");

		getGameObjects()[mCenterObjectN].setInput(new KeyboardInputComponent(config.controls1));
		getGameObjects()[mCenterObjectN].setPhysics(new NoClipPhysicsComponent(rect, 1));
		getGameObjects()[mCenterObjectN].setCombat(new InvincibleCombatComponent(9999, 9999, 9999, 9999, 9999));

		mSpawnClock.restart();						

	}

	//===============FOCUS OBJECT CHANGING=========
	if(sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
		for(int i = 0; i < getGameObjects().size(); ++i) {

			sf::FloatRect rect;
			rect.left = getMouseCoordinates().x;
			rect.top = getMouseCoordinates().y;
			rect.width = 1;
			rect.height = 1;
			
			if(rect.intersects(getGameObjects()[i].getPhysics()->getRect())) {
				mCenterObjectN = i;
				break;
			}

		}

	}


	//===============UPDATING HUD==================
	std::ostringstream hudHealth;
	//std::ostringstream hudMana;
	std::ostringstream hudEnemyCount;
	std::ostringstream hudObjectCoordinates;
	std::ostringstream hudMouseCoordinates;
	std::ostringstream hudOutConsole;

	if(getGameObjects().size() != 0) {
		
		hudHealth << getGameObjects()[mCenterObjectN].getCombat()->getHP();
		//hudMana << getGameObjects()[mCenterObjectN].getCombat()->getMP();	
		hudObjectCoordinates << "X: " << getGameObjects()[mCenterObjectN].getPhysics()->getRect().left << '\n'
							 << "Y: " << getGameObjects()[mCenterObjectN].getPhysics()->getRect().top;

	}

	hudEnemyCount << "Number of game objects: " << getGameObjects().size();
	hudMouseCoordinates << "X: " << sf::Mouse::getPosition(window).x << '\n'
						<< "Y: " << sf::Mouse::getPosition(window).y;

	if(!playerIsAlive)
		hudOutConsole << "Player is dead!\n";

	mTextHealth.setString(hudHealth.str());
	//mTextMana.setString(hudMana.str());
	mTextEnemyCount.setString(hudEnemyCount.str());
	mTextObjectCoordinates.setString(hudObjectCoordinates.str());
	mTextMouseCoordinates.setString(hudMouseCoordinates.str());
	mOutConsole.setString(hudOutConsole.str());

	mTextHealth.setPosition(viewPosition.x, viewPosition.y);
	//textMana.setPosition(mViewPosition.x, mViewPosition.y + gFontSize);
	mTextEnemyCount.setPosition(viewPosition.x, viewPosition.y + gFontSize * 2);
	mTextObjectCoordinates.setPosition(viewPosition.x, viewPosition.y + gFontSize * 3);
	mTextMouseCoordinates.setPosition(viewPosition.x, viewPosition.y + gFontSize * 6);
	mOutConsole.setPosition(viewPosition.x, viewPosition.y + gFontSize * 9);

}

void World::render(sf::RenderWindow& window, sf::View& view, config& config) {

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
	window.draw(mTextObjectCoordinates);
	window.draw(mTextMouseCoordinates);
	window.draw(mOutConsole);
	
}

void World::updateMouseCoordinates(sf::RenderWindow& window, config& config, sf::Vector2f viewPosition) {

	mMouseCoordinates.x = viewPosition.x + sf::Mouse::getPosition(window).x / (config.screenWidth / mViewWidth);
	mMouseCoordinates.y = viewPosition.y + sf::Mouse::getPosition(window).y / (config.screenWidth / mViewWidth);

}

void World::resolveMapCollision(GameObject* object, int direction, int tileSize) {

	sf::FloatRect rect = object->getPhysics()->getRect();
	sf::Vector2f movement = object->getPhysics()->getMovement();

	for(int i = rect.top / tileSize; i < (rect.top + rect.height) / tileSize; ++i)
		for(int j = rect.left / tileSize; j < (rect.left + rect.width) / tileSize; ++j) {

			try {
				if(getCollisionMap().at(i).at(j)) {

					if((movement.x > 0) && (direction == 0)) {
						
						if(int(rect.top) % tileSize <= gMapCollisionAccuracy * tileSize) {

							rect.top = ceil(rect.top);
							rect.top -= int(rect.top) % 10;

						}

						rect.left = j * tileSize - rect.width;
						object->getInput()->setBadDirection(2);

					}

					if((movement.x < 0) && (direction == 0)) {

						if(int(rect.top) % tileSize <= gMapCollisionAccuracy * tileSize) {

							rect.top = ceil(rect.top);
							rect.top -= int(rect.top) % 10;

						}

						rect.left = j * tileSize + tileSize;
						object->getInput()->setBadDirection(4);

					}

					if((movement.y > 0) && (direction == 1)) {

						if(int(rect.left) % tileSize <= gMapCollisionAccuracy * tileSize) {

							rect.left = ceil(rect.left);
							rect.left -= int(rect.left) % 10;

						}

						rect.top = i * tileSize - rect.height;
						object->getInput()->setBadDirection(3);

					}

					if((movement.y < 0) && (direction == 1)) {
						
						if(int(rect.left) % tileSize <= gMapCollisionAccuracy * tileSize) {

							rect.left = ceil(rect.left);
							rect.left -= int(rect.left) % 10;

						}

						rect.top = i * tileSize + tileSize;	
						object->getInput()->setBadDirection(1);

					}

				}
			}
			catch(const std::out_of_range& e) {

				object->getSocial()->setName("GOD");
				object->getSocial()->setFaction("gods");
				object->setPhysics(new NoClipPhysicsComponent(rect, 1));
				object->setCombat(new InvincibleCombatComponent(9999, 9999, 9999, 9999, 9999));
				//mGodExists = true;
				std::cout << "Bad luck! Out of map range!\n";
				continue;

			}

		}

	object->getPhysics()->setRect(rect);

}

void World::resolveObjectCollision(GameObject* object, int direction) {

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

bool World::loadLevelMap(std::string filename) {

	std::ifstream inputFile(filename);

	int height;
	int width;

	inputFile >> height >> width;

	if(height <= 0 || width <= 0) {
		std::cout << "Incorrect map size!\n";
		return false;
	}

	setMapHeight(height);
	setMapWidth(width);

	//Resizing the array.
	getLevelMap().resize(getMapHeight());
	for(int i = 0; i < getMapHeight(); ++i)
		getLevelMap()[i].resize(getMapWidth());

	//Loading level data.
	inputFile.get();
	for(int i = 0; i < getMapHeight(); ++i) {
		for(int j = 0; j < getMapWidth(); ++j) {
			
			if(inputFile.eof()) {
				
				std::cout << "Unexpected end of level file!\n";
				return false;

			}
			char temp = inputFile.get();
			getLevelMap()[i][j] = int(temp);

		}
		inputFile.get();
	}

	//Loading spawn data.
	inputFile >> objectCount;
	
	if(objectCount < 0) {
		std::cout << "Incorrect object count!\n";
		return false;
	}

	std::cout << "Objects to spawn: " << objectCount << '\n';

	delete[] objects;
	objects = new SpawnEntry[objectCount];
	for(int i = 0; i < objectCount; ++i) {

		inputFile >> objects[i].x >> objects[i].y;
		inputFile.get();
		inputFile >> objects[i].objectID;
		inputFile.get();

	}

	inputFile.close();

	return true;

}

void World::buildCollisionMap() {

	//Resizing the array.
	getCollisionMap().resize(getMapHeight());
	for(int i = 0; i < getMapHeight(); ++i)
		getCollisionMap()[i].resize(getMapWidth());

	for(int i = 0; i < getMapHeight(); ++i)
		for(int j = 0; j < getMapWidth(); ++j) {

			if(getLevelMap().at(i).at(j) == 'B')
				getCollisionMap().at(i).at(j) = true;
			else
				getCollisionMap().at(i).at(j) = false;

		}

}

void World::deleteLevelMap() {

	getLevelMap().clear();
	getGameObjects().clear();

}

void World::spawnObject(Objects::ID objectID, sf::Vector2i coordinates, config& config) {

	GameObject* temp;
	bool canSpawn = true;
	int width;
	int height;

	switch(objectID) {

		case(Objects::Player):

			width = config.tileSize;
			height = config.tileSize;
			temp = new GameObject(	new KeyboardInputComponent(config.controls1),
									new DynamicPhysicsComponent(sf::FloatRect(coordinates.x, coordinates.y, width, height), 0.1),
									new HumanoidGraphicsComponent(Textures::Elf_Green),
									new HumanoidCombatComponent(150, 150, 40, 100, 2),
									new HumanoidSocialComponent("Player", "players")  );
			temp->setPlayer(true);
			break;

		case(Objects::Elf_Enemy):

			width = config.tileSize;
			height = config.tileSize;
			temp = new GameObject(	new BotActiveInputComponent(),
									new DynamicPhysicsComponent(sf::FloatRect(coordinates.x, coordinates.y, width, height), 0.05),
									new HumanoidGraphicsComponent(Textures::Elf_Red),
									new HumanoidCombatComponent(150, 150, 40, 130, 2),
									new HumanoidSocialComponent("Red Elf", "red_elves")  );
			temp->setPlayer(false);
			break;

		case(Objects::Elf_Friendly):

			width = config.tileSize;
			height = config.tileSize;
			temp = new GameObject(	new BotPassiveInputComponent(4),
									new DynamicPhysicsComponent(sf::FloatRect(coordinates.x, coordinates.y, width, height), 0.03),
									new HumanoidGraphicsComponent(Textures::Elf_Yellow),
									new HumanoidCombatComponent(150, 150, 40, 130, 2),
									new HumanoidSocialComponent("Yellow Elf", "yellow_elves")  );
			temp->setPlayer(false);
			break;

		case(Objects::Elf_Minion):

			width = config.tileSize;
			height = config.tileSize;	
			temp = new GameObject(	new KeyboardInputComponent(config.controls2),
									new DynamicPhysicsComponent(sf::FloatRect(coordinates.x, coordinates.y, width, height), 0.04),
									new HumanoidGraphicsComponent(Textures::Elf_Yellow),
									new HumanoidCombatComponent(150, 150, 40, 130, 2),
									new HumanoidSocialComponent("Minion", "players")  );
			temp->setPlayer(false);
			break;

		default:

			break;

	}

	sf::FloatRect objectRect = (*temp).getPhysics()->getRect();

	//Collision with objects.
	for(int i = 0; i < getGameObjects().size(); ++i)
		if((objectRect.intersects(getGameObjects()[i].getPhysics()->getRect())) && (temp != &getGameObjects()[i])) {
			canSpawn = false;
			break;
		}
	
	//Level map collision.
	try {
		for(int i = objectRect.top / config.tileSize; i < (objectRect.top + objectRect.height) / config.tileSize; ++i)
			for(int j = objectRect.left / config.tileSize; j < (objectRect.left + objectRect.width) / config.tileSize; ++j)
				if(getCollisionMap().at(i).at(j)) {
					canSpawn = false;
					break;
				}
	}
	catch(const std::out_of_range& e) {

		std::cout << "Can't spawn here! Out of map range!\n";
		return;

	}

	if(canSpawn) {
	
		getGameObjects().push_back(*temp);
		std::string name = (*temp).getSocial()->getName();
		std::cout << name.c_str() << " successfully spawned.\n";

	} else {

		std::cout << "Can't spawn here!\n";

	}


}

bool World::areEnemies(GameObject& object1, GameObject& object2) {

	std::string faction1 = object1.getSocial()->getFaction();
	std::string faction2 = object2.getSocial()->getFaction();

	if(faction1 == faction2)
		return false;
	else if(mFactionKarmaMap[faction1] == mFactionKarmaMap[faction2])
		return false;
	return true;

}

std::vector<std::vector<bool>>& World::getCollisionMap() {
	return mCollisionMap;
}

float World::getViewWidth() {
	return mViewWidth;
}

float World::getViewHeight() {
	return mViewHeight;
}

sf::Vector2i World::getMouseCoordinates() {
	return mMouseCoordinates;
}

std::map<std::string, bool>& World::getFactionKarmaMap() {
	return mFactionKarmaMap;
}
//
//==========================================
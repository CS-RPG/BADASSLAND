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
	mObjectMap.insert(std::make_pair("Player", Objects::Player));
	mObjectMap.insert(std::make_pair("Elf_Enemy", Objects::Elf_Enemy));
	mObjectMap.insert(std::make_pair("Elf_Minion", Objects::Elf_Minion));
	mObjectMap.insert(std::make_pair("Elf_Friendly", Objects::Elf_Friendly));

	mSpawnClock.restart();

	loadLevelMap(fileName);


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
	mTextPlayerCoordinates.setFont(gFont);
	mTextPlayerCoordinates.setString("");
	mTextPlayerCoordinates.setCharacterSize(gFontSize);
	mTextPlayerCoordinates.setStyle(sf::Text::Bold);
	mTextPlayerCoordinates.setColor(sf::Color::Color(125, 145, 176));
	mTextPlayerCoordinates.setPosition(0, gFontSize * 3);


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

void World::update(float deltaTime, sf::RenderWindow& window, sf::View& view, config& config) {
	
	//===============UPDATING GAME LOGIC===========
	for(int i = 0; i < getGameObjects().size(); ++i)	
		getGameObjects()[i].update(deltaTime, &config, *this);
	
	bool playerIsAlive = false;

	//Deleting objects marked for removal.
	for(int i = 0; i < getGameObjects().size(); ++i) {
		if(getGameObjects()[i].getCombat()->isMarkedForRemoval()) {

			getGameObjects().erase(getGameObjects().begin() + i);
			--i;
			//getGameObjects()[i].getInput()->setTargeting(false);

		} else if(getGameObjects()[i].isPlayer())
			playerIsAlive = true;

	}


	//============UPDATING VIEW====================
	sf::Vector2f viewPosition;
	sf::FloatRect screenCenter = getGameObjects()[mCenterObjectN].getPhysics()->getRect();

	viewPosition.x = screenCenter.left + screenCenter.width / 2 - mViewWidth / 2;
	viewPosition.y = screenCenter.top + screenCenter.height / 2 - mViewHeight / 2;
	//viewPosition.x = getGameObjects()[0].getPhysics()->getRect().left + config.tileSize / 2 - config.screenWidth / 2;
	//viewPosition.y = getGameObjects()[0].getPhysics()->getRect().top + config.tileSize / 2 - config.screenHeight / 2;
	
	if(viewPosition.x < 0)														viewPosition.x = 0;
	if(viewPosition.x > getMapWidth() * config.tileSize - mViewWidth)			viewPosition.x = getMapWidth() * config.tileSize - mViewWidth;
	if(viewPosition.y < 0)														viewPosition.y = 0;
	if(viewPosition.y > getMapHeight() * config.tileSize - mViewHeight)			viewPosition.y = getMapHeight() * config.tileSize - mViewHeight;

	//View at center when whole map fits on a screen.
	if(mViewWidth > getMapWidth() * config.tileSize)
		viewPosition.x = - (mViewWidth - getMapWidth() * config.tileSize) / 2;

	if(mViewHeight> getMapHeight() * config.tileSize)
		viewPosition.y = - (mViewHeight - getMapHeight() * config.tileSize) / 2;


	view.reset(sf::FloatRect(viewPosition.x, viewPosition.y, mViewWidth, mViewHeight));
	window.setView(view);


	std::cout << "View position: " << viewPosition.x << " " << viewPosition.y << '\n';


	//===============SPAWNING OBJECTS==============
	if((sf::Keyboard::isKeyPressed(sf::Keyboard::G)) && (mSpawnClock.getElapsedTime().asSeconds() > config.spawnDelay)) {

		sf::Vector2i coordinates;
		coordinates.x = viewPosition.x + sf::Mouse::getPosition(window).x / (config.screenWidth / mViewWidth);
		coordinates.y = viewPosition.y + sf::Mouse::getPosition(window).y / (config.screenWidth / mViewWidth);
		spawnObject(Objects::Elf_Enemy, coordinates, config);
		mSpawnClock.restart();

	}

	if((sf::Keyboard::isKeyPressed(sf::Keyboard::H)) && (mSpawnClock.getElapsedTime().asSeconds() > config.spawnDelay)) {

		sf::Vector2i coordinates;
		coordinates.x = viewPosition.x + sf::Mouse::getPosition(window).x / (config.screenWidth / mViewWidth);
		coordinates.y = viewPosition.y + sf::Mouse::getPosition(window).y / (config.screenWidth / mViewWidth);
		spawnObject(Objects::Elf_Friendly, coordinates, config);
		mSpawnClock.restart();

	}

	if((sf::Keyboard::isKeyPressed(sf::Keyboard::F)) && (mSpawnClock.getElapsedTime().asSeconds() > config.spawnDelay)) {

		sf::Vector2i coordinates;
		coordinates.x = viewPosition.x + sf::Mouse::getPosition(window).x / (config.screenWidth / mViewWidth);
		coordinates.y = viewPosition.y + sf::Mouse::getPosition(window).y / (config.screenWidth / mViewWidth);
		spawnObject(Objects::Elf_Minion, coordinates, config);
		mSpawnClock.restart();

	}
	

	//===============FOCUS OBJECT CHANGING=========
	if(sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
		for(int i = 0; i < getGameObjects().size(); ++i) {

			sf::FloatRect rect;
			rect.left = viewPosition.x + sf::Mouse::getPosition(window).x / (config.screenWidth / mViewWidth);
			rect.top = viewPosition.y + sf::Mouse::getPosition(window).y / (config.screenWidth / mViewWidth);
			rect.width = 1;
			rect.height = 1;
			
			if(rect.intersects(getGameObjects()[i].getPhysics()->getRect())) {
				mCenterObjectN = i;
				break;
			}

		}

	}


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


	//===============UPDATING HUD==================
	std::ostringstream hudHealth;
	//std::ostringstream hudMana;
	std::ostringstream hudEnemyCount;
	std::ostringstream hudPlayerCoordinates;
	std::ostringstream hudMouseCoordinates;
	std::ostringstream hudOutConsole;

	hudHealth << getGameObjects()[0].getCombat()->getHP();
	//hudMana << getGameObjects()[0].getCombat()->getMP();
	hudEnemyCount << "Number of game objects: " << getGameObjects().size();
	hudPlayerCoordinates << "X: " << getGameObjects()[0].getPhysics()->getRect().left + config.tileSize / 2 << '\n'
						 << "Y: " << getGameObjects()[0].getPhysics()->getRect().top + config.tileSize / 2;
	hudMouseCoordinates << "X: " << sf::Mouse::getPosition(window).x << '\n'
						<< "Y: " << sf::Mouse::getPosition(window).y;
	if(!playerIsAlive) {
		hudOutConsole << "Player is dead!\n";
	}

	//mTextMana.setString(hudMana.str());
	mTextHealth.setString(hudHealth.str());
	mTextEnemyCount.setString(hudEnemyCount.str());
	mTextPlayerCoordinates.setString(hudPlayerCoordinates.str());
	mTextMouseCoordinates.setString(hudMouseCoordinates.str());
	mOutConsole.setString(hudOutConsole.str());

	mTextHealth.setPosition(viewPosition.x, viewPosition.y);
	//textMana.setPosition(mViewPosition.x, mViewPosition.y + textMana.getCharacterSize());
	mTextEnemyCount.setPosition(viewPosition.x, viewPosition.y + mTextEnemyCount.getCharacterSize() * 2);
	mTextPlayerCoordinates.setPosition(viewPosition.x, viewPosition.y + mTextPlayerCoordinates.getCharacterSize() * 3);
	mTextMouseCoordinates.setPosition(viewPosition.x, viewPosition.y + mTextPlayerCoordinates.getCharacterSize() * 6);
	mOutConsole.setPosition(viewPosition.x, viewPosition.y + mTextPlayerCoordinates.getCharacterSize() * 9);

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
	window.draw(mTextPlayerCoordinates);
	window.draw(mTextMouseCoordinates);
	window.draw(mOutConsole);
	
}

void World::resolveMapCollision(GameObject* object, int direction, int tileSize) {

	sf::FloatRect rect = object->getPhysics()->getRect();
	sf::Vector2f movement = object->getPhysics()->getMovement();

	for(int i = rect.top / tileSize; i < (rect.top + rect.height) / tileSize; ++i)
		for(int j = rect.left / tileSize; j < (rect.left + rect.width) / tileSize; ++j) {

			try {
				if(getLevelMap().at(i).at(j) == 'B') {

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

void World::loadLevelMap(std::string filename) {

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

	//Loading level data.
	inputFile.get();
	for(int i = 0; i < getMapHeight(); ++i) {
		for(int j = 0; j < getMapWidth(); ++j) {
			char temp = inputFile.get();
			getLevelMap()[i][j] = int(temp);
		}
		inputFile.get();
	}

	//Loading spawn data.
	inputFile >> objectCount;
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
									new HumanoidCombatComponent(150, 150, 40, 40, 2),
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
				if(getLevelMap().at(i).at(j) == 'B') {
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
//
//==========================================
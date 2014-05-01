//World.cpp
#include <World.hpp>


void outputMap(std::vector<std::vector<int>> map);
void outputPath(std::vector<sf::Vector2i>& path);

extern sf::Font				gFont;
extern int					gFontSize;

extern float				gMapCollisionAccuracy;
extern float				gMaxZoomRate;
extern float				gGridThickness;

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
	mTextMouseCoordinates.setPosition(0, gFontSize * 7);


	//===============HUD OUT CONSOLE===============
	mOutConsole.setFont(gFont);
	mOutConsole.setString("");
	mOutConsole.setCharacterSize(gFontSize);
	mOutConsole.setStyle(sf::Text::Bold);
	mOutConsole.setColor(sf::Color::Color(sf::Color::Red));
	mOutConsole.setPosition(0, gFontSize * 10);


	//===============TILE OBJECT===================
	mTile = sf::RectangleShape(sf::Vector2f(config.tileSize, config.tileSize));


	//===============PATH HIGHLIGHT================
	mPathTile = sf::RectangleShape(sf::Vector2f(config.tileSize, config.tileSize));
	mPathTile.setFillColor(sf::Color::Green);
	mPathHighlight = false;


	//===============GRID==========================
	mVerticalLine = sf::RectangleShape(sf::Vector2f(mViewWidth, gGridThickness));
	mHorizontalLine = sf::RectangleShape(sf::Vector2f(gGridThickness, mViewHeight));
	
	mVerticalLine.setFillColor(sf::Color::Black);
	mHorizontalLine.setFillColor(sf::Color::Black);

	mGridActive = false;
	

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

void World::onCreate() {

	for(int i = 0; i < getSharedObjects().size(); ++i) {
		
		
		getGameObjects().push_back(getSharedObjects().back());
		getSharedObjects().pop_back();
	}

}

void World::onExit() {

	for(int i = 0; i < getGameObjects().size(); ++i)
		if(getGameObjects()[i].isPlayer())
			getSharedObjects().push_back(getGameObjects()[i]);

}

void World::update(float deltaTime, sf::RenderWindow& window, sf::View& view, config& config) {
	
	if(mTerminateGame)
		window.close();

	//sf::Vector2i windowPosition = window.getPosition();
	//std::cout << windowPosition.x << " " << windowPosition.y << '\n';

	//===============UPDATING GAME LOGIC===========
	for(int i = 0; i < getGameObjects().size(); ++i)
		getGameObjects()[i].update(deltaTime, &config, *this);
	
	bool playerIsAlive = false;

	//Deleting objects marked for removal.
	for(int i = 0; i < getGameObjects().size(); ++i) {

		if(getGameObjects()[i].getCombat()->isMarkedForRemoval()) {

			if(i == mCenterObjectN && i != 0)
				--mCenterObjectN;

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
		mCenterObjectN = 0;
		screenCenter.left = mViewWidth / 2;
		screenCenter.top = mViewHeight / 2;
	}

	viewPosition.x = screenCenter.left + screenCenter.width / 2 - mViewWidth / 2;
	viewPosition.y = screenCenter.top + screenCenter.height / 2 - mViewHeight / 2;
	//viewPosition.x = getGameObjects()[0].getPhysics()->getRect().left + config.tileSize / 2 - config.screenWidth / 2;
	//viewPosition.y = getGameObjects()[0].getPhysics()->getRect().top + config.tileSize / 2 - config.screenHeight / 2;
	
	//If focus object is within level borders, do the View optimization stuff.
	//If not, just focus on the object.
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

	//Updating grid.
	mVerticalLine.setSize(sf::Vector2f(getMapWidth() * config.tileSize, gGridThickness * mViewWidth / config.screenWidth));
	mHorizontalLine.setSize(sf::Vector2f(gGridThickness * mViewHeight / config.screenHeight, getMapHeight() * config.tileSize));

	//std::cout << "View position: " << viewPosition.x << " " << viewPosition.y << '\n';


	//===============HANDLING INPUT================
	handleInput(config);


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
		hudObjectCoordinates	<< std::string(getGameObjects()[mCenterObjectN].getSocial()->getName()) << ":\n"
								<< "X: " << getGameObjects()[mCenterObjectN].getPhysics()->getRect().left << '\n'
								<< "Y: " << getGameObjects()[mCenterObjectN].getPhysics()->getRect().top;

	}

	hudEnemyCount << "Number of game objects: " << getGameObjects().size();
	hudMouseCoordinates << "X: " << sf::Mouse::getPosition(window).x << ' '	<< "Y: " << sf::Mouse::getPosition(window).y << '\n'
						<< "X: " << getMouseCoordinates().x << ' '	<< "Y: " << getMouseCoordinates().y;

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
	mTextMouseCoordinates.setPosition(viewPosition.x, viewPosition.y + gFontSize * 7);
	mOutConsole.setPosition(viewPosition.x, viewPosition.y + gFontSize * 10);

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

		}
	}

	//Path highlight.
	if(mPathHighlight) {

		for(int i = 0; i < getGameObjects()[mCenterObjectN].getInput()->getPath().size(); ++i) {

			sf::Vector2i tilePosition = getGameObjects()[mCenterObjectN].getInput()->getPath()[i];
			mPathTile.setPosition(tilePosition.x * config.tileSize, tilePosition.y * config.tileSize);
			window.draw(mPathTile);

		}

	}

	//Rendering grid.
	if(mGridActive) {

		for(int i = 0; i < getMapHeight(); ++i)
			for(int j = 0; j < getMapWidth(); ++j) {
			
				mHorizontalLine.setPosition(config.tileSize * j, 0);
				mVerticalLine.setPosition(0, config.tileSize * i);

				window.draw(mHorizontalLine);
				window.draw(mVerticalLine);

			}

	}

	//Rendering objects.
	for(int i = 0; i < getGameObjects().size(); ++i) 
		getGameObjects()[i].getGraphics()->draw(window);

	/*
	//Reconfiguring view for HUD rendering.
	sf::FloatRect viewRect;
	viewRect.left = view.getCenter().x - mViewWidth / 2;
	viewRect.top = view.getCenter().y - mViewHeight / 2;
	viewRect.width = config.screenWidth;
	viewRect.height = config.screenHeight;

	view.reset(viewRect);
	window.setView(view);
	*/

	//Rendering HUD.
	window.draw(mTextHealth);
	//window.draw(mTextMana);
	window.draw(mTextEnemyCount);
	window.draw(mTextObjectCoordinates);
	window.draw(mTextMouseCoordinates);
	window.draw(mOutConsole);
	
}

void World::handleInput(config& config) {

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

	//===============GODMODE CHEAT=================
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

	//Deleting objects.
	if((sf::Keyboard::isKeyPressed(sf::Keyboard::Delete)) && (mSpawnClock.getElapsedTime().asSeconds() > config.spawnDelay)) {

		for(int i = 0; i < getGameObjects().size(); ++i) {

			sf::FloatRect rect;
			rect.left = getMouseCoordinates().x;
			rect.top = getMouseCoordinates().y;
			rect.width = 1;
			rect.height = 1;
			
			if(rect.intersects(getGameObjects()[i].getPhysics()->getRect())) {
				getGameObjects()[i].getCombat()->setMarkedForRemoval(true);
				break;
			}

		}

		mSpawnClock.restart();

	}


	//Pathfinding.
	if((sf::Keyboard::isKeyPressed(sf::Keyboard::M)) && (mSpawnClock.getElapsedTime().asSeconds() > config.spawnDelay)) {

		sf::Vector2i destination;
		destination.x = getMouseCoordinates().x / config.tileSize;
		destination.y = getMouseCoordinates().y / config.tileSize;

		sf::Vector2i source;
		source.x = getGameObjects()[mCenterObjectN].getPhysics()->getRect().left / config.tileSize;
		source.y = getGameObjects()[mCenterObjectN].getPhysics()->getRect().top / config.tileSize;

		std::vector<sf::Vector2i> path;
		wavePathFind(source, destination, path);
		outputPath(path);

		mSpawnClock.restart();

	}


	//Grid ON / OFF.
	if((sf::Keyboard::isKeyPressed(sf::Keyboard::L)) && (mSpawnClock.getElapsedTime().asSeconds() > config.spawnDelay)) {

		mGridActive ^= 1;
		mSpawnClock.restart();

	}


	//Path highlight ON / OFF.
	if((sf::Keyboard::isKeyPressed(sf::Keyboard::B)) && (mSpawnClock.getElapsedTime().asSeconds() > config.spawnDelay)) {

		mPathHighlight ^= 1;
		mSpawnClock.restart();

	}

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

					//Right.
					if((movement.x > 0) && (direction == 0)) {
						
						if(int(rect.top) % tileSize <= gMapCollisionAccuracy * tileSize/* && object->isPlayer()*/) {

							rect.top = ceil(rect.top);
							rect.top -= int(rect.top) % 10;

						}
						
						//rect.left = (int(rect.left) / tileSize) * tileSize;
						rect.left = j * tileSize - rect.width;
						//rect.left -= int(rect.left) % tileSize;
						object->getInput()->setBadDirection(2);

					}

					//Left.
					if((movement.x < 0) && (direction == 0)) {
						
						if(int(rect.top) % tileSize <= gMapCollisionAccuracy * tileSize/* && object->isPlayer()*/) {

							rect.top = ceil(rect.top);
							rect.top -= int(rect.top) % 10;

						}
						
						//rect.left = (int(rect.left + rect.width) / tileSize) * tileSize;
						rect.left = j * tileSize + tileSize;
						//rect.left -= int(rect.left) % tileSize - rect.width;
						object->getInput()->setBadDirection(4);

					}

					//Down.
					if((movement.y > 0) && (direction == 1)) {
						
						if(int(rect.left) % tileSize <= gMapCollisionAccuracy * tileSize/* && object->isPlayer()*/) {

							rect.left = ceil(rect.left);
							rect.left -= int(rect.left) % 10;

						}
						
						//rect.top = (int(rect.top) / tileSize) * tileSize;
						rect.top = i * tileSize - rect.height;
						//rect.top -= int(rect.top) % tileSize;
						object->getInput()->setBadDirection(3);

					}

					//Up.
					if((movement.y < 0) && (direction == 1)) {
						
						if(int(rect.left) % tileSize <= gMapCollisionAccuracy * tileSize/* && object->isPlayer()*/) {

							rect.left = ceil(rect.left);
							rect.left -= int(rect.left) % 10;

						}
						
						//rect.top = (int(rect.top + rect.height) / tileSize) * tileSize;
						rect.top = i * tileSize + tileSize;	
						//rect.top -= int(rect.top) % tileSize - rect.height;
						object->getInput()->setBadDirection(1);

					}

				}
			}
			catch(const std::out_of_range& e) {

				object->getSocial()->setName("GOD");
				object->getSocial()->setFaction("gods");
				//object->setPhysics(new NoClipPhysicsComponent(rect, 1));
				//object->setCombat(new InvincibleCombatComponent(9999, 9999, 9999, 9999, 9999));
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
		if(rect.intersects(currentRect) && object != &getGameObjects()[i] && !getGameObjects()[i].isNoClip()) {

			if((movement.x > 0) && (direction == 0)) {rect.left -= (rect.left + rect.width) - currentRect.left;			object->getInput()->setBadDirection(2);}
			if((movement.x < 0) && (direction == 0)) {rect.left += (currentRect.left + currentRect.width) - rect.left;	object->getInput()->setBadDirection(4);}
			if((movement.y > 0) && (direction == 1)) {rect.top -= (rect.top + rect.height) - currentRect.top;			object->getInput()->setBadDirection(3);}
			if((movement.y < 0) && (direction == 1)) {rect.top += (currentRect.top + currentRect.height) - rect.top;	object->getInput()->setBadDirection(1);}

		}
		
	}

	if(rect.left < -120 || rect.left > 120 * 40 || rect.top > 12 * 120 || rect.top < -120) {
		std::cout << "ObjectCollision Error\n";
	}

	object->getPhysics()->setRect(rect);

}

std::vector<sf::Vector2i> World::getAdjacentTiles(sf::Vector2i tile) {

	std::vector<sf::Vector2i> result;

	if (tile.x - 1 >= 0)
		result.push_back(sf::Vector2i(tile.x - 1, tile.y));

	if (tile.x + 1 <= getMapWidth())
		result.push_back(sf::Vector2i(tile.x + 1, tile.y));

	if (tile.y - 1 >= 0)
		result.push_back(sf::Vector2i(tile.x, tile.y - 1));

	if (tile.y + 1 <= getMapHeight())
		result.push_back(sf::Vector2i(tile.x, tile.y + 1));

	return result;

}

bool World::wavePathFind(sf::Vector2i source, sf::Vector2i destination, std::vector<sf::Vector2i>& path) {

	//If source / destination is a wall tile.
	if (getCollisionMap()[source.y][source.x] || getCollisionMap()[destination.y][destination.x]) {
		std::cout << "WALL\n";
		return false;
	}


	int mapHeight = getMapHeight();
	int mapWidth = getMapWidth();


	//Creating work space.
	std::vector<std::vector<int>> map;
	map.resize(mapHeight);
	for (int i = 0; i < mapHeight; ++i)
		map[i].resize(mapWidth);

	for (int i = 0; i < mapHeight; ++i)
		for (int j = 0; j < mapWidth; ++j)
			if(getCollisionMap()[i][j])
				map[i][j] = 1;
			else
				map[i][j] = 0;

	//Wave starting value.
	int wave = 2;

	bool stop;

	//Marking starting point.
	map[source.y][source.x] = wave;

	for (bool stop = false; !stop && map[destination.y][destination.x] == 0; ++wave) {

		//Current step visualization.
		//std::cout << "\nStep: " << wave - 1 << '\n';
		//outputMap(map);

		stop = true;

		for (int i = 0; i < mapHeight; ++i)
			for (int j = 0; j < mapWidth; ++j)
				if (map[i][j] == wave) {

					//Marking all adjacent non-marked tiles with (wave + 1).
					std::vector<sf::Vector2i> tiles = getAdjacentTiles(sf::Vector2i(j, i));
					for (int k = 0; k < tiles.size(); ++k)
						if (map[tiles[k].y][tiles[k].x] == 0) {
							map[tiles[k].y][tiles[k].x] = wave + 1;
							stop = false;
						}

				}

	}

	if (map[destination.y][destination.x] == 0)
		return false;

	//Constructing path.
	path.clear();
	path.resize(wave - 1);
	
	sf::Vector2i currentTile = destination;
	path[wave - 2] = currentTile;


	for (; wave > 2;) {

		path[wave - 2] = currentTile;
		--wave;

		std::vector<sf::Vector2i> tiles = getAdjacentTiles(currentTile);
		for (int i = 0; i < tiles.size(); ++i)
			if (map[tiles[i].y][tiles[i].x] == wave)
				currentTile = tiles[i];

	}
	path[0] = sf::Vector2i(source.x, source.y);
	
	path.erase(path.begin());

	//outputPath(path);

	return true;

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
									new HumanoidCombatComponent(150, 150, 40, 180, 2),
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
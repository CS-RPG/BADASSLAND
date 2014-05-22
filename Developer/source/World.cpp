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


World::World(std::string fileName, config& config, StateMachine* game) {

	setStateMachine(game);

	//===============INITIALIZATION================
	initializeObjectMap();
	initializeFactionKarmaMap();
	initializeScripts();

	mConfig = getStateMachine()->getConfig();

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
	gTextureHolder.load("HP_Bar");
	//gTextureHolder.load(Textures::TileSet, "./textures/testTileSet.png");

	//sf::Sprite tile(tileSet);


	//===============LOADING OBJECTS===============
	loadObject("Elf_Yellow");
	loadObject("Elf_Red");
	loadObject("Elf_Minion");
	loadObject("Player");

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
	

	//===============SPAWNING OBJECTS==============
	//If player exists, last player, written in level-file, will be the view center.
	//If not (and there are any objects), first GameObject will be the view center.
	mCenterObjectN = 0;
	mIsPlayerAlive = false;
	getGameObjects().reserve(objectCount);	//Reserving memory for objects. (???)
	for(int i = 0; i < objectCount; ++i)
		spawnObject(objects[i].objectID, sf::Vector2i(objects[i].x, objects[i].y), config);

	for(int i = 0; i < getGameObjects().size(); ++i) {
		if(getGameObjects()[i].isPlayer()) {
			mIsPlayerAlive = true;
			mCenterObjectN = i;
			std::cout << "Player exists.\n";
		}
	}

	if(!mIsPlayerAlive) 
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

	//===============GOOD==========================
	mFactionKarmaMap.insert(std::make_pair("players", true));
	mFactionKarmaMap.insert(std::make_pair("gods", true));
	mFactionKarmaMap.insert(std::make_pair("yellow_elves", true));

	//===============EVIL==========================
	mFactionKarmaMap.insert(std::make_pair("devils", false));
	mFactionKarmaMap.insert(std::make_pair("red_elves", false));

}

void World::initializeScripts() {

	getScripts().insert(std::make_pair("changeState", &State::changeState));

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

//============UPDATE========================
void World::update(float deltaTime, sf::RenderWindow& window, sf::View& view, config& config) {
	
	if(mTerminateGame)
		window.close();

	updateObjects(deltaTime, config);
	updateView(window, view, config);
	updateMouseCoordinates(window, config, mViewPosition);
	updateGrid(config);
	updateHUD(window);

	handleInput(config);

}

void World::updateObjects(float deltaTime, config& config) {

	myGameObjectIter current = getGameObjects().begin();
	myGameObjectConstIter end = getGameObjects().end();

	for(; current != end; ++current)
		(*current).update(deltaTime, &config, *this);

	//Deleting objects marked for removal.
	for(int i = 0; i < getGameObjects().size(); ++i) {

		if(getGameObjects()[i].getCombat()->isMarkedForRemoval()) {

			if(getGameObjects()[i].isPlayer())
				mIsPlayerAlive = false;

			if(i == mCenterObjectN && i != 0)
				--mCenterObjectN;

			getGameObjects()[i].getCombat()->update(getGameObjects()[i], *this);

			getGameObjects().erase(getGameObjects().begin() + i);
			--i;

		} 

	}

}

void World::updateView(sf::RenderWindow& window, sf::View& view, config& config) {

	if(getGameObjects().size() != 0)	
		mScreenCenter = getGameObjects().at(mCenterObjectN).getPhysics()->getRect();
	else {
		mCenterObjectN = 0;
		mScreenCenter.left = mViewWidth / 2;
		mScreenCenter.top = mViewHeight / 2;
	}

	mViewPosition.x = mScreenCenter.left + mScreenCenter.width / 2 - mViewWidth / 2;
	mViewPosition.y = mScreenCenter.top + mScreenCenter.height / 2 - mViewHeight / 2;
	
	//If focus object is within level borders, do the View optimization stuff.
	//If not, just focus on the object.
	if(mScreenCenter.left >= 0 && mScreenCenter.left <= (getMapWidth() - 1) * config.tileSize &&
			mScreenCenter.top >= 0 && mScreenCenter.top <= (getMapHeight() - 1) * config.tileSize ||
			getGameObjects().size() == 0) {
	
		if(mViewPosition.x < 0)													mViewPosition.x = 0;
		if(mViewPosition.x > getMapWidth() * config.tileSize - mViewWidth)		mViewPosition.x = getMapWidth() * config.tileSize - mViewWidth;
		if(mViewPosition.y < 0)													mViewPosition.y = 0;
		if(mViewPosition.y > getMapHeight() * config.tileSize - mViewHeight)	mViewPosition.y = getMapHeight() * config.tileSize - mViewHeight;

		//View at center when whole map fits on a screen.
		if(mViewWidth > getMapWidth() * config.tileSize)
			mViewPosition.x = - (mViewWidth - getMapWidth() * config.tileSize) / 2;

		if(mViewHeight> getMapHeight() * config.tileSize)
			mViewPosition.y = - (mViewHeight - getMapHeight() * config.tileSize) / 2;
	
	}
	
	view.reset(sf::FloatRect(mViewPosition.x, mViewPosition.y, mViewWidth, mViewHeight));
	window.setView(view);

}

void World::updateMouseCoordinates(sf::RenderWindow& window, config& config, sf::Vector2f viewPosition) {

	mMouseCoordinates.x = viewPosition.x + sf::Mouse::getPosition(window).x / (config.screenWidth / mViewWidth);
	mMouseCoordinates.y = viewPosition.y + sf::Mouse::getPosition(window).y / (config.screenWidth / mViewWidth);

}

void World::updateGrid(config& config) {

	mVerticalLine.setSize(sf::Vector2f(getMapWidth() * config.tileSize, gGridThickness * mViewWidth / config.screenWidth));
	mHorizontalLine.setSize(sf::Vector2f(gGridThickness * mViewHeight / config.screenHeight, getMapHeight() * config.tileSize));

}

void World::updateHUD(sf::RenderWindow& window) {

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

	if(!mIsPlayerAlive)
		hudOutConsole << "Player is dead!\n";

	mTextHealth.setString(hudHealth.str());
	//mTextMana.setString(hudMana.str());
	mTextEnemyCount.setString(hudEnemyCount.str());
	mTextObjectCoordinates.setString(hudObjectCoordinates.str());
	mTextMouseCoordinates.setString(hudMouseCoordinates.str());
	mOutConsole.setString(hudOutConsole.str());

	mTextHealth.setPosition(mViewPosition.x, mViewPosition.y);
	//textMana.setPosition(mViewPosition.x, mViewPosition.y + gFontSize);
	mTextEnemyCount.setPosition(mViewPosition.x, mViewPosition.y + gFontSize * 2);
	mTextObjectCoordinates.setPosition(mViewPosition.x, mViewPosition.y + gFontSize * 3);
	mTextMouseCoordinates.setPosition(mViewPosition.x, mViewPosition.y + gFontSize * 7);
	mOutConsole.setPosition(mViewPosition.x, mViewPosition.y + gFontSize * 10);

}

//============INPUT=========================
void World::handleInput(config& config) {

	//===============ZOOM==========================
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::PageDown) && mSpawnClock.getElapsedTime().asSeconds() > config.spawnDelay) {
		
		zoomOut();
		mSpawnClock.restart();

	}
	
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::PageUp) && mSpawnClock.getElapsedTime().asSeconds() > config.spawnDelay) {

		zoomIn();
		mSpawnClock.restart();

	}

	//===============SPAWNING OBJECTS==============
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::G) && mSpawnClock.getElapsedTime().asSeconds() > config.spawnDelay) {

		sf::Vector2i coordinates;
		coordinates.x = getMouseCoordinates().x;
		coordinates.y = getMouseCoordinates().y;
		spawnObject("Elf_Red", coordinates, config);
		mSpawnClock.restart();

	}

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::H) && mSpawnClock.getElapsedTime().asSeconds() > config.spawnDelay) {

		sf::Vector2i coordinates;
		coordinates.x = getMouseCoordinates().x;
		coordinates.y = getMouseCoordinates().y;
		spawnObject("Elf_Yellow", coordinates, config);
		mSpawnClock.restart();

	}

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::F) && mSpawnClock.getElapsedTime().asSeconds() > config.spawnDelay) {

		sf::Vector2i coordinates;
		coordinates.x = getMouseCoordinates().x;
		coordinates.y = getMouseCoordinates().y;
		spawnObject("Elf_Minion", coordinates, config);
		mSpawnClock.restart();

	}

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::P) && mSpawnClock.getElapsedTime().asSeconds() > config.spawnDelay) {

		sf::Vector2i coordinates;
		coordinates.x = getMouseCoordinates().x;
		coordinates.y = getMouseCoordinates().y;
		spawnObject("Player", coordinates, config);
		mSpawnClock.restart();

	}

	//===============GODMODE CHEAT=================
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Tilde) && mSpawnClock.getElapsedTime().asSeconds() > config.spawnDelay && getGameObjects().size() != 0) {
	
		sf::FloatRect rect = getGameObjects()[mCenterObjectN].getPhysics()->getRect();
		
		getGameObjects()[mCenterObjectN].getSocial()->setName("GOD");
		getGameObjects()[mCenterObjectN].getSocial()->setFaction("gods");

		getGameObjects()[mCenterObjectN].setInput(new KeyboardInputComponent(config.controls1));
		getGameObjects()[mCenterObjectN].setPhysics(new NoClipPhysicsComponent(rect, 0.1));
		getGameObjects()[mCenterObjectN].setCombat(new InvincibleCombatComponent(9999, 9999, 9999, 9999, 9999));

		mSpawnClock.restart();						

	}

	//===============FOCUS OBJECT CHANGING=========
	if(sf::Mouse::isButtonPressed(sf::Mouse::Right)) {

		sf::FloatRect rect;
		rect.left = getMouseCoordinates().x;
		rect.top = getMouseCoordinates().y;
		rect.width = 1;
		rect.height = 1;

		for(int i = 0; i < getGameObjects().size(); ++i) {

			if(rect.intersects(getGameObjects()[i].getPhysics()->getRect())) {
				mCenterObjectN = i;
				break;
			}

		}

	}

	//Deleting objects.
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Delete) && mSpawnClock.getElapsedTime().asSeconds() > config.spawnDelay) {
	
		sf::FloatRect rect;
		rect.left = getMouseCoordinates().x;
		rect.top = getMouseCoordinates().y;
		rect.width = 1;
		rect.height = 1;		

		myGameObjectIter current = getGameObjects().begin();
		myGameObjectConstIter end = getGameObjects().end();

		for(; current != end; ++current) {

			if(rect.intersects(current->getPhysics()->getRect())) {
				current->getCombat()->setMarkedForRemoval(true);
				break;
			}

		}

		mSpawnClock.restart();

	}

	//Pathfinding.
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::M) && mSpawnClock.getElapsedTime().asSeconds() > config.spawnDelay && getGameObjects().size() != 0) {

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
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::L) && mSpawnClock.getElapsedTime().asSeconds() > config.spawnDelay) {

		mGridActive ^= 1;
		mSpawnClock.restart();

	}


	//Path highlight ON / OFF.
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::B) && mSpawnClock.getElapsedTime().asSeconds() > config.spawnDelay) {

		mPathHighlight ^= 1;
		mSpawnClock.restart();

	}

	//Main menu.
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) && getButtonClock().getElapsedTime().asSeconds() > config.spawnDelay) {
		
		getStateMachine()->changeState("MainMenu ");
		getButtonClock().restart();

	}

}

//============SCRIPTS=======================
void World::zoomIn() {

	if(mViewWidth / mConfig.screenWidth >= 1 / gMaxZoomRate) {
			mViewWidth /= mConfig.zoomRate;
			mViewHeight /= mConfig.zoomRate;
	}

}

void World::zoomOut() {

	if(mViewWidth / mConfig.screenWidth <= gMaxZoomRate) {
		mViewWidth *= mConfig.zoomRate;
		mViewHeight *= mConfig.zoomRate;
	}

}

//============RENDER========================
void World::render(sf::RenderWindow& window, sf::View& view, config& config) {

	renderMap(window, view, config);
	renderObjects(window, view, config);
	renderHUD(window, view, config);
	
}

void World::renderMap(sf::RenderWindow& window, sf::View& view, config& config) {

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

		for(int i = 0; i <= getMapWidth(); ++i) {
			mHorizontalLine.setPosition(config.tileSize * i, 0);
			window.draw(mHorizontalLine);
		}
				
		for(int i = 0; i <= getMapHeight(); ++i) {
			mVerticalLine.setPosition(0, config.tileSize * i);
			window.draw(mVerticalLine);
		}

	}

}

void World::renderObjects(sf::RenderWindow& window, sf::View& view, config& config) {

	myGameObjectIter current = getGameObjects().begin();
	myGameObjectConstIter end = getGameObjects().end();

	for(; current != end; ++current)
		current->getGraphics()->draw(window);

}

void World::renderHUD(sf::RenderWindow& window, sf::View& view, config& config) {

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

	window.draw(mTextHealth);
	//window.draw(mTextMana);
	window.draw(mTextEnemyCount);
	window.draw(mTextObjectCoordinates);
	window.draw(mTextMouseCoordinates);
	window.draw(mOutConsole);

}

//============OTHER=========================
void World::resolveMapCollision(GameObject* object, int direction, int tileSize) {

	sf::FloatRect rect = object->getPhysics()->getRect();
	sf::Vector2f movement = object->getPhysics()->getMovement();					

	//If out of range map.
	if(rect.left < 0) {

		object->getInput()->setBadDirection(4);
		for(int i = 0; i < getMapWidth(); ++i)
			if(!getCollisionMap().at(rect.top / tileSize).at(i)) {
				rect.left = i * tileSize;
				break;
			}

	} else if(rect.left + rect.width > getMapWidth() * tileSize) {
		
		object->getInput()->setBadDirection(2);
		for(int i = getMapWidth() - 1; i > 0; --i)
			if(!getCollisionMap().at(rect.top / tileSize).at(i)) {
				rect.left = i * tileSize;
				break;
			}

	}

	if(rect.top < 0) {

		object->getInput()->setBadDirection(1);
		for(int i = 0; i < getMapHeight(); ++i)
			if(!getCollisionMap().at(i).at(rect.left / tileSize)) {
				rect.top = i * tileSize;
				break;
			}

	} else if(rect.top + rect.height > getMapHeight() * tileSize) {
		
		object->getInput()->setBadDirection(3);
		for(int i = getMapHeight() - 1; i > 0; --i)
			if(!getCollisionMap().at(i).at(rect.left / tileSize)) {
				rect.top = i * tileSize;
				break;
			}

	}

	//Collision map.
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

	myGameObjectIter current = getGameObjects().begin();
	myGameObjectConstIter end = getGameObjects().end();

	for(; current != end; ++current) {
		
		sf::FloatRect currentRect = current->getPhysics()->getRect();
		if(rect.intersects(currentRect) && object != &(*current) && !(current->isNoClip())) {

			if((movement.x > 0) && (direction == 0)) {rect.left = currentRect.left - rect.width;			object->getInput()->setBadDirection(2); /*current->getInput()->setBadDirection(4);*/}
			if((movement.x < 0) && (direction == 0)) {rect.left = currentRect.left + currentRect.width;		object->getInput()->setBadDirection(4); /*current->getInput()->setBadDirection(2);*/}
			if((movement.y > 0) && (direction == 1)) {rect.top = currentRect.top - rect.height;				object->getInput()->setBadDirection(3); /*current->getInput()->setBadDirection(1);*/}
			if((movement.y < 0) && (direction == 1)) {rect.top = currentRect.top + currentRect.height;		object->getInput()->setBadDirection(1); /*current->getInput()->setBadDirection(3);*/}

		}
		
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

	//path[0] = sf::Vector2i(source.x, source.y);
	
	//Deleting first node.
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
		inputFile.close();
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
				inputFile.close();
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
		inputFile.close();
		return false;
	}

	std::cout << "Objects to spawn: " << objectCount << '\n';

	getGameObjects().reserve(objectCount);

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

bool World::loadObject(std::string objectID) {

	std::ifstream inputFile("./Objects/" + objectID + ".txt");

	if(!inputFile.good()) {
		std::cout << "Object file doesn't exist!\n";
		inputFile.close();
		return false;
	}

	//Temp buffer.
	std::string temp;

	//InputComponent.
	getline(inputFile, temp);	//SKIP LINE.
	objectInput inputSettings;
	inputFile >> inputSettings;
	mObjectInput.insert(std::make_pair(objectID, inputSettings));
	inputFile.get();			//SKIP LINE.
	getline(inputFile, temp);	//SKIP LINE.

	//Physics.
	objectPhysics physicsSettings;
	inputFile >> physicsSettings;
	mObjectPhysics.insert(std::make_pair(objectID, physicsSettings));
	inputFile.get();			//SKIP LINE.
	getline(inputFile, temp);	//SKIP LINE.

	//Graphics.
	objectGraphics graphicsSettings;
	inputFile >> graphicsSettings;
	gTextureHolder.load(graphicsSettings.textureID);
	mObjectGraphics.insert(std::make_pair(objectID, graphicsSettings));
	inputFile.get();			//SKIP LINE.
	getline(inputFile, temp);	//SKIP LINE.

	//Combat.
	objectCombat combatSettings;
	inputFile >> combatSettings;
	mObjectCombat.insert(std::make_pair(objectID, combatSettings));
	inputFile.get();			//SKIP LINE.
	getline(inputFile, temp);	//SKIP LINE.

	//Social.
	objectSocial socialSettings;
	inputFile >> socialSettings;
	mObjectSocial.insert(std::make_pair(objectID, socialSettings));


	std::cout << socialSettings.componentType.c_str() << '\n';
	std::cout << temp.c_str() << '\n';

	inputFile.close();

	return true;

}

void World::spawnObject(std::string objectID, sf::Vector2i coordinates, config& config) {

	GameObject* temp;
	bool canSpawn = true;
	int width;
	int height;

	InputComponent* input = NULL;
	PhysicsComponent* physics = NULL;
	GraphicsComponent* graphics = NULL;
	CombatComponent* combat = NULL;
	SocialComponent* social = NULL;

	//Input.
	if(mObjectInput[objectID].componentType == "passiveAI")
		input = new BotPassiveInputComponent(4);
	else if(mObjectInput[objectID].componentType == "activeAI")
		input = new BotActiveInputComponent();
	else if(mObjectInput[objectID].componentType == "keyboard") {
		
		if(mObjectInput[objectID].controlType == 1)
			input = new KeyboardInputComponent(config.controls1);
		else if(mObjectInput[objectID].controlType == 2)
			input = new KeyboardInputComponent(config.controls2);

	}

	//Physics.
	if(mObjectPhysics[objectID].componentType == "Dynamic")
		physics = new DynamicPhysicsComponent(mObjectPhysics[objectID], sf::Vector2f(coordinates.x, coordinates.y));
	//else if(mObjectPhysics[objectID] == "Static")

	//Graphics.
	if(mObjectGraphics[objectID].componentType == "Humanoid")
		graphics = new HumanoidGraphicsComponent(mObjectGraphics[objectID]);

	//Combat.
	if(mObjectCombat[objectID].componentType == "Humanoid")
		combat = new HumanoidCombatComponent(mObjectCombat[objectID]);

	//Social.
	if(mObjectSocial[objectID].componentType == "Humanoid")
		social = new HumanoidSocialComponent(mObjectSocial[objectID]);

	temp = new GameObject(	input,
							physics,
							graphics,
							combat,
							social	);

	if(mObjectInput[objectID].componentType == "keyboard")
		temp->setPlayer(true);
	else
		temp->setPlayer(false);

	sf::FloatRect objectRect = (*temp).getPhysics()->getRect();

	//Collision with objects.
	for(int i = 0; i < getGameObjects().size(); ++i)
		if((objectRect.intersects(getGameObjects()[i].getPhysics()->getRect())) && (temp != &getGameObjects()[i])) {
			canSpawn = false;
			break;
		}
	
	//Level map collision.
	if(objectRect.left >= 0 && objectRect.left <= (getMapWidth() - 1) * config.tileSize && objectRect.top >= 0 && objectRect.top <= (getMapHeight() - 1) * config.tileSize) {
		
		for(int i = objectRect.top / config.tileSize; i < (objectRect.top + objectRect.height) / config.tileSize; ++i)
			for(int j = objectRect.left / config.tileSize; j < (objectRect.left + objectRect.width) / config.tileSize; ++j)
				if(getCollisionMap().at(i).at(j)) {
					canSpawn = false;
					break;
				}

	} else {

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

//============GET===========================
std::vector<std::vector<int>>& World::getLevelMap() {
	return mLevelMap;
}

int World::getMapHeight() {
	return mMapHeight;
}

int World::getMapWidth() {
	return mMapWidth;
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

bool World::isPlayerAlive() {
	return mIsPlayerAlive;
}

//============SET===========================
void World::setMapHeight(int height) {
	mMapHeight = height;
}

void World::setMapWidth(int width) {
	mMapWidth = width;
}

void World::setPlayerAlive(bool isAlive) {
	mIsPlayerAlive = isAlive;
}

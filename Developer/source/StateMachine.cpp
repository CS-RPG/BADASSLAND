//StateMachine.cpp
#include <StateMachine.hpp>

extern TextureHolder		gTextureHolder;
extern sf::Font				gFont;
extern int					gFontSize;
extern int					gControlsCount;


//============StateMachine==================
//
StateMachine::StateMachine() {

	//Creating mControlsMap.
	mControlsMap.insert(std::make_pair("Up", sf::Keyboard::Up));
	mControlsMap.insert(std::make_pair("Down", sf::Keyboard::Down));
	mControlsMap.insert(std::make_pair("Left", sf::Keyboard::Left));
	mControlsMap.insert(std::make_pair("Right", sf::Keyboard::Right));

	mControlsMap.insert(std::make_pair("W", sf::Keyboard::W));
	mControlsMap.insert(std::make_pair("S", sf::Keyboard::S));
	mControlsMap.insert(std::make_pair("A", sf::Keyboard::A));
	mControlsMap.insert(std::make_pair("D", sf::Keyboard::D));

	//Config, global settings.
	loadConfigFile("config.txt");
	gFont.loadFromFile("sansation.ttf");
	gFontSize = 30;

	//Creating window, view.
	mWindow = new sf::RenderWindow(sf::VideoMode(mConfig.screenWidth, mConfig.screenHeight), "Badass Tales of BADASSLAND!!!!111");
	mView.reset(sf::FloatRect(0, 0, mConfig.screenWidth, mConfig.screenHeight));
	mView.setViewport(sf::FloatRect(0, 0, 1, 1));

	mFPS_CAP = 60;		//!!!Replace with a global variable.

	std::string levelMapName = "./levels/level1.txt";
	mCurrentState = new World(levelMapName, mConfig);

}

void StateMachine::run() {

	//sf::Time timePerFrame = sf::seconds(1 / mFPS_CAP);
	sf::Clock gameLoopClock;

	//sf::Time timeSinceLastUpdate = sf::Time::Zero;
	
	while(mWindow->isOpen()/* && (mCurrentState->getGameObjects().size() != 0)*/) {

		float deltaTime = gameLoopClock.getElapsedTime().asMicroseconds();
		deltaTime /= mConfig.gameSpeed;

		//timeSinceLastUpdate += gameLoopClock.getElapsedTime();
		gameLoopClock.restart();

		//while(timeSinceLastUpdate > timePerFrame) {
			
			//timeSinceLastUpdate -= timePerFrame;

			processEvents();
			update(deltaTime);
			render();

		//}

	}
	

}

void StateMachine::update(float deltaTime) {

	mCurrentState->update(deltaTime, *mWindow, mView, mConfig);

}

void StateMachine::render() {

	mWindow->clear(sf::Color::White);
	mCurrentState->render(*mWindow, mView, mConfig);
	mWindow->display();

}

void StateMachine::processEvents() {

	sf::Event event;
	while(mWindow->pollEvent(event)) {
		switch(event.type) {
			
			case(sf::Event::Closed):
				mWindow->close();
				break;

		}
	}

}

void StateMachine::changeState() {

}

void StateMachine::addState() {

}

void StateMachine::loadConfigFile(std::string filename = "config.txt") {

	std::ifstream inputFile;
	inputFile.open(filename);
	
	std::string temp;

	//Screen resolution.
	getline(inputFile, temp);
	inputFile >> mConfig.screenWidth >> mConfig.screenHeight;
	inputFile.get();
	inputFile.get();

	//Tile size.
	getline(inputFile, temp);
	inputFile >> mConfig.tileSize;
	inputFile.get();
	inputFile.get();

	//Game speed.
	getline(inputFile, temp);
	inputFile >> mConfig.gameSpeed;
	inputFile.get();
	inputFile.get();

	//Spawn delay (in seconds).
	getline(inputFile, temp);
	inputFile >> mConfig.spawnDelay;
	inputFile.get();
	inputFile.get();

	//Level map file.
	getline(inputFile, temp);
	inputFile >> mConfig.levelMapName;
	inputFile.get();
	inputFile.get();

	//Controls1.
	std::string key;
	getline(inputFile, temp);
	for(int i = 0; i < gControlsCount; ++i) {

		inputFile >> key;
		mConfig.controls1[i] = mControlsMap[key];

	}
	inputFile.get();
	inputFile.get();


	//Controls2.
	getline(inputFile, temp);
	for(int i = 0; i < gControlsCount; ++i) {

		inputFile >> key;
		mConfig.controls2[i] = mControlsMap[key];

	}

	inputFile.close();
	return;

}
//
//==========================================
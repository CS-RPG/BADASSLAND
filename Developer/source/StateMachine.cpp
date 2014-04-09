//StateMachine.cpp
#include <StateMachine.hpp>

extern TextureHolder		gTextureHolder;
extern sf::Font				gFont;
extern int					gFontSize;


//============StateMachine==================
//
StateMachine::StateMachine() {

	loadConfigFile("config.txt");
	gFont.loadFromFile("sansation.ttf");
	gFontSize = 30;

	std::string levelMapName = "./levels/level1.txt";
	mCurrentState = new World(mConfig.tileSize, levelMapName);

}

void StateMachine::run() {

	sf::Time timePerFrame;
	sf::Clock gameLoopClock;

	sf::Time timeSinceLastUpdate = sf::Time::Zero;

	while(mWindow.isOpen()) {

		timeSinceLastUpdate += gameLoopClock.restart();

		while(timeSinceLastUpdate > timePerFrame) {
			
			update(timePerFrame.asSeconds());
			render();

		}

	}

}

void StateMachine::update(float deltaTime) {

	mCurrentState->update(deltaTime, mWindow, mView, mConfig);

}

void StateMachine::render() {

	mCurrentState->render(mWindow, mView, mConfig);

}

void StateMachine::changeState() {

}

void StateMachine::addState() {

}

void StateMachine::loadConfigFile(std::string filename = "config.txt") {

	std::ifstream inputFile;
	inputFile.open(filename);
	
	std::string temp;

	getline(inputFile, temp);
	inputFile >> mConfig.screenWidth >> mConfig.screenHeight;
	inputFile.get();
	inputFile.get();

	getline(inputFile, temp);
	inputFile >> mConfig.tileSize;
	inputFile.get();
	inputFile.get();

	getline(inputFile, temp);
	inputFile >> mConfig.playerStartingX >> mConfig.playerStartingY;
	inputFile.get();
	inputFile.get();

	getline(inputFile, temp);
	inputFile >> mConfig.gameSpeed;
	inputFile.get();
	inputFile.get();

	getline(inputFile, temp);
	inputFile >> mConfig.levelMapName;

	inputFile.close();
	return;

}
//
//==========================================
//StateMachine.cpp
#include <StateMachine.hpp>

extern TextureHolder		gTextureHolder;
extern sf::Font				gFont;
extern int					gFontSize;


//============StateMachine==================
//
StateMachine::StateMachine() {

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

	getline(inputFile, temp);
	inputFile >> mConfig.screenWidth >> mConfig.screenHeight;
	inputFile.get();
	inputFile.get();

	getline(inputFile, temp);
	inputFile >> mConfig.tileSize;
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
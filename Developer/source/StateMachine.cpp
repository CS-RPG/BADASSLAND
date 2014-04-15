//StateMachine.cpp
#include <StateMachine.hpp>

extern TextureHolder				gTextureHolder;
extern sf::Font						gFont;
extern int							gFontSize;
extern int							gControlsCount;
extern std::vector<sf::Vector2u>	gScreenResolutions;
extern float						gMaxWindowSizeMultiplier;


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

	//Loading config.
	if(!loadConfigFile("config.txt")) {
		std::cout << "Config file error!\n" << "Press space to exit.\n";
		mTerminateGame = true;
		return;
	} else
		mTerminateGame = false;

	gFont.loadFromFile("sansation.ttf");
	gFontSize = 30;

	//Display aspect ratio, saving Window size.
	mDisplayAspectRatio = float(mConfig.screenWidth) / float(mConfig.screenHeight);
	mPreviousWindowSize = sf::Vector2u(mConfig.screenWidth, mConfig.screenHeight);

	//Creating window, view.
	if(mConfig.fullscreen)
		mWindow = new sf::RenderWindow(sf::VideoMode(mConfig.screenWidth, mConfig.screenHeight), "Badass Tales of BADASSLAND!!!!111", sf::Style::Fullscreen);
	else
		mWindow = new sf::RenderWindow(sf::VideoMode(mConfig.screenWidth, mConfig.screenHeight), "Badass Tales of BADASSLAND!!!!111");
	
	mView.reset(sf::FloatRect(0, 0, mConfig.screenWidth, mConfig.screenHeight));
	mView.setViewport(sf::FloatRect(0, 0, 1, 1));

	mFPS_CAP = 60;		//!!!Replace with a global variable.

	//std::string levelMapName = "./levels/level1.txt";
	std::string levelMapName = "./levels/" + mConfig.levelMapName;
	mCurrentState = new World(levelMapName, mConfig);

}

void StateMachine::run() {

	//sf::Time timePerFrame = sf::seconds(1 / mFPS_CAP);
	sf::Clock gameLoopClock;

	//sf::Time timeSinceLastUpdate = sf::Time::Zero;
	if(mTerminateGame) {
		//mWindow->waitEvent(sf::Event::KeyPressed(sf::Keyboard::Space));
		while(!sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {}
		return;
	}
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

			case(sf::Event::KeyPressed):
				if(event.key.code == sf::Keyboard::Escape)
					mWindow->close();
				break;

			case(sf::Event::Resized):
				
				std::cout << "Window has been resized!\n";

				if(float(mWindow->getSize().x / mWindow->getSize().y) != mDisplayAspectRatio) {
					
					std::cout << "You shall not pass!\n";
					mWindow->setSize(mPreviousWindowSize);

				} else {

					if(event.size.width > 2560 || event.size.height > 1600 || event.size.width < 320 || event.size.height < 200)
						mWindow->setSize(mPreviousWindowSize);
					else
						mPreviousWindowSize = mWindow->getSize();

				}

				break;

			default:
				break;

		}
	}

}

void StateMachine::changeState() {

}

void StateMachine::addState() {

}

bool StateMachine::loadConfigFile(std::string filename = "config.txt") {

	std::ifstream inputFile;
	inputFile.open(filename);
	
	std::string temp;

	//Screen resolution.
	getline(inputFile, temp);
	inputFile >> mConfig.screenWidth >> mConfig.screenHeight;
	if(mConfig.screenWidth > 2560 || mConfig.screenHeight > 1600 || mConfig.screenWidth < 320 || mConfig.screenHeight < 200) {
		std::cout << "Incorrect screen resolution!\n";
		return false;
	}
	inputFile.get();
	inputFile.get();

	//Fullscreen mode flag.
	getline(inputFile, temp);
	std::string fullscreenMode;
	inputFile >> fullscreenMode;
	if(fullscreenMode == "false")
		mConfig.fullscreen = false;
	else if(fullscreenMode == "true")
		mConfig.fullscreen = true;
	else {
		std::cout << "Incorrect fullscreen flag!\n";
		return false;
	}
	inputFile.get();
	inputFile.get();

	//Zoom rate.
	getline(inputFile, temp);
	inputFile >> mConfig.zoomRate;
	if(mConfig.zoomRate <= 0) {
		std::cout << "Incorrect zoom rate!\n";
		return false;
	}
	inputFile.get();
	inputFile.get();

	//Tile size.
	getline(inputFile, temp);
	inputFile >> mConfig.tileSize;
	if(mConfig.tileSize <= 0) {
		std::cout << "Incorrect tile size!\n";
		return false;
	}
	inputFile.get();
	inputFile.get();

	//Game speed.
	getline(inputFile, temp);
	inputFile >> mConfig.gameSpeed;
	if(mConfig.gameSpeed <= 0) {
		std::cout << "Incorrect game speed!\n";
		return false;
	}
	inputFile.get();
	inputFile.get();

	//Spawn delay (in seconds).
	getline(inputFile, temp);
	inputFile >> mConfig.spawnDelay;
	if(mConfig.spawnDelay <= 0) {
		std::cout << "Incorrect spawn delay!\n";
		return false;
	}
	inputFile.get();
	inputFile.get();

	//Level map file.
	getline(inputFile, temp);
	inputFile >> mConfig.levelMapName;
	std::ifstream testInputFile("./levels/" + mConfig.levelMapName);
	if(!testInputFile.good()) {
		std::cout << "File doesn't exist!\n";
		return false;
	}
	
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
	return true;

}
//
//==========================================
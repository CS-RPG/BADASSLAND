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
	initializeControlsMap();

	//Creating mStatesMap.
	initializeStatesMap();

	//Loading config.
	if(!loadConfigFile("config.txt")) {

		std::cout << "Config file error!\n" << "Press space to exit.\n";
		mTerminateGame = true;
		while(!sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {}
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

void StateMachine::initializeControlsMap() {

	mControlsMap.insert(std::make_pair("A", sf::Keyboard::A));
    mControlsMap.insert(std::make_pair("B", sf::Keyboard::B));
    mControlsMap.insert(std::make_pair("C", sf::Keyboard::C));
    mControlsMap.insert(std::make_pair("D", sf::Keyboard::D));
    mControlsMap.insert(std::make_pair("E", sf::Keyboard::E));
    mControlsMap.insert(std::make_pair("F", sf::Keyboard::F));
    mControlsMap.insert(std::make_pair("G", sf::Keyboard::G));
    mControlsMap.insert(std::make_pair("H", sf::Keyboard::H));
    mControlsMap.insert(std::make_pair("I", sf::Keyboard::I));
    mControlsMap.insert(std::make_pair("J", sf::Keyboard::J));
    mControlsMap.insert(std::make_pair("K", sf::Keyboard::K));
    mControlsMap.insert(std::make_pair("L", sf::Keyboard::L));
    mControlsMap.insert(std::make_pair("M", sf::Keyboard::M));
    mControlsMap.insert(std::make_pair("N", sf::Keyboard::N));
    mControlsMap.insert(std::make_pair("O", sf::Keyboard::O));
    mControlsMap.insert(std::make_pair("P", sf::Keyboard::P));
    mControlsMap.insert(std::make_pair("Q", sf::Keyboard::Q));
    mControlsMap.insert(std::make_pair("R", sf::Keyboard::R));
    mControlsMap.insert(std::make_pair("S", sf::Keyboard::S));
    mControlsMap.insert(std::make_pair("T", sf::Keyboard::T));
    mControlsMap.insert(std::make_pair("U", sf::Keyboard::U));
    mControlsMap.insert(std::make_pair("V", sf::Keyboard::V));
    mControlsMap.insert(std::make_pair("W", sf::Keyboard::W));
    mControlsMap.insert(std::make_pair("X", sf::Keyboard::X));
    mControlsMap.insert(std::make_pair("Y", sf::Keyboard::Y));
    mControlsMap.insert(std::make_pair("Z", sf::Keyboard::Z));
    mControlsMap.insert(std::make_pair("Num0", sf::Keyboard::Num0));
    mControlsMap.insert(std::make_pair("Num1", sf::Keyboard::Num1));
    mControlsMap.insert(std::make_pair("Num2", sf::Keyboard::Num2));
    mControlsMap.insert(std::make_pair("Num3", sf::Keyboard::Num3));
    mControlsMap.insert(std::make_pair("Num4", sf::Keyboard::Num4));
    mControlsMap.insert(std::make_pair("Num5", sf::Keyboard::Num5));
    mControlsMap.insert(std::make_pair("Num6", sf::Keyboard::Num6));
    mControlsMap.insert(std::make_pair("Num7", sf::Keyboard::Num7));
    mControlsMap.insert(std::make_pair("Num8", sf::Keyboard::Num8));
    mControlsMap.insert(std::make_pair("Num9", sf::Keyboard::Num9));
    mControlsMap.insert(std::make_pair("Escape", sf::Keyboard::Escape));
    mControlsMap.insert(std::make_pair("LControl", sf::Keyboard::LControl));
    mControlsMap.insert(std::make_pair("LShift", sf::Keyboard::LShift));
    mControlsMap.insert(std::make_pair("LAlt", sf::Keyboard::LAlt));
    mControlsMap.insert(std::make_pair("LSystem", sf::Keyboard::LSystem));
    mControlsMap.insert(std::make_pair("RControl", sf::Keyboard::RControl));
    mControlsMap.insert(std::make_pair("RShift", sf::Keyboard::RShift));
    mControlsMap.insert(std::make_pair("RAlt", sf::Keyboard::RAlt));
    mControlsMap.insert(std::make_pair("RSystem", sf::Keyboard::RSystem));
    mControlsMap.insert(std::make_pair("Menu", sf::Keyboard::Menu));
    mControlsMap.insert(std::make_pair("LBracket", sf::Keyboard::LBracket));
    mControlsMap.insert(std::make_pair("RBracket", sf::Keyboard::RBracket));
    mControlsMap.insert(std::make_pair("SemiColon", sf::Keyboard::SemiColon));
    mControlsMap.insert(std::make_pair("Comma", sf::Keyboard::Comma));
    mControlsMap.insert(std::make_pair("Period", sf::Keyboard::Period));
    mControlsMap.insert(std::make_pair("Quote", sf::Keyboard::Quote));
    mControlsMap.insert(std::make_pair("Slash", sf::Keyboard::Slash));
    mControlsMap.insert(std::make_pair("BackSlash", sf::Keyboard::BackSlash));
    mControlsMap.insert(std::make_pair("Tilde", sf::Keyboard::Tilde));
    mControlsMap.insert(std::make_pair("Equal", sf::Keyboard::Equal));
    mControlsMap.insert(std::make_pair("Dash", sf::Keyboard::Dash));
    mControlsMap.insert(std::make_pair("Space", sf::Keyboard::Space));
    mControlsMap.insert(std::make_pair("Return", sf::Keyboard::Return));
    mControlsMap.insert(std::make_pair("BackSpace", sf::Keyboard::BackSpace));
    mControlsMap.insert(std::make_pair("Tab", sf::Keyboard::Tab));
    mControlsMap.insert(std::make_pair("PageUp", sf::Keyboard::PageUp));
    mControlsMap.insert(std::make_pair("PageDown", sf::Keyboard::PageDown));
    mControlsMap.insert(std::make_pair("End", sf::Keyboard::End));
    mControlsMap.insert(std::make_pair("Home", sf::Keyboard::Home));
    mControlsMap.insert(std::make_pair("Insert", sf::Keyboard::Insert));
    mControlsMap.insert(std::make_pair("Delete", sf::Keyboard::Delete));
    mControlsMap.insert(std::make_pair("Add", sf::Keyboard::Add));
    mControlsMap.insert(std::make_pair("Subtract", sf::Keyboard::Subtract));
    mControlsMap.insert(std::make_pair("Multiply", sf::Keyboard::Multiply));
    mControlsMap.insert(std::make_pair("Divide", sf::Keyboard::Divide));
    mControlsMap.insert(std::make_pair("Left", sf::Keyboard::Left));
    mControlsMap.insert(std::make_pair("Right", sf::Keyboard::Right));
    mControlsMap.insert(std::make_pair("Up", sf::Keyboard::Up));
    mControlsMap.insert(std::make_pair("Down", sf::Keyboard::Down));
    mControlsMap.insert(std::make_pair("Numpad0", sf::Keyboard::Numpad0));
    mControlsMap.insert(std::make_pair("Numpad1", sf::Keyboard::Numpad1));
    mControlsMap.insert(std::make_pair("Numpad2", sf::Keyboard::Numpad2));
    mControlsMap.insert(std::make_pair("Numpad3", sf::Keyboard::Numpad3));
    mControlsMap.insert(std::make_pair("Numpad4", sf::Keyboard::Numpad4));
    mControlsMap.insert(std::make_pair("Numpad5", sf::Keyboard::Numpad5));
    mControlsMap.insert(std::make_pair("Numpad6", sf::Keyboard::Numpad6));
    mControlsMap.insert(std::make_pair("Numpad7", sf::Keyboard::Numpad7));
    mControlsMap.insert(std::make_pair("Numpad8", sf::Keyboard::Numpad8));
	mControlsMap.insert(std::make_pair("Numpad9", sf::Keyboard::Numpad9));
	mControlsMap.insert(std::make_pair("F1", sf::Keyboard::F1));
    mControlsMap.insert(std::make_pair("F2", sf::Keyboard::F2));
    mControlsMap.insert(std::make_pair("F3", sf::Keyboard::F3));
    mControlsMap.insert(std::make_pair("F4", sf::Keyboard::F4));
    mControlsMap.insert(std::make_pair("F5", sf::Keyboard::F5));
    mControlsMap.insert(std::make_pair("F6", sf::Keyboard::F6));
    mControlsMap.insert(std::make_pair("F7", sf::Keyboard::F7));
    mControlsMap.insert(std::make_pair("F8", sf::Keyboard::F8));
    mControlsMap.insert(std::make_pair("F9", sf::Keyboard::F9));
    mControlsMap.insert(std::make_pair("F10", sf::Keyboard::F10));
    mControlsMap.insert(std::make_pair("F11", sf::Keyboard::F11));
    mControlsMap.insert(std::make_pair("F12", sf::Keyboard::F12));
    mControlsMap.insert(std::make_pair("F13", sf::Keyboard::F13));
    mControlsMap.insert(std::make_pair("F14", sf::Keyboard::F14));
    mControlsMap.insert(std::make_pair("F15", sf::Keyboard::F15));
    mControlsMap.insert(std::make_pair("Pause", sf::Keyboard::Pause));

}

void StateMachine::initializeStatesMap() {

	mStatesMap.insert(std::make_pair("MainMenu", States::MainMenu));
	mStatesMap.insert(std::make_pair("World", States::World));
	mStatesMap.insert(std::make_pair("Local", States::Local));
	mStatesMap.insert(std::make_pair("InGame", States::InGame));

}

void StateMachine::run() {

	//sf::Time timePerFrame = sf::seconds(1 / mFPS_CAP);
	sf::Clock gameLoopClock;

	//sf::Time timeSinceLastUpdate = sf::Time::Zero;
	if(mTerminateGame)
		return;

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

				if(mWindow->getSize().y != 0 && float(mWindow->getSize().x / mWindow->getSize().y) != mDisplayAspectRatio) {
					
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

void StateMachine::changeState(std::string stateName) {
	/*
	mCurrentState->onExit();
	delete mCurrentState;

	switch(mStatesMap[stateName]) {

		case(States::MainMenu):
			mCurrentState = new MainMenu();
			break;

		case(States::World):
			mCurrentState = new World("level2", mConfig);
			break;

	}

	mCurrentState->onCreate();
	*/
}

void StateMachine::addState() {

}

bool StateMachine::loadConfigFile(std::string filename = "config.txt") {

	std::ifstream inputFile;
	inputFile.open(filename);
	if(!inputFile.good()) {
		std::cout << "Config file doesn't exist!\n";
		return false;
	}

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
		std::cout << "Level file doesn't exist!\n";
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
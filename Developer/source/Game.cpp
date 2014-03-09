//Game.cpp
#include <SFML/Graphics.hpp>
#include <World.hpp>
#include <GameObject.hpp>

#include <fstream>
#include <vector>

#include <Game.hpp>


//=================Game=====================
//
void Game::run() {

	loadConfigFile(mConfig);

	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	mTimePerFrame = sf::seconds(1.f/60.f);

	while(mWindow.isOpen()) {

		sf::Time elapsedTime = mGameClock.restart();
		timeSinceLastUpdate = elapsedTime;
		while(timeSinceLastUpdate > mTimePerFrame) {

			timeSinceLastUpdate -= mTimePerFrame;

			processEvents();
			update(mTimePerFrame);

		}

	}

}

void Game::processEvents() {

	sf::Event event;

	while(mWindow.pollEvent(event)) {

		switch(event.type) {

			case(sf::Event::Closed)
				:mWindow.close();
				break;

		}

	}

}

void Game::update(sf::Time) {

	std::vector<std::vector<int>> levelMap = mWorld.getLevelMap();

}

void Game::render() {

}

void Game::loadConfigFile(config& config) {

	std::ifstream inputFile;
	inputFile.open("config.txt");
	
	std::string temp;

	getline(inputFile, temp);
	inputFile >> config.screenWidth >> config.screenHeight;
	inputFile.get();
	inputFile.get();

	getline(inputFile, temp);
	inputFile >> config.tileSize;
	inputFile.get();
	inputFile.get();

	getline(inputFile, temp);
	inputFile >> config.playerStartingX >> config.playerStartingY;
	inputFile.get();
	inputFile.get();

	getline(inputFile, temp);
	inputFile >> config.gameSpeed;
	inputFile.get();
	inputFile.get();

	getline(inputFile, temp);
	inputFile >> config.levelMapName;

	inputFile.close();
	return;

}
//
//==========================================




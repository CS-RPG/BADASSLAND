
//FONT - BIG.
//http://patorjk.com/software/taag/



//SFML includes.
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

//std includes.
#include <fstream>
#include <iostream>
#include <string>
#include <stdlib.h>
#include <ctime>
#include <sstream>
#include <vector>

//Badass includes.
#include <Game.hpp>
#include <World.hpp>
#include <GameObject.hpp>
#include <DataTypes.hpp>


//   _____                                             _       
//  / ____|                                           | |      
// | |     ___  _ __ ___  _ __   ___  _ __   ___ _ __ | |_ ___ 
// | |    / _ \| '_ ` _ \| '_ \ / _ \| '_ \ / _ \ '_ \| __/ __|
// | |___| (_) | | | | | | |_) | (_) | | | |  __/ | | | |_\__ \
//  \_____\___/|_| |_| |_| .__/ \___/|_| |_|\___|_| |_|\__|___/
//                       | |                                   
//                       |_|
//

//============BASE==========================
#include <InputComponent.hpp>
#include <PhysicsComponent.hpp>
#include <GraphicsComponent.hpp>
#include <CombatComponent.hpp>
#include <SocialComponent.hpp>

//============INPUT=========================
#include <KeyboardInputComponent.hpp>
#include <BotActiveInputComponent.hpp>
#include <BotPassiveInputComponent.hpp>

//============PHYSICS=======================
#include <DynamicPhysicsComponent.hpp>

//============GRAPHICS======================
#include <HumanoidGraphicsComponent.hpp>

//============COMBAT========================
#include <HumanoidCombatComponent.hpp>

//============SOCIAL========================
#include <HumanoidSocialComponent.hpp>




//Range multiplier for target capturing.
const float rangeMultiplier = 6;



//FUNCTION PROTOTYPES.
float calculateDistance(sf::FloatRect, sf::FloatRect);




//  ______             _               __                  _   _                 
// |  ____|           (_)             / _|                | | (_)                
// | |__   _ __   __ _ _ _ __   ___  | |_ _   _ _ __   ___| |_ _  ___  _ __  ___ 
// |  __| | '_ \ / _` | | '_ \ / _ \ |  _| | | | '_ \ / __| __| |/ _ \| '_ \/ __|
// | |____| | | | (_| | | | | |  __/ | | | |_| | | | | (__| |_| | (_) | | | \__ \
// |______|_| |_|\__, |_|_| |_|\___| |_|  \__,_|_| |_|\___|\__|_|\___/|_| |_|___/
//                __/ |                                                          
//               |___
//


void loadConfigFile(config& config, std::string filename = "config.txt") {

	std::ifstream inputFile;
	inputFile.open(filename);
	
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

float calculateDistance(sf::FloatRect object, sf::FloatRect target) {

	float deltaX =  (target.left + target.width / 2) -
					(object.left + object.width / 2);

	float deltaY =  (target.top + target.height / 2) - 
					(object.top + object.height / 2);

	float distance = std::sqrt( deltaX * deltaX + deltaY * deltaY);

	return distance;

}



//FACTORY.



//  __  __       _       
// |  \/  |     (_)      
// | \  / | __ _ _ _ __  
// | |\/| |/ _` | | '_ \ 
// | |  | | (_| | | | | |
// |_|  |_|\__,_|_|_| |_|
//                       

int main() {
	
	srand(time(NULL));

	//Loading config file.
	config config;
	loadConfigFile(config, "config.txt");

	//Game game;
	//game.run();


	//Clocks.
	sf::Clock gameClock;
	sf::Clock invincibilityClock;
	sf::Clock spawnClock;


	//
	//GRAPHICS.
	//
	//
	//
	sf::RectangleShape rectangle(sf::Vector2f(config.tileSize, config.tileSize));	//For tiles (temporary).

	//Loading font.
	sf::Font font;
	font.loadFromFile("sansation.ttf");

	//HUD Health.
	sf::Text textHealth("", font, 30);
	textHealth.setStyle(sf::Text::Bold);
	textHealth.setColor(sf::Color::Red);
	textHealth.setPosition(0, 0);

	//HUD Mana.
	sf::Text textMana("", font, 30);
	textMana.setStyle(sf::Text::Bold);
	textMana.setColor(sf::Color::Blue);
	textMana.setPosition(0, textMana.getCharacterSize());

	//HUD Enemy count.
	sf::Text textEnemyCount("", font, 30);
	textEnemyCount.setStyle(sf::Text::Bold);
	textEnemyCount.setColor(sf::Color::Color(184, 138, 0));
	textEnemyCount.setPosition(0, textEnemyCount.getCharacterSize() * 2);

	//HUD Player coordinates.
	sf::Text textPlayerCoordinates("", font, 30);
	textPlayerCoordinates.setStyle(sf::Text::Bold);
	textPlayerCoordinates.setColor(sf::Color::Color(125, 145, 176));
	textPlayerCoordinates.setPosition(0, textPlayerCoordinates.getCharacterSize() * 3);

	//HUD Mouse coordinates.
	sf::Text textMouseCoordinates("", font, 30);
	textMouseCoordinates.setStyle(sf::Text::Bold);
	textMouseCoordinates.setColor(sf::Color::Color(125, 145, 176));
	textMouseCoordinates.setPosition(0, textMouseCoordinates.getCharacterSize() * 6);

	//Loading textures.
	sf::Texture hpBar;
	sf::Texture tileSet;
	sf::Texture playerTexture;
	sf::Texture enemyTexture;
	sf::Texture healthPotionTexture;

	if(!hpBar.loadFromFile("./textures/HPBar.png"))							return 1;
	if(!tileSet.loadFromFile("./textures/testTileSet.png"))					return 1;
	if(!playerTexture.loadFromFile("./textures/playerSpriteList.png"))		return 1;
	if(!enemyTexture.loadFromFile("./textures/enemySpriteList.png"))		return 1;
	if(!healthPotionTexture.loadFromFile("./textures/healthPotion.png"))	return 1;

	sf::Sprite tile(tileSet);

	//Sounds.
	sf::SoundBuffer emenyHitSoundBuffer;

	emenyHitSoundBuffer.loadFromFile("sound1.ogg");

	sf::Sound emenyHitSound(emenyHitSoundBuffer);

	//World.
	std::string levelMapName = "./levels/level1.txt";

	World world;
	world.loadLevelMap(levelMapName);
	std::vector<std::vector<int>> levelMap = world.getLevelMap();
	
	//Creating player.
	world.getGameObjects().push_back( *(new GameObject(  new KeyboardInputComponent(),
														 new DynamicPhysicsComponent(sf::FloatRect(config.playerStartingX, config.playerStartingY, config.tileSize, config.tileSize), 0.1),
														 new HumanoidGraphicsComponent(&playerTexture, &hpBar, &font),
														 new HumanoidCombatComponent(150, 150, 40, 40, 2),
														 new HumanoidSocialComponent("Player 1", "players")  )) );

	//Creating window, view.
	sf::RenderWindow mWindow(sf::VideoMode(config.screenWidth, config.screenHeight), "Badass Tales of BADASSLAND!!!!111");
	sf::View mView;
	mView.reset(sf::FloatRect(0, 0, config.screenWidth, config.screenHeight));
	mView.setViewport(sf::FloatRect(0, 0, 1, 1));
	sf::Vector2f mViewPosition;

	//Game cycle.
	while( (mWindow.isOpen()) && (world.getGameObjects().size() != 0) ) {

		float time = gameClock.getElapsedTime().asMicroseconds();
		gameClock.restart();
		time /= config.gameSpeed;

		//Processing events.
		sf::Event event;
		while(mWindow.pollEvent(event)) {
			switch(event.type) {
				
				case(sf::Event::Closed):
					mWindow.close();
					break;

			}
		}

		//Spawning objects.
		if((sf::Keyboard::isKeyPressed(sf::Keyboard::G)) && (spawnClock.getElapsedTime().asSeconds() > 0.25)) {
			world.getGameObjects().push_back( *(new GameObject( new BotActiveInputComponent(),
																new DynamicPhysicsComponent(sf::FloatRect(config.playerStartingX, config.playerStartingY, config.tileSize, config.tileSize), 0.05),
																new HumanoidGraphicsComponent(&enemyTexture, &hpBar, &font),
																new HumanoidCombatComponent(150, 150, 40, 130, 2),
																new HumanoidSocialComponent("Dark Elf", "dark_elves")  )) );
			spawnClock.restart();
		}

		if((sf::Keyboard::isKeyPressed(sf::Keyboard::F)) && (spawnClock.getElapsedTime().asSeconds() > 0.25)) {
			world.getGameObjects().push_back( *(new GameObject( new BotPassiveInputComponent(4),
																new DynamicPhysicsComponent(sf::FloatRect(config.playerStartingX, config.playerStartingY, config.tileSize, config.tileSize), 0.03),
																new HumanoidGraphicsComponent(&playerTexture, &hpBar, &font),
																new HumanoidCombatComponent(150, 150, 40, 40, 2),
																new HumanoidSocialComponent("Green Elf", "green_elves")  )) );
			spawnClock.restart();
		}



		//Updating all objects.
		//player->update(time, levelMap, &config, world);
		for(int i = 0; i < world.getGameObjects().size(); ++i)	
			world.getGameObjects()[i].update(time, levelMap, &config, world);



		//HUD.
		std::ostringstream hudHealth;
		//std::ostringstream hudMana;
		std::ostringstream hudEnemyCount;
		std::ostringstream hudPlayerCoordinates;
		std::ostringstream hudMouseCoordinates;

		//hudHealth << player->getCombat()->getHP();
		hudHealth << world.getGameObjects()[0].getCombat()->getHP();
		//hudMana << player.getMP();
		hudEnemyCount << "Number of game objects: " << world.getGameObjects().size();
		hudPlayerCoordinates << "X: " << world.getGameObjects()[0].getPhysics()->getRect().left + config.tileSize / 2 << '\n'
							 << "Y: " << world.getGameObjects()[0].getPhysics()->getRect().top + config.tileSize / 2;
		hudMouseCoordinates << "X: " << sf::Mouse::getPosition(mWindow).x << '\n'
							<< "Y: " << sf::Mouse::getPosition(mWindow).y;

		//textMana.setString(hudMana.str());
		textHealth.setString(hudHealth.str());
		textEnemyCount.setString(hudEnemyCount.str());
		textPlayerCoordinates.setString(hudPlayerCoordinates.str());
		textMouseCoordinates.setString(hudMouseCoordinates.str());


		//View.
		mViewPosition.x = world.getGameObjects()[0].getPhysics()->getRect().left + config.tileSize / 2 - config.screenWidth / 2;
		mViewPosition.y = world.getGameObjects()[0].getPhysics()->getRect().top + config.tileSize / 2 - config.screenHeight / 2;
		
		if(mViewPosition.x < 0)																	mViewPosition.x = 0;
		if(mViewPosition.x > world.getMapWidth() * config.tileSize - config.screenWidth)		mViewPosition.x = world.getMapWidth() * config.tileSize - config.screenWidth;
		if(mViewPosition.y < 0)																	mViewPosition.y = 0;
		if(mViewPosition.y > world.getMapHeight() * config.tileSize - config.screenHeight)		mViewPosition.y = world.getMapHeight() * config.tileSize - config.screenHeight;

		mView.reset(sf::FloatRect(mViewPosition.x, mViewPosition.y, config.screenWidth, config.screenHeight));
		mWindow.setView(mView);

		textHealth.setPosition(mViewPosition.x, mViewPosition.y);
		//textMana.setPosition(mViewPosition.x, mViewPosition.y + textMana.getCharacterSize());
		textEnemyCount.setPosition(mViewPosition.x, mViewPosition.y + textEnemyCount.getCharacterSize() * 2);
		textPlayerCoordinates.setPosition(mViewPosition.x, mViewPosition.y + textPlayerCoordinates.getCharacterSize() * 3);
		textMouseCoordinates.setPosition(mViewPosition.x, mViewPosition.y + textPlayerCoordinates.getCharacterSize() * 6);


		//Clearing the screen.
		mWindow.clear(sf::Color::White);

		//Rendering level map tiles.
		for(int i = 0; i < world.getMapHeight(); ++i) {
			for(int j = 0; j < world.getMapWidth(); ++j) {
				
				switch(levelMap[i][j]) {
					case 'B': rectangle.setFillColor(sf::Color::Black); break;
					case '0': rectangle.setFillColor(sf::Color::Green); break;
					default: continue;
				}

				rectangle.setPosition(config.tileSize * j, config.tileSize * i);
				mWindow.draw(rectangle);
				//tile.setPosition(tileSize * j - offsetX, tileSize * i - offsetY);
				//mWindow.draw(tile);

			}
		}


		//Rendering all the objects.
		for(int i = 0; i < world.getGameObjects().size(); ++i) 
			world.getGameObjects()[i].getGraphics()->draw(mWindow);

		mWindow.draw(textHealth);
		mWindow.draw(textMana);
		mWindow.draw(textEnemyCount);
		mWindow.draw(textPlayerCoordinates);
		mWindow.draw(textMouseCoordinates);
		mWindow.display();
		
		//Deleting objects marked for removal.
		for(int i = 0; i < world.getGameObjects().size(); ++i)
			if(world.getGameObjects()[i].getCombat()->isMarkedForRemoval()) {
				world.getGameObjects().erase(world.getGameObjects().begin() + i);
				--i;
				//world.getGameObjects()[i].getInput()->setTargeting(false);
			}
		
	}

	return 0;

}
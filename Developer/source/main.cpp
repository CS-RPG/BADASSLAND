
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

//Game font.
//extern sf::Font font;
//extern int fontCharacterSize;


//FUNCTION PROTOTYPES.
float calculateDistance(sf::FloatRect, sf::FloatRect);




//  ______             _               __                  _   _                 
// |  ____|           (_)             / _|                | | (_)                
// | |__   _ __   __ _ _ _ __   ___  | |_ _   _ _ __   ___| |_ _  ___  _ __  ___ 
// |  __| | '_ \ / _` | | '_ \ / _ \ |  _| | | | '_ \ / __| __| |/ _ \| '_ \/ __|
// | |____| | | | (_| | | | | |  __/ | | | |_| | | | | (__| |_| | (_) | | | \__ \
// |______|_| |_|\__, |_|_| |_|\___| |_|  \__,_|_| |_|\___|\__|_|\___/|_| |_|___/
//                __/ |                                                          
//               |___/
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
/*
GameObject* createPlayer() {

	return new GameObject(	new KeyboardInputComponent(),
							new DynamicPhysicsComponent(sf::FloatRect(config.playerStartingX, config.playerStartingY, config.tileSize, config.tileSize), 0.1),
							new HumanoidGraphicsComponent(&playerTexture, &hpBar, &font),
							new HumanoidCombatComponent(150, 150, 40, 40, 2),
							new HumanoidSocialComponent("Player 1", "players")  );

}
*/



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


	//Clocks.
	sf::Clock gameClock;
	sf::Clock spawnClock;


	//===============GRAPHICS======================
	//
	//Loading font.
	sf::Font font;
	font.loadFromFile("sansation.ttf");


	//Loading textures.
	sf::Texture hpBar;
	sf::Texture tileSet;
	sf::Texture greenElfTexture;
	sf::Texture redElfTexture;
	sf::Texture yellowElfTexture;
	sf::Texture healthPotionTexture;

	if(!hpBar.loadFromFile("./textures/HPBar.png"))								return 1;
	if(!tileSet.loadFromFile("./textures/testTileSet.png"))						return 1;
	if(!greenElfTexture.loadFromFile("./textures/green_elf_sprite_list.png"))	return 1;
	if(!redElfTexture.loadFromFile("./textures/red_elf_sprite_list.png"))		return 1;
	if(!yellowElfTexture.loadFromFile("./textures/yellow_elf_sprite_list.png"))	return 1;
	if(!healthPotionTexture.loadFromFile("./textures/healthPotion.png"))		return 1;

	//sf::Sprite tile(tileSet);
	//
	//=============================================



	//===============SOUND=========================
	//
	//sf::SoundBuffer emenyHitSoundBuffer;

	//emenyHitSoundBuffer.loadFromFile("sound1.ogg");

	//sf::Sound emenyHitSound(emenyHitSoundBuffer);
	//
	//=============================================



	//World.
	std::string levelMapName = "./levels/level1.txt";

	World world(font, 30, config.tileSize, levelMapName);
	world.loadLevelMap(levelMapName);
	
	//Key bindings.
	sf::Keyboard::Key playerControls[4] = { sf::Keyboard::Key::Up,
											sf::Keyboard::Key::Down,
											sf::Keyboard::Key::Left,
											sf::Keyboard::Key::Right };

	sf::Keyboard::Key enemyControls[4] =  {	sf::Keyboard::Key::W,
											sf::Keyboard::Key::S,
											sf::Keyboard::Key::A,
											sf::Keyboard::Key::D };

	//Creating player.
	world.getGameObjects().push_back( *(new GameObject(  new KeyboardInputComponent(playerControls),
														 new DynamicPhysicsComponent(sf::FloatRect(config.playerStartingX, config.playerStartingY, config.tileSize, config.tileSize), 0.1),
														 new HumanoidGraphicsComponent(&greenElfTexture, &hpBar, &font),
														 new HumanoidCombatComponent(150, 150, 40, 40, 2),
														 new HumanoidSocialComponent("Player 1", "yellow_elves")  )) );

	//Creating window, view.
	sf::RenderWindow mWindow(sf::VideoMode(config.screenWidth, config.screenHeight), "Badass Tales of BADASSLAND!!!!111");
	sf::View mView;
	mView.reset(sf::FloatRect(0, 0, config.screenWidth, config.screenHeight));
	mView.setViewport(sf::FloatRect(0, 0, 1, 1));

	//Game loop.
	while( (mWindow.isOpen()) && (world.getGameObjects().size() != 0) ) {

		float deltaTime = gameClock.getElapsedTime().asMicroseconds();
		gameClock.restart();
		deltaTime /= config.gameSpeed;

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
																new DynamicPhysicsComponent(sf::FloatRect(2 * config.playerStartingX, config.playerStartingY, config.tileSize, config.tileSize), 0.05),
																new HumanoidGraphicsComponent(&redElfTexture, &hpBar, &font),
																new HumanoidCombatComponent(150, 150, 40, 130, 2),
																new HumanoidSocialComponent("Red Elf", "red_elves")  )) );
			spawnClock.restart();
		}

		if((sf::Keyboard::isKeyPressed(sf::Keyboard::F)) && (spawnClock.getElapsedTime().asSeconds() > 0.25)) {
			world.getGameObjects().push_back( *(new GameObject( new BotActiveInputComponent(/*enemyControls*/),
																new DynamicPhysicsComponent(sf::FloatRect(3 * config.playerStartingX, config.playerStartingY, config.tileSize, config.tileSize), 0.03),
																new HumanoidGraphicsComponent(&yellowElfTexture, &hpBar, &font),
																new HumanoidCombatComponent(150, 150, 40, 130, 2),
																new HumanoidSocialComponent("Yellow Elf", "yellow_elves")  )) );
			spawnClock.restart();
		}
		

		//Updating.
		world.update(deltaTime, mWindow, mView, config);


		//Rendering.
		mWindow.clear(sf::Color::White);
		world.render(mWindow, mView, config);
		mWindow.display();
		
	}

	return 0;

}
//  ____          _____           _____ _____ _               _   _ _____  
// |  _ \   /\   |  __ \   /\    / ____/ ____| |        /\   | \ | |  __ \ 
// | |_) | /  \  | |  | | /  \  | (___| (___ | |       /  \  |  \| | |  | |
// |  _ < / /\ \ | |  | |/ /\ \  \___ \\___ \| |      / /\ \ | . ` | |  | |
// | |_) / ____ \| |__| / ____ \ ____) |___) | |____ / ____ \| |\  | |__| |
// |____/_/____\_\_____/_/    \_\_____/_____/|______/_/    \_\_| \_|_____/ 
// |  __ \|  __ \ / __ \     | |  ____/ ____|__   __|                      
// | |__) | |__) | |  | |    | | |__ | |       | |                         
// |  ___/|  _  /| |  | |_   | |  __|| |       | |                         
// | |    | | \ \| |__| | |__| | |___| |____   | |                         
// |_|    |_|  \_\\____/ \____/|______\_____|  |_|                         
//                                                        


#include <_includes_badass.hpp>
#include <_includes_system.hpp>


//============GLOBAL VARIABLES==============
//
const float			gRangeMultiplier = 6;	//Range multiplier for target capturing.

TextureHolder		gTextureHolder;
sf::Font			gFont;
int					gFontSize;
//
//==========================================


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


	//===============CLOCKS========================
	sf::Clock gameClock;


	//===============GRAPHICS======================
	//
	//Loading font.
	gFont.loadFromFile("sansation.ttf");
	gFontSize = 30;

	//Loading textures.
	gTextureHolder.load(Textures::HP_Bar, "./textures/HPBar.png");
	gTextureHolder.load(Textures::Elf_Red, "./textures/red_elf_sprite_list.png");
	gTextureHolder.load(Textures::Elf_Green, "./textures/green_elf_sprite_list.png");
	gTextureHolder.load(Textures::Elf_Yellow, "./textures/yellow_elf_sprite_list.png");
	//gTextureHolder.load(Textures::TileSet, "./textures/testTileSet.png");
	//gTextureHolder.load(Textures::HP_Potion, "./textures/healthPotion.png");

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

	World world(config.tileSize, levelMapName);
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
														 new HumanoidGraphicsComponent(Textures::Elf_Green),
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

		//Updating.
		world.update(deltaTime, mWindow, mView, config);


		//Rendering.
		mWindow.clear(sf::Color::White);
		world.render(mWindow, mView, config);
		mWindow.display();
		
	}

	return 0;

}
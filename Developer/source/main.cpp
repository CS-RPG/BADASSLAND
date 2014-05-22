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
float						gRangeMultiplier = 60;	//Range multiplier for target capturing.
int							gControlsCount = 4;		//Number of user-defined controls.

TextureHolder				gTextureHolder;
sf::Font					gFont;
int							gFontSize;

float						gAnimationSpeedMultiplier = 0.05;

float						gMapCollisionAccuracy = 0.08;		//Should be less than 1.
float						gMaxWindowSizeMultiplier = 3;
float						gMaxZoomRate = 10;
float						gGridThickness = 1.5;

std::vector<sf::Vector2u>	gScreenResolutions;


//============DIRECTORIES===================
std::string					gLevelsDirectiory = "./levels/";
std::string					gTexturesDirectory = "./textures/";
std::string					gObjectsDirectoty = "./Objects/";


//  ______             _               __                  _   _                 
// |  ____|           (_)             / _|                | | (_)                
// | |__   _ __   __ _ _ _ __   ___  | |_ _   _ _ __   ___| |_ _  ___  _ __  ___ 
// |  __| | '_ \ / _` | | '_ \ / _ \ |  _| | | | '_ \ / __| __| |/ _ \| '_ \/ __|
// | |____| | | | (_| | | | | |  __/ | | | |_| | | | | (__| |_| | (_) | | | \__ \
// |______|_| |_|\__, |_|_| |_|\___| |_|  \__,_|_| |_|\___|\__|_|\___/|_| |_|___/
//                __/ |                                                          
//               |___/
//


float calculateDistance(sf::FloatRect object, sf::FloatRect target) {

	float deltaX =  (target.left + target.width / 2) -
					(object.left + object.width / 2);

	float deltaY =  (target.top + target.height / 2) - 
					(object.top + object.height / 2);

	float distance = std::sqrt( deltaX * deltaX + deltaY * deltaY);

	return distance;

}

void outputMap(std::vector<std::vector<int>> map) {
	
	for (int i = 0; i < map.size(); ++i) {
		for (int j = 0; j < map[0].size(); ++j)
			std::cout << map[i][j];
		std::cout << '\n';
	}

}

void outputPath(std::vector<sf::Vector2i>& path) {

	std::cout << "\nPath:\n";
	for (int i = 0; i < path.size(); ++i)
		std::cout << path[i].x << ' ' << path[i].y << '\n';

}

//============OBJECT DATA FUNCTIONS=========

//Input.
std::istream& operator>>(std::istream& is, objectInput& inputSettings) {

	char buffer[50];
	is.getline(buffer, 50);
	inputSettings.componentType = std::string(buffer);

	if(inputSettings.componentType == std::string("keyboard"))
		is >> inputSettings.controlType;

	return is;

}

//Physics.
std::istream& operator>>(std::istream& is, objectPhysics& physicsSettings) {

	char buffer[50];
	is.getline(buffer, 50);
	physicsSettings.componentType = std::string(buffer);

	is >> physicsSettings.size.x;
	is >> physicsSettings.size.y;
	is >> physicsSettings.speed;

	return is;

}

//Graphics.
std::istream& operator>>(std::istream& is, objectGraphics& graphicsSettings) {

	bool badData = false;

	char buffer[50];
	is.getline(buffer, 50);
	graphicsSettings.componentType = std::string(buffer);

	is.getline(buffer, 50);
	graphicsSettings.textureID = std::string(buffer);
	
	is >> graphicsSettings.frameCount;
	for(int i = 0; i < 5; ++i) {

		is >> graphicsSettings.frames[i].x;
		is >> graphicsSettings.frames[i].y;
		if(graphicsSettings.frames[i].x < 0 || graphicsSettings.frames[i].y < 0)
			badData = true;

	}
	is >> graphicsSettings.width;
	is >> graphicsSettings.height;

	if(	graphicsSettings.frameCount <= 0 ||
		graphicsSettings.width <= 0 ||
		graphicsSettings.height <= 0)
		badData = true;

	if(badData)
		is.setstate(std::ios::failbit);

	return is;

}

//Combat.
std::istream& operator>>(std::istream& is, objectCombat& combatSettings) {

	char buffer[50];
	is.getline(buffer, 50);
	combatSettings.componentType = std::string(buffer);

	is >> combatSettings.maxHP;
	is >> combatSettings.damage;
	is >> combatSettings.attackRange;
	is >> combatSettings.attackSpeed;

	return is;

}

//Social.
std::istream& operator>>(std::istream& is, objectSocial& socialSettings) {

	char buffer[50];
	is.getline(buffer, 50);
	socialSettings.componentType = std::string(buffer);

	is.getline(buffer, 50);
	socialSettings.name = std::string(buffer);
	std::cout << socialSettings.name;

	is.getline(buffer, 50);
	socialSettings.faction = std::string(buffer);

	return is;

}


//  __  __       _       
// |  \/  |     (_)      
// | \  / | __ _ _ _ __  
// | |\/| |/ _` | | '_ \ 
// | |  | | (_| | | | | |
// |_|  |_|\__,_|_|_| |_|
//                       

int main() {
	
	srand(time(NULL));

	StateMachine game;
	game.run();

	return 0;

}

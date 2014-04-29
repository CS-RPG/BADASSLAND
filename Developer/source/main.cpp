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
float						gRangeMultiplier = 6;	//Range multiplier for target capturing.
int							gControlsCount = 4;		//Number of user-defined controls.

TextureHolder				gTextureHolder;
sf::Font					gFont;
int							gFontSize;

float						gMapCollisionAccuracy = 0.05;		//Should be less than 1.
float						gMaxWindowSizeMultiplier = 3;
float						gMaxZoomRate = 10;
float						gGridThickness = 1.5;

std::vector<sf::Vector2u>	gScreenResolutions;

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

	for (int i = 0; i < path.size(); ++i)
		std::cout << path[i].x + 1 << ' ' << path[i].y + 1 << '\n';

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
#include <SFML/Graphics.hpp>

//Attributes.
struct attributes {

     int							strength;				//Carry weight, melee damage. 
     int							endurance;				//HP, resistances.
     int							agility;				//Speed, ranged damage.
     int							intelligence;			//MP, magic damage.
     int							luck;					//Dodge chance, critical damage chance.

};

//Resistances.
struct resistances {

     int							poison;
     int							paralize;
     int							lightning;
     int							fire;
     int							ice;
     int							earth;

};

//Health bar.
struct healthBar {

	sf::Sprite						mSprite;
	int								mWidth;
	int								mHeight;

};

//Structure used for storing config values (your C.O.).
struct config {

	float							invincibilityTime;		
	int								playerStartingX;					
	int								playerStartingY;			
	int								mapHeight;				
	int								mapWidth;				
	int								tileSize;						
	int								screenWidth;					
	int								screenHeight;
	int								gameSpeed;
	float							offsetX;				//Map scrolling
	float							offsetY;				//offset.
	std::string						levelMapName;

};


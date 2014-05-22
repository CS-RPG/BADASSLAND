//DataTypes.hpp
#ifndef _DataTypes_hpp_included_
#define _DataTypes_hpp_included_

#include <_includes_system.hpp>
#include <TextureHolder.hpp>

extern TextureHolder		gTextureHolder;

class InputComponent;
class PhysicsComponent;
class GraphicsComponent;
class CombatComponent;
class SocialComponent;


//============ATTRIBUTES====================
struct attributes {

     int							strength;				//Carry weight, melee damage. 
     int							endurance;				//HP, resistances.
     int							agility;				//Speed, ranged damage.
     int							intelligence;			//MP, magic damage.
     int							luck;					//Dodge chance, critical damage chance.

};

//============RESISTANCES===================
struct resistances {

     int							poison;
     int							paralize;
     int							lightning;
     int							fire;
     int							ice;
     int							earth;

};

//============CONFIG========================
struct config {
	
	int								screenWidth;					
	int								screenHeight;
	bool							fullscreen;
	float							zoomRate;
	int								tileSize;						

	int								gameSpeed;
	float							spawnDelay;

	std::string						levelMapName;

	sf::Keyboard::Key				controls1[4];
	sf::Keyboard::Key				controls2[4];

};

//============OBJECT DATA===============
struct objectInput {

	friend							std::istream& operator>>(std::istream& is, objectInput& inputSettings);

	std::string						componentType;

	int								controlType;

};

struct objectPhysics {

	friend							std::istream& operator>>(std::istream& is, objectPhysics& physicsSettings);

	std::string						componentType;

	sf::Vector2f					size;
	float							speed;

};

struct objectGraphics {

	friend							std::istream& operator>>(std::istream& is, objectGraphics& graphicsSettings);

	std::string						componentType;

	std::string						textureID;

	int								width;
	int								height;

	int								frameCount;
	sf::Vector2i					frames[5];

};

struct objectCombat {

	friend							std::istream& operator>>(std::istream& is, objectCombat& combatSettings);

	std::string						componentType;

	float							maxHP;
	float							damage;
	float							attackRange;
	float							attackSpeed;

};

struct objectSocial {

	friend							std::istream& operator>>(std::istream& is, objectSocial& socialSettings);

	std::string						componentType;

	std::string						name;
	std::string						faction;

};


#endif

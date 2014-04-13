//DataTypes.hpp
#ifndef _DataTypes_hpp_included_
#define _DataTypes_hpp_included_

#include <_includes_system.hpp>

//============Attributes====================
//
struct attributes {

     int							strength;				//Carry weight, melee damage. 
     int							endurance;				//HP, resistances.
     int							agility;				//Speed, ranged damage.
     int							intelligence;			//MP, magic damage.
     int							luck;					//Dodge chance, critical damage chance.

};
//
//==========================================



//============Resistances===================
//
struct resistances {

     int							poison;
     int							paralize;
     int							lightning;
     int							fire;
     int							ice;
     int							earth;

};
//
//==========================================



//============Config========================
//
struct config {
	
	int								screenWidth;					
	int								screenHeight;
	int								tileSize;						

	int								gameSpeed;
	std::string						levelMapName;

	sf::Keyboard::Key				controls1[4];
	sf::Keyboard::Key				controls2[4];

};
//
//==========================================


#endif
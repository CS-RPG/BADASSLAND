BADASS TALES of BADASSLAND v. 0.7.1 alpha

Paul Pushmin,
Leo Vetlugin

Follow us on GitHub:
https://github.com/CS-RPG/BADASSLAND


//===================================\\
New in 0.7.1:
(+) - New features.
(*) - Bug fixes.

PLEASE, don't mess with config and level files (input errors are not handled).

+ Right-Mouse Button click on an object to change view focus.
+ Loading key bindings from config file.
+ List of objects to be spawned on a level.
* Fixed object collision bug (objects now won't be spawned if they can't be spawned).
* Fixed game crash after someone is killed (std::vector out_of_range error).

\\===================================//


//===================================\\
Level file structure:

mapHeight mapWidth
*
* map itself
*
objectCount
coordinates(x, y) objID (see available objectIDs below)
\\===================================//


//===================================\\
Objects that can be spawned :		 ||
									 ||
Player,								 ||
Elf_Enemy,							 ||
Elf_Friendly,						 ||
Elf_Minion							 ||
\\===================================//


//===================================\\
In-Game controls:					 ||
									 ||
(coordinates = mouse coordinates)	 ||
Spawn Elf_Enemy - G					 ||
Spawn Elf_Friendly Elf - F			 ||
Spawn Elf_Minion - H (controls2)	 ||
									 ||
Player - controls1					 ||
									 ||
Right-Mouse Button - change focus to ||
a different game object.			 ||
\\===================================//

BADASS TALES of BADASSLAND v. 0.7.3.1 alpha

Paul Pushmin,
Leo Vetlugin

Follow us on GitHub:
https://github.com/CS-RPG/BADASSLAND

PLEASE, don't mess with config and level files (input errors are not handled).


//============CHANGELOG=========================\\
New in 0.7.3.1:
(+) - New features.
(*) - Bug fixes.

 * Fixed focus object changing bug while zooming is active.
-----------------------------------------------------------------
New in 0.7.3:
(+) - New features.
(*) - Bug fixes.

 + Zooming added - PgUp / PgDown.
 + Battle logs (in console).
-----------------------------------------------------------------
New in 0.7.2:
(+) - New features.
(*) - Bug fixes.

 * Fixed AI targeting bug.
-----------------------------------------------------------------
New in 0.7.1:
(+) - New features.
(*) - Bug fixes.

+ Right-Mouse Button click on an object to change view focus.
+ Loading key bindings from config file.
+ List of objects to be spawned on a level.
* Fixed object collision bug (objects now won't be spawned if they can't be spawned).
* Fixed game crash after someone is killed (std::vector out_of_range error).
-----------------------------------------------------------------


//============LEVEL FILE STRUCTURE==============\\

mapHeight mapWidth
*
* map itself
*
objectCount
objX objY objID (see available objectIDs below)


//============AVAILABLE GAMEOBJECT IDs==========\\
		
Player,
Elf_Enemy,
Elf_Friendly,
Elf_Minion


//============IN-GAME CONTROLS==================\\
								
(coordinates = mouse coordinates)	 
Spawn Elf_Enemy - G					 
Spawn Elf_Friendly Elf - F			 
Spawn Elf_Minion - H (controls2)	 
									 
Player - controls1					 
									 
Right-Mouse Button - change focus to 
a different game object.

PgUp / PgDown - Zoom In / Out


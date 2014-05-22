BADASS TALES of BADASSLAND v. 0.8.1 alpha

Paul Pushmin,
Leo Vetlugin

Follow us on GitHub:
https://github.com/CS-RPG/BADASSLAND


//============CHANGELOG=========================\\
New in 0.8.1:
(+) - New features.
(*) - Bug fixes.

 + Added MainMenu and buttons.
 + Loading FactionKarmaMap from file.
 + Loading objects from file.
 * Fixes object out of map range bug.
-----------------------------------------------------------------
New in 0.8:
(+) - New features.
(*) - Bug fixes.

 + Pathfinding for enemy AI introduced (though AI movements are not very smooth)/
 + Path highlighting (Keyboard - B).
 + Grid overlay introduced (Keyboard - L).
-----------------------------------------------------------------
New in 0.7.6.1:
(+) - New features.
(*) - Bug fixes.

 + Invincibility combat component for GOD.
 * Fixed stack overflow bug (GODs recursively attacking each other).
 * Fixed object attacking itself bug.
 * Fixed incorrect level file input.
-----------------------------------------------------------------
New in 0.7.6:
(+) - New features.
(*) - Bug fixes.

 + Player attack enabled.
 + Improved view / zoom.
 + GOD.
 * Fixed resizing division by zero bug.
-----------------------------------------------------------------

New in 0.7.5.1:
(+) - New features.
(*) - Bug fixes.

 + Fullscreen mode (in config).
-----------------------------------------------------------------
New in 0.7.5:
(+) - New features.
(*) - Bug fixes.

 * Fixed resizing bug.
 * Fixed incorrect input error handling (just config file).
-----------------------------------------------------------------
New in 0.7.4:
(+) - New features.
(*) - Bug fixes.

 + Customizable zoom rate (in config).
 * Fixed map collision detection accuracy bug.
-----------------------------------------------------------------
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

Left-Mouse Button - ATTACK!!!11

Super-God Mode - ~

PgUp / PgDown - Zoom In / Out


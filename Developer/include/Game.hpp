//Game.hpp
#ifndef _Game_hpp_included_
#define _Game_hpp_included_

#include <DataTypes.hpp>
#include <World.hpp>


//============Game==========================
//
class Game {
public:

									Game();

	void							run();
	void							update(sf::Time);
	void							processEvents();
	void							render();

	void							loadConfigFile(config&);

private:

	sf::RenderWindow				mWindow;
	int								mScreenWidth;					
	int								mScreenHeight;

	float							mInvincibilityTime;
	sf::Clock						mGameClock;
	sf::Clock						mInvincibilityClock;
	sf::Clock						mSpawnClock;
	sf::Time						mTimePerFrame;

	config							mConfig;
	std::vector<std::vector<int>>	mLevelMap;
			
	int								mTileSize;						
	int								mGameSpeed;
	float							mOffsetX;				//Map scrolling
	float							mOffsetY;				//offset.

	World							mWorld;

};
//
//==========================================


#endif
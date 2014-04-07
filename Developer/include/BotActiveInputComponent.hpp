//BotActiveInputComponent.hpp
#ifndef _BotActiveInputComponent_hpp_included_
#define _BotActiveInputComponent_hpp_included_

#include <SFML/Graphics.hpp>
#include <World.hpp>
#include <GameObject.hpp>

#include <iostream>

//============BotActiveInputComponent=======
//
class BotActiveInputComponent : public InputComponent {
public:

									BotActiveInputComponent();
	virtual void					update(GameObject&, World&);

};
//
//==========================================


#endif
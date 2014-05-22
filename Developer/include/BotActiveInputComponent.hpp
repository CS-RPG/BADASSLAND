//BotActiveInputComponent.hpp
#ifndef _BotActiveInputComponent_hpp_included_
#define _BotActiveInputComponent_hpp_included_

#include <World.hpp>
#include <GameObject.hpp>

#include <_includes_system.hpp>
#include <_includes_badass.hpp>

//============BotActiveInputComponent=======
class BotActiveInputComponent : public InputComponent {
public:

									BotActiveInputComponent();
	virtual void					update(GameObject& object, World& world, config& config);

};


#endif

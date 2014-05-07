//BotPassiveInputComponent.hpp
#ifndef _BotPassiveInputComponent_hpp_included_
#define _BotPassiveInputComponent_hpp_included_

#include <_includes_system.hpp>
#include <_includes_badass.hpp>

//============BotPassiveInputComponent======
//
class BotPassiveInputComponent : public InputComponent {
public:

									BotPassiveInputComponent(float changeDirectionFrequency);
	virtual void					update(GameObject& source, World& world, config& config);	

private:

};
//
//==========================================


#endif
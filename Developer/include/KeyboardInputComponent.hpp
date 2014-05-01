//KeyboardInputComponent.hpp
#ifndef _KeyboardInputComponent_hpp_included_
#define _KeyboardInputComponent_hpp_included_

#include <_includes_system.hpp>

#include <World.hpp>
#include <GameObject.hpp>

#include <InputComponent.hpp>
#include <_includes_badass.hpp>


//============KeyboardInputComponent========
//
class KeyboardInputComponent : public InputComponent {
public:

									KeyboardInputComponent(sf::Keyboard::Key*);
	virtual void					update(GameObject&, World&, config& config);

private:

	sf::Keyboard::Key*				mKeyBindings;

};
//
//==========================================


#endif
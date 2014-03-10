//KeyboardInputComponent.hpp
#ifndef _KeyboardInputComponent_hpp_included_
#define _KeyboardInputComponent_hpp_included_

#include <SFML/Graphics.hpp>
#include <World.hpp>
#include <GameObject.hpp>

#include <InputComponent.hpp>


//============KeyboardInputComponent========
//
class KeyboardInputComponent : public InputComponent {
public:

									KeyboardInputComponent(sf::Keyboard::Key*);
	virtual void					update(GameObject&, World&);

private:

	sf::Keyboard::Key*				mKeyBindings;

};
//
//==========================================


#endif
//KeyboardInputComponent.hpp
#ifndef _KeyboardInputComponent_hpp_included_
#define _KeyboardInputComponent_hpp_included_

#include <InputComponent.hpp>


//============KeyboardInputComponent========
//
class KeyboardInputComponent : public InputComponent {
public:

	virtual void					update(GameObject&, World&);

};
//
//==========================================


#endif
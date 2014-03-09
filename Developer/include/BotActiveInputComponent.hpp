//BotActiveInputComponent.hpp
#ifndef _BotActiveInputComponent_hpp_included_
#define _BotActiveInputComponent_hpp_included_


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
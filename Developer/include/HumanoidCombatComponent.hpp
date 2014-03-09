//HumanoidCombatComponent.hpp
#ifndef _HumanoidCombatComponent_hpp_included_
#define _HumanoidCombatComponent_hpp_included_


//============HumanoidCombatComponent=======
//
class HumanoidCombatComponent : public CombatComponent {
public:

									HumanoidCombatComponent(int, int, float, float, float);
	virtual void					update(GameObject&, World&);
	virtual void					attack(GameObject*);
	virtual void					takeDamage(float);
	//virtual void					heal(float);

};
//
//==========================================


#endif
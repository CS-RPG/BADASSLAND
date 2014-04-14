//HumanoidCombatComponent.hpp
#ifndef _HumanoidCombatComponent_hpp_included_
#define _HumanoidCombatComponent_hpp_included_


//============HumanoidCombatComponent=======
//
class HumanoidCombatComponent : public CombatComponent {
public:

									HumanoidCombatComponent(int HP, int maxHP, float damage, float attackRange, float attackSpeed);
	virtual void					update(GameObject& object, World& world);
	virtual void					attack(GameObject& attacker, GameObject& target);
	virtual void					takeDamage(GameObject& attacker, GameObject& target, float damage);
	//virtual void					heal(float);

};
//
//==========================================


#endif
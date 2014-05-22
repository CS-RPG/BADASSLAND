//HumanoidCombatComponent.hpp
#ifndef _HumanoidCombatComponent_hpp_included_
#define _HumanoidCombatComponent_hpp_included_

#include <_includes_badass.hpp>
#include <_includes_system.hpp>


//============HumanoidCombatComponent=======
class HumanoidCombatComponent : public CombatComponent {
public:

									HumanoidCombatComponent(int HP, int maxHP, float damage, float attackRange, float attackSpeed);
									HumanoidCombatComponent(objectCombat settings);
									//HumanoidCombatComponent(objectCombat settings, float hp);

	virtual void					update(GameObject& object, World& world);
	virtual void					attack(GameObject& attacker, GameObject& target);
	virtual void					takeDamage(GameObject& attacker, GameObject& target, float damage);
	//virtual void					heal(float);

};


#endif

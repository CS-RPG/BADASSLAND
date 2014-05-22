//InvincibleCombatComponent.hpp
#ifndef _InvincibleCombatComponent_hpp_included_
#define _InvincibleCombatComponent_hpp_included_

#include <_includes_badass.hpp>
#include <_includes_system.hpp>


//============InvincibleCombatComponent=====
class InvincibleCombatComponent : public CombatComponent {
public:

									InvincibleCombatComponent(int HP, int maxHP, float damage, float attackRange, float attackSpeed);
	virtual void					update(GameObject& object, World& world);
	virtual void					attack(GameObject& attacker, GameObject& target);
	virtual void					takeDamage(GameObject& attacker, GameObject& target, float damage);
	//virtual void					heal(float);

};


#endif

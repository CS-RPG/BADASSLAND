//CombatComponent.cpp
#include <World.hpp>

#include <CombatComponent.hpp>


//============CombatComponent===============
//
CombatComponent::~CombatComponent() {}

void CombatComponent::deleteFromAttackers(GameObject& object, World& world) {

	myGameObjectIter current = world.getGameObjects().begin();
	myGameObjectConstIter end = world.getGameObjects().end();
	int attackers = 0;

	for(; current != end; ++current) {
		/*
		if(*(current->getInput()->getTarget()) == object) {
			current->getInput()->setTargeting(false);
			current->getInput()->setPathExists(false);
			++attackers;
		}
		*/
	}
	//std::cout << attackers << '\n';

}

float CombatComponent::getHP() {
	return mHP;
}

float CombatComponent::getMaxHP() {
	return mMaxHP;
}

float CombatComponent::getDT() {
	return mDT;
}

bool CombatComponent::isAlive() {
	return mIsAlive;
}

bool CombatComponent::isMarkedForRemoval() {
	return mIsMarkedForRemoval;
}

float CombatComponent::getAttackRange() {
	return mAttackRange;
}

float CombatComponent::getAttackSpeed() {
	return mAttackSpeed;
}

float CombatComponent::getDamage() {
	return mDamage;
}

bool CombatComponent::isReadyToAttack() {

	if(mAttackClock.getElapsedTime().asSeconds() > 1 / mAttackSpeed)
		return true;
	else
		return false;

}

void CombatComponent::setHP(float HP) {
	mHP = HP;
}

void CombatComponent::setMaxHP(float maxHP) {
	mMaxHP = maxHP;
}

void CombatComponent::setDT(float DT) {
	mDT = DT;
}

void CombatComponent::kill() {
	mIsAlive = false;
}

void CombatComponent::revive() {
	mIsAlive = true;
}

void CombatComponent::setAlive(bool alive) {
	mIsAlive = alive;
}

void CombatComponent::setMarkedForRemoval(bool isMarkedForRemoval) {
	mIsMarkedForRemoval = isMarkedForRemoval;
}

void CombatComponent::setAttackRange(float attackRange) {
	mAttackRange = attackRange;
}

void CombatComponent::setAttackSpeed(float attackSpeed) {
	mAttackSpeed = attackSpeed;
}

void CombatComponent::setDamage(float damage) {
	mDamage = damage;
}

void CombatComponent::restartAttackClock() {
	mAttackClock.restart();
}

//GameObject.cpp
#include <GameObject.hpp>


//=================GameObject===============
//
GameObject::GameObject( InputComponent* input, 
						PhysicsComponent* physics,
						GraphicsComponent* graphics,
						CombatComponent* combat,
						SocialComponent* social )
:mInput(input),
mPhysics(physics),
mGraphics(graphics),
mCombat(combat),
mSocial(social) {

	setNoClip(false);

}

void GameObject::update(float deltaTime, struct config* config, World& world) {

	mInput->update(*this, world);
	mPhysics->update(*this, world, config->tileSize, deltaTime);
	mGraphics->update(*this, deltaTime);
	mCombat->update(*this, world);

}

InputComponent*& GameObject::getInput() {
	return mInput;
}

PhysicsComponent*& GameObject::getPhysics() {
	return mPhysics;
}

GraphicsComponent*& GameObject::getGraphics() {
	return mGraphics;
}

CombatComponent*& GameObject::getCombat() {
	return mCombat;
}

SocialComponent*& GameObject::getSocial() {
	return mSocial;
}

bool GameObject::isPlayer() {
	return mIsPlayer;
}

bool GameObject::isNoClip() {
	return mIsNoClip;
}

void GameObject::setInput(InputComponent* input) {
	mInput = input;
}

void GameObject::setPhysics(PhysicsComponent* physics) {
	mPhysics = physics;
}

void GameObject::setGraphics(GraphicsComponent* graphics) {
	mGraphics = graphics;
}

void GameObject::setCombat(CombatComponent* combat) {
	mCombat = combat;
}

void GameObject::setSocial(SocialComponent* social) {
	mSocial = social;
}

void GameObject::setPlayer(bool isPlayer) {
	mIsPlayer = isPlayer;
}

void GameObject::setNoClip(bool isNoClip) {
	mIsNoClip = isNoClip;
}
//
//==========================================


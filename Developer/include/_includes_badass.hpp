//includes_badass.hpp
#ifndef __includes_badass_hpp_
#define __includes_badass_hpp_

//=====================================
//
//			FONT - BIG.
//	http://patorjk.com/software/taag/
//
//=====================================


//============ENGINE========================
#include <GameObject.hpp>
#include <DataTypes.hpp>
#include <State.hpp>
#include <StateMachine.hpp>
#include <TextureHolder.hpp>


//============STATES========================
#include <World.hpp>


//   _____                                             _       
//  / ____|                                           | |      
// | |     ___  _ __ ___  _ __   ___  _ __   ___ _ __ | |_ ___ 
// | |    / _ \| '_ ` _ \| '_ \ / _ \| '_ \ / _ \ '_ \| __/ __|
// | |___| (_) | | | | | | |_) | (_) | | | |  __/ | | | |_\__ \
//  \_____\___/|_| |_| |_| .__/ \___/|_| |_|\___|_| |_|\__|___/
//                       | |                                   
//                       |_|
//

//============BASE==========================
#include <InputComponent.hpp>
#include <PhysicsComponent.hpp>
#include <GraphicsComponent.hpp>
#include <CombatComponent.hpp>
#include <SocialComponent.hpp>

//============INPUT=========================
#include <KeyboardInputComponent.hpp>
#include <BotActiveInputComponent.hpp>
#include <BotPassiveInputComponent.hpp>

//============PHYSICS=======================
#include <DynamicPhysicsComponent.hpp>
#include <NoClipPhysicsComponent.hpp>

//============GRAPHICS======================
#include <HumanoidGraphicsComponent.hpp>

//============COMBAT========================
#include <HumanoidCombatComponent.hpp>
#include <InvincibleCombatComponent.hpp>

//============SOCIAL========================
#include <HumanoidSocialComponent.hpp>


#endif
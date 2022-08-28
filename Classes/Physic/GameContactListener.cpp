//
//  GameContactListener.cpp
//  Letgobirdy
//
//  Created by Harry Nguyen on 5/22/15.
//
//

#include "GameContactListener.h"
#include "Character.h"
#include "GameDefineState.h"
#include "Barrier.h"
#include "Monster.h"


GameContactListener::GameContactListener() {}

void GameContactListener::setDelegate(GameDelegate* mDelegate) {
  this->delegate = mDelegate;
}
void GameContactListener::BeginContact(b2Contact* contact) {
  if (contact && contact->IsTouching()) {
    b2Body* bodyA = contact->GetFixtureA()->GetBody();
    b2Body* bodyB = contact->GetFixtureB()->GetBody();
    if (bodyB->GetUserData() !=NULL  &&
        bodyA->GetUserData() !=NULL) {
      Character* player = dynamic_cast<Character*>((Ref*)bodyA->GetUserData());
      if(player) {
        this->processCollisionPlayerVsObject(bodyA, bodyB);
      } else {
        this->processCollisionPlayerVsObject(bodyB, bodyA);
      }
    } else {
      b2Body* characterBody;
      b2Body* objectBody;
      if (bodyA->GetUserData() != NULL &&
          objectBody->GetUserData() == NULL) {
        characterBody = bodyA;
        objectBody = bodyB;
      } else {
        characterBody = bodyB;
        objectBody = bodyA;
      }
      if(objectBody->GetFixtureList()[0].GetFilterData().categoryBits == kWallCatBit) {
        Character* character = (Character*)characterBody->GetUserData();
        character->contactWithWall();
        if(delegate) { delegate->blockTouchWhileContactWithWall();}
      }
      if(objectBody->GetFixtureList()[0].GetFilterData().categoryBits == kGroundCatBit) {
        Character* character = (Character*)characterBody->GetUserData();
        character->contactWithGround();
        if(delegate) { delegate->characterJumpFailed();}
      }
    }
  }
}

void GameContactListener::processCollisionPlayerVsObject(b2Body* characterBody, b2Body* barrierBody) {
  Character* character = (Character*)characterBody->GetUserData();
  if(barrierBody->GetFixtureList()[0].GetFilterData().categoryBits == kNextBarrierCatBit) {
    Barrier* barrier = (Barrier*)barrierBody->GetUserData();
    barrier->runActionWhenContactWithCharacter();
    barrier->makeAsPassedBarrier();
    character->contactWithBarrier(true);
    if(delegate) { delegate->characterJumpSuccess();}
  } else {
    character->contactWithBarrier(false);
    if(delegate) { delegate->characterJumpFailed();}
  }
}

void GameContactListener::EndContact(b2Contact* contact) {}
void GameContactListener::PreSolve(b2Contact *contact, const b2Manifold *oldManifold) {}

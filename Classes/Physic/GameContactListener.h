//
//  GameContactListener.h
//  Letgobirdy
//
//  Created by Harry Nguyen on 5/22/15.
//
//

#ifndef __Letgobirdy__GameContactListener__
#define __Letgobirdy__GameContactListener__

#include "cocos2d.h"
#include "Character.h"
#include <Box2D/Box2D.h>

class GameContactListener : public b2ContactListener {
private:
  GameDelegate* delegate;
public:
  GameContactListener();
  void BeginContact(b2Contact* contact); // When we first contact
  void EndContact(b2Contact* contact); // When we end contact
  void PreSolve(b2Contact* contact, const b2Manifold* oldManifold);
  void setDelegate(GameDelegate* mDelegate);
  void processCollisionPlayerVsObject(b2Body* characterBody, b2Body* objectBody);
};

#endif /* defined(__Letgobirdy__GameContactListener__) */

//
//  Barrier.h
//  Letgobirdy
//
//  Created by Harry Nguyen on 12/13/16.
//
//

#ifndef Barrier_h
#define Barrier_h

#include <stdio.h>
#include <Box2D/Box2D.h>
#include "BaseObject.h"

class Barrier: public BaseObject {
protected:
  b2World* worldPhysicManger;
  b2Body * bodyBarrier;
  b2Vec2 positionUpdateBarrier;
public:
  Barrier();
  virtual ~Barrier();
  static Barrier* createSpriteWithFileName(const char* file);
  static Barrier* createSpriteWithFrameName(const char* file);
  void initBodyWithPhysicManager(b2World* physicWorld, const Vec2& position);
  void runActionWhenContactWithCharacter();
  void activeUpdatePhysic();
  void makeAsPassedBarrier();
};
#endif /* Barrier_h */

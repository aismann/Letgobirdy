//
//  Monster.h
//  Letgobirdy
//
//  Created by Harry Nguyen on 12/13/16.
//
//

#ifndef Monster_h
#define Monster_h

#include <stdio.h>
#include "Barrier.h"
#include <Box2D/Box2D.h>
#include <spine/spine-cocos2dx.h>
#include "spine/spine-cocos2dx.h"
using namespace spine;

class Monster: public Node {
private:
  bool isFirstRunning;
  bool isStaticMonster;
  int type;
  int runningDirection;
  float veloCity;
public:
  Monster();
  virtual ~Monster();
  void initMonsterWithSpine();
  virtual void onEnter();
  virtual void onExit();
  virtual void update(float dt);
  void setVelocity(float veloCity);
  void setIsStaticMonster(bool isStaticMonster);
  
  void setMonsterType(int type);
  int getTypeMonster();
  void changeRunActionWitAnimation();
  void addPrisoner();
};

#endif /* Monster_h */

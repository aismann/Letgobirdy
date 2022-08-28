//
//  Character.h
//  Letgobirdy
//
//  Created by Harry Nguyen on 12/13/16.
//
//

#ifndef Character_h
#define Character_h

#include <stdio.h>
#include <Box2D/Box2D.h>
#include <spine/spine-cocos2dx.h>
#include "cocos2d.h"
#include "GameDelegate.h"
USING_NS_CC;

class Character: public Node {
private:
  spine::SkeletonAnimation* skeletonCharacter;
  b2World* worldPhysicManger;
  b2Body * bodyCharacter;
  float velocity;
  float currentPositionYBeforeJump;
  float nextPositionY;
  int runningDirection;
  GameDelegate* delegate;
public:
  int status;
public:
  Character();
  virtual ~Character();
  void update(float dt);
  void createCharacterBySpine();
  void defineBodyCharacter(b2World* worldPhysic);
  
  void setDelegate(GameDelegate* mDelegate);
  void setNextPositionY(float yPosition);
  void saveCurrentPositionYBeforeJump();
  
  void singleJump();
  void doubleJump();
  bool checkDieCharacter(const Vec2& position);
  
  /// Contact
  void contactWithGround();
  void contactWithWall();
  void contactWithBarrier(bool isNextLevel);
  void dieCharacter();
};

#endif /* Character_h */

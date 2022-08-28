//
//  GameManager.h
//  Letgobirdy
//
//  Created by Harry Nguyen on 1/16/17.
//
//

#ifndef GameManager_h
#define GameManager_h

#include <stdio.h>

#include "cocos2d.h"
#include "Monster.h"
#include "Barrier.h"
#include "Character.h"
#include <Box2D/Box2D.h>
USING_NS_CC;

class GameManager {
public:
  b2World* worldPhysicManger;
  std::vector<Monster*> arrayMonster;
  std::vector<Barrier*> arrayBarrier;
  bool isGameOver;
  Character* character;
public:
  GameManager();
  virtual ~GameManager();
  void addBarrierAndMonster(Barrier* barrier , Monster* monster);
  void update(float dt);
  void clearDataForGame();
  void setPhysic(b2World* world);
  void setCharacter(Character* character);
  void setVisibleForBarrierAndMonster();
  int getCountBarrier();
  void restartGameManager();
};

#endif /* GameManager_h */

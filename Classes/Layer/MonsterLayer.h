//
//  MonsterLayer.h
//  Letgobirdy
//
//  Created by Harry Nguyen on 1/17/17.
//
//

#ifndef MonsterLayer_h
#define MonsterLayer_h

#include <stdio.h>
#include "cocos2d.h"
#include "GameManager.h"
#include <Box2D/Box2D.h>
USING_NS_CC;

class MonsterLayer: public Layer {
public:
  std::vector<int> arrayType;
  Size visibleSize ;
  Vec2 origin;
  GameManager* gameManager;
  b2World* physicWorld;
  Sprite* landingSprite;
public:
  
  void setGameManager(GameManager* gameManager);
  CREATE_FUNC(MonsterLayer);
  virtual ~MonsterLayer();
  virtual bool init();
  void update(float dt);
  void createMonstersAndBarriersLevel(int level, int counterMonster, float startPointY, int barrierIndexWithNoMonster);
  void initPhysicForMap(b2World* physic);
  void initPhysicLanding();
  int randomMonsterType();
};

#endif /* MonsterLayer_h */

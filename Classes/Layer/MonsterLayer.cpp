//
//  MonsterLayer.cpp
//  Letgobirdy
//
//  Created by Harry Nguyen on 1/17/17.
//
//

#include "MonsterLayer.h"
#include "Barrier.h"
#include "Monster.h"
#include <cstdlib>
#include "GameManager.h"
#include "GameDefineState.h"
#include "TestBody.h"

MonsterLayer::~MonsterLayer() {}

bool MonsterLayer::init() {
  if(!Layer::init()) {
    return false;
  }
  visibleSize = Director::getInstance()->getWinSize();
  origin = Director::getInstance()->getVisibleOrigin();
  
  landingSprite = Sprite::create("landing.png");
  landingSprite->setPosition(Vec2(visibleSize.width/2, 0.0));
  this->addChild(landingSprite,1);
  return true;
}

void MonsterLayer::setGameManager(GameManager* gameManager) {
  this->gameManager = gameManager;
}

void MonsterLayer::initPhysicForMap(b2World* physic) {
  physicWorld = physic;
  gameManager->setPhysic(physicWorld);
  this->initPhysicLanding();
}

void MonsterLayer::update(float dt) {
  if(gameManager == NULL) { return; }
  gameManager->update(dt);
}

int MonsterLayer::randomMonsterType(){
  int type = 0;
  if(arrayType.empty()) {
    int numberType0 = rand() %2+3;
    int numberType1 = rand() %4+3;
    for (int i = 0; i < numberType0; i++) {
      arrayType.push_back(0);
    }
    for (int i = 0; i < numberType1; i++) {
      arrayType.push_back(1);
    }
  } else {
    int numberArray = rand()% arrayType.size();
    type = arrayType.at(numberArray);
    int temp = arrayType.at(numberArray);
    arrayType.at(numberArray) = arrayType.at(arrayType.size()-1);
    arrayType.at(arrayType.size()-1) = temp;
    arrayType.pop_back();
  }
  return type;
}


void MonsterLayer::createMonstersAndBarriersLevel(int level, int counterMonster, float startPointY, int barrierIndexWithNoMonster) {
  srand( (unsigned)time(NULL) );
  int intWidth1 = visibleSize.width*3/4;
  int intWidth2 = visibleSize.width/4;
  
  int randomStaticMonsterType1 = rand() % 3+2;
  int randomStaticMonsterType2 = rand() % 5+6;
  int hiddenIndex = rand() % 7+3;
  
  float velocityMonster = VELOCITY_MONSTER;
  if(level >= 2) {
    hiddenIndex = 1000;
    velocityMonster = VELOCITY_MONSTER + 0.4f;
  }
  
  for (int i = 0; i < counterMonster; i++) {
    int randompositionX1 =  0 + (rand() % int(intWidth1));
    int randompositionX2 = intWidth2 + (rand() % (int)(visibleSize.width - intWidth2));
    
    Barrier* barrier = Barrier::createSpriteWithFileName("tree.png");
    barrier->setPosition(Vec2(visibleSize.width/2,startPointY + HEIGHT_DISTANCE_BETWEEN_BARRIER +i*HEIGHT_DISTANCE_BETWEEN_BARRIER));
    this->addChild(barrier,200);
    int monsterType = this->randomMonsterType();
    
    if(i != barrierIndexWithNoMonster && i != hiddenIndex) {
      Monster* monster = new Monster();
      if(monsterType == 0) {
        monster->setPosition(Vec2(randompositionX1, barrier->getPositionY()));
      } else{
        monster->setPosition(Vec2(randompositionX2, barrier->getPositionY()));
      }
      if(velocityMonster + i/40 > RUN_VELOCITY_CHARACTER){
        velocityMonster = RUN_VELOCITY_CHARACTER - i/40;
      }
      monster->setVelocity(velocityMonster + i/40);
      monster->setMonsterType(monsterType);
      monster->initMonsterWithSpine();
      if(i == randomStaticMonsterType1 || i == randomStaticMonsterType2) {
        monster->setIsStaticMonster(true);
        if(i == randomStaticMonsterType1){
          monster->setScaleX(-1);
          monster->setPosition(Vec2(intWidth1*0.9, barrier->getPositionY()));
        } else{
          monster->setPosition(Vec2(intWidth2*1.2, barrier->getPositionY()));
        }
        monster->addPrisoner();
      } else {
        monster->setIsStaticMonster(false);
      }
      this->addChild(monster,200);
      gameManager->addBarrierAndMonster(barrier, monster);
    } else {
      gameManager->addBarrierAndMonster(barrier, NULL);
    }
  }
}

void MonsterLayer::initPhysicLanding() {
  /*setup bottom land*/
  b2PolygonShape shapeLand;
  shapeLand.SetAsBox((landingSprite->getContentSize().width)/PTM_RATIO,
                     (landingSprite->getContentSize().height/2.0) /PTM_RATIO);
  b2FixtureDef WallFixDef;
  WallFixDef.density = 0;
  WallFixDef.friction = 0;
  WallFixDef.restitution  = 0.0;
  WallFixDef.shape = &shapeLand;
  WallFixDef.filter.categoryBits = kGroundCatBit;
  WallFixDef.filter.maskBits = kGroundMaskBit;
  b2BodyDef landBodyDef;
  landBodyDef.type = b2_staticBody;
  landBodyDef.position.Set(0.0/PTM_RATIO, 0.0/PTM_RATIO);
  b2Body* body = physicWorld->CreateBody(&landBodyDef);
  body->CreateFixture(&WallFixDef);
  
  /*setup left land*/
  b2PolygonShape shapeLeftLand;
  shapeLeftLand.SetAsBox((10)/PTM_RATIO, (Director::getInstance()->getWinSize().height*1000) /PTM_RATIO);
  b2FixtureDef WallLeftFixDef;
  WallLeftFixDef.density = 0.0f;
  WallLeftFixDef.friction = 0;
  WallLeftFixDef.restitution  = -10.0;
  WallLeftFixDef.shape = &shapeLeftLand;
  WallLeftFixDef.filter.categoryBits = kWallCatBit;
  WallLeftFixDef.filter.maskBits = kWallMasBit;
  b2BodyDef landLeftBodyDef;
  landLeftBodyDef.type = b2_staticBody;
  landLeftBodyDef.position.Set((visibleSize.width)/PTM_RATIO, (visibleSize.height/2)/PTM_RATIO);
  b2Body* bodyLeft = physicWorld->CreateBody(&landLeftBodyDef);
  bodyLeft->CreateFixture(&WallLeftFixDef);
  
  /*setup right land*/
  b2PolygonShape shapeRightLand;
  shapeRightLand.SetAsBox((10)/PTM_RATIO, (Director::getInstance()->getWinSize().height*1000) /PTM_RATIO);
  b2FixtureDef WallRightFixDef;
  WallRightFixDef.density = 0.0f;
  WallRightFixDef.friction = 0;
  WallRightFixDef.restitution  = -10.0;
  WallRightFixDef.shape = &shapeRightLand;
  WallRightFixDef.filter.categoryBits = kWallCatBit;
  WallRightFixDef.filter.maskBits = kWallMasBit;
  b2BodyDef landRightBodyDef;
  landRightBodyDef.type = b2_staticBody;
  landRightBodyDef.position.Set(0/PTM_RATIO, (visibleSize.height/2) /PTM_RATIO);
  b2Body* bodyRightLand = physicWorld->CreateBody(&landRightBodyDef);
  bodyRightLand->CreateFixture(&WallRightFixDef);
  
  //  TestBody* testBody = new TestBody();
  //  physicWorld->SetDebugDraw(testBody);
  physicWorld->SetAllowSleeping(false);
}







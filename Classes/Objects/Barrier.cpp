//
//  Barrier.cpp
//  Letgobirdy
//
//  Created by Harry Nguyen on 12/13/16.
//
//

#include "Barrier.h"
#include "GameDefineState.h"

Barrier::Barrier() {
  isPhysicBody =  false;
}

Barrier::~Barrier() {}

Barrier* Barrier::createSpriteWithFileName(const char* file) {
  Barrier* barrier = new Barrier();
  barrier->createWithFileName(file);
  return barrier;
}

Barrier* Barrier::createSpriteWithFrameName(const char* file) {
  Barrier* barrier = new Barrier();
  barrier->createWithFrameName(file);
  return barrier;
}

void Barrier::initBodyWithPhysicManager(b2World* physicWorld, const Vec2& position) {
  if(isPhysicBody) { return; }
  positionUpdateBarrier = b2Vec2((position.x)/PTM_RATIO ,(position.y - this->getContentSize().height*0.1f)/PTM_RATIO);
  isPhysicBody = true;
  b2BodyDef barrierBodyDef;
  barrierBodyDef.type = b2_staticBody;
  barrierBodyDef.userData = this;
  barrierBodyDef.bullet = true;
  barrierBodyDef.position.Set((position.x)/PTM_RATIO ,(position.y - this->getContentSize().height*2)/PTM_RATIO);
  worldPhysicManger = physicWorld;
  bodyBarrier = worldPhysicManger->CreateBody(&barrierBodyDef);
  bodyBarrier->SetFixedRotation(false);
  
  b2PolygonShape barrier_shape;
  barrier_shape.SetAsBox(this->getContentSize().width/2/PTM_RATIO, 0.4/PTM_RATIO);
  b2FixtureDef barrier_FixDef;
  barrier_FixDef.density = 0;
  barrier_FixDef.friction = 0;
  barrier_FixDef.restitution  = 0.0;
  barrier_FixDef.filter.categoryBits = kNextBarrierCatBit;
  barrier_FixDef.filter.maskBits = kNextBarrierMaskBit;
  barrier_FixDef.shape = &barrier_shape;
  bodyBarrier->CreateFixture(&barrier_FixDef);
}

void Barrier::activeUpdatePhysic() {
  if (!isPhysicBody) { return; }
  auto action1 = DelayTime::create(0.2f);
  auto action2 = CallFunc::create([=](){
    bodyBarrier->SetTransform(positionUpdateBarrier,0.0f);
  });
  this->runAction(Sequence::create(action1,action2, NULL));
}

void Barrier::runActionWhenContactWithCharacter() {
  float position_BeforeRun = this->getPositionY() - 15.0f;
  float position_AfterRun =  position_BeforeRun + 15.0f;
  auto action1 = MoveTo::create(0.1, Vec2(this->getPositionX(), position_BeforeRun));
  auto action2 = MoveTo::create(0.1, Vec2(this->getPositionX(), position_AfterRun));
  this->runAction(Sequence::create(action1, action2, NULL));
}


void Barrier::onEnter() {
  BaseObject::onEnter();
}

void Barrier::onExit() {
  BaseObject::onExit();
}

void Barrier::makeAsPassedBarrier() {
  b2Filter filter = bodyBarrier->GetFixtureList()[0].GetFilterData();
  filter.categoryBits = kPassedBarrierCatBit;
  filter.maskBits = kPassedBarrierMaskBit;
  bodyBarrier->GetFixtureList()[0].SetFilterData(filter);
}

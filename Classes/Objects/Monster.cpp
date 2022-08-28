//
//  Monster.cpp
//  Letgobirdy
//
//  Created by Harry Nguyen on 12/13/16.
//
//

#include "Monster.h"
#include "GameDefineState.h"

Monster::Monster() {
  isFirstRunning = true;
}

Monster::~Monster() {}

void Monster::onEnter() {
  Node::onEnter();
}

void Monster::onExit() {
  Node::onExit();
}

void Monster::setVelocity(float veloCity) {
  this->veloCity = veloCity;
}

void Monster::setIsStaticMonster(bool isStaticMonster) {
  this->isStaticMonster = isStaticMonster;
}

void Monster::addPrisoner(){
  Sprite* prisoner;
  if(type == 0) {
    prisoner = Sprite::create("cage_cat.png");
  } else {
    prisoner = Sprite::create("cage_pig.png");
  }
  prisoner->setPosition(Vec2(0, prisoner->getContentSize().height/2));
  this->addChild(prisoner,1);
}

void Monster::update(float dt) {
  if(isStaticMonster == true) { return; }
  if(isFirstRunning) {
    if(this->getPositionX() > Director::getInstance()->getWinSize().width/2) {
      this->setPositionX(this->getPositionX() + veloCity);
      this->setScaleX(1);
    }
    if(this->getPositionX() <= Director::getInstance()->getWinSize().width/2){
      this->setPositionX(this->getPositionX() - veloCity);
      this->setScaleX(-1);
    }
    if(this->getPositionX() < - this->getContentSize().width/2 ||
       this->getPositionX() >= Director::getInstance()->getWinSize().width + this->getContentSize().width/2) {
      isFirstRunning = false;
    }
  } else {
    if(this->getPositionX() >= Director::getInstance()->getWinSize().width + this->getContentSize().width/2) {
      runningDirection = LEFT_RIGHT;
    }
    if( this->getPositionX() <= - this->getContentSize().width/2) {
      runningDirection = RIGHT_LEFT;
    }
    if(runningDirection == LEFT_RIGHT) {
      this->setPositionX(this->getPositionX() -  veloCity);
      this->setScaleX(-1);
    }
    if(runningDirection == RIGHT_LEFT) {
      this->setPositionX(this->getPositionX() +  veloCity);
      this->setScaleX(1);
    }
  }
}

void Monster::setMonsterType(int type) {
  this->type = type;
}

int Monster::getTypeMonster() {
  return type;
}

void Monster::initMonsterWithSpine() {
  if (type == 0) {
    spine::SkeletonAnimation* skeletonNode = spine::SkeletonAnimation::createWithJsonFile("cat.json",
                                                                                          "cat.atlas",
                                                                                          RATIO_SCALE_MONSTER_SKELETON);
    skeletonNode->clearTrack();
    skeletonNode->setAnimation(0, "walk", true);
    this->addChild(skeletonNode);
  }
  
  if(type == 1){
    spine::SkeletonAnimation* skeletonNode = spine::SkeletonAnimation::createWithJsonFile("pig.json",
                                                                                          "pig.atlas",
                                                                                          RATIO_SCALE_MONSTER_SKELETON);
    skeletonNode->clearTrack();
    skeletonNode->setAnimation(0, "walk", true);
    this->addChild(skeletonNode);
  }
}

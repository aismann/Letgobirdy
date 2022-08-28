//
//  BaseObject.cpp
//  Letgobirdy
//
//  Created by Harry Nguyen on 12/13/16.
//
//

#include "BaseObject.h"
#include "GameDefineState.h"

BaseObject::BaseObject() {
  isPhysicBody = false;
}

BaseObject::~BaseObject() {}

void BaseObject::createWithFileName(const char* fileName) {
  Sprite::initWithFile(fileName);
}

void BaseObject::createWithFrameName(const char* fileName) {
  Sprite::initWithSpriteFrameName(fileName);
}

void BaseObject::onEnter() {
  Sprite::onEnter();
}

void BaseObject::onExit() {
  Sprite::onExit();
}

void BaseObject::update(float dt) {}

void BaseObject::createWithAnimation() {}

void BaseObject::setVelocity(float velocity) {
  this->veloCity = velocity;
}

float BaseObject::getVeloCity() {
  return veloCity;
}

void BaseObject::setNewState(int newState) {
  this->state = newState;
}

int BaseObject::getStateCurrent() {
  return state;
}

void BaseObject::setFlipSprite(bool isFlip) {
  this->isFlip = isFlip;
  if(isFlip) {
    this->setFlippedX(true);
  } else{
    this->setFlippedX(false);
  }
}

void BaseObject::setEnableUpdate(bool isUpdateStatus) {
  this->isUpdateStatus = isUpdateStatus;
}

bool BaseObject::getEnableUpdate() {
  return isUpdateStatus;
}

void BaseObject::setPositionObject(const Vec2& pos) { }

void BaseObject::setScaleVeloCity(float scale) {
  this->veloCity *= scale;
}

bool BaseObject::getIsPhysicBody() {
  return isPhysicBody;
}

void BaseObject::setIsPhysicBody(bool isPhysicBody) {
  this->isPhysicBody = isPhysicBody;
}

Vec2 BaseObject::getPositionObject() {
  return this->getPosition();
}

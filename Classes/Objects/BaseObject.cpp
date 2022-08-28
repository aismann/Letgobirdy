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

bool BaseObject::getIsPhysicBody() {
  return isPhysicBody;
}

void BaseObject::setIsPhysicBody(bool isPhysicBody) {
  this->isPhysicBody = isPhysicBody;
}

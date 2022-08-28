
//
//  Character.cpp
//  Letgobirdy
//
//  Created by Harry Nguyen on 12/13/16.
//
//

#include "Character.h"
#include "GameDefineState.h"
#include "SimpleAudioEngine.h"

Character::Character() {
  velocity = VELOCITY_CHARACTER;
  status = RUNNING;
  runningDirection = LEFT_RIGHT;
}

Character::~Character() {}

void Character::setDelegate(GameDelegate* mDelegate) {
  this->delegate = mDelegate;
}

void Character::update(float dt) {
  switch (status) {
    case RUNNING:
      if (this->getPositionX() >= Director::getInstance()->getWinSize().width - (WIDTH_RUN_BUFFER)) {
        runningDirection = RIGHT_LEFT;
      } else if(this->getPositionX() <= (WIDTH_RUN_BUFFER)) {
        runningDirection = LEFT_RIGHT;
      }
      if (runningDirection == RIGHT_LEFT) {
        this->setPositionX(this->getPositionX() - velocity);
        this->setScaleX(-1);
      } else {
        this->setPositionX(this->getPositionX() + velocity);
        this->setScaleX(1);
      }
      bodyCharacter->SetTransform(b2Vec2(this->getPositionX()/PTM_RATIO,
                                         (this->getPositionY() + BUFFER_SIZE_HEIGHT/2)/PTM_RATIO), 0.0f);
      break;
    case JUMPING:
      this->setPosition(Vec2(bodyCharacter->GetPosition().x*PTM_RATIO,
                             (bodyCharacter->GetPosition().y)*PTM_RATIO - BUFFER_SIZE_HEIGHT/2));
    default:
      break;
  }
}

void Character::saveCurrentPositionYBeforeJump() {
  currentPositionYBeforeJump = this->getPositionY();
}

void Character::setNextPositionY(float yPosition) {
  nextPositionY = yPosition;
}

void Character::singleJump() {
  if(bodyCharacter == NULL) { return; }
  CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("jump.mp3");
  skeletonCharacter->clearTrack();
  skeletonCharacter->setAnimation(0, "jump", false);
  
  b2Filter filter = bodyCharacter->GetFixtureList()[0].GetFilterData();
  filter.categoryBits = kCharacterCatBit;
  filter.maskBits = kCharacterMaskBit;
  bodyCharacter->GetFixtureList()[0].SetFilterData(filter);
  
  switch (runningDirection) {
    case RIGHT_LEFT:
      bodyCharacter->SetLinearVelocity(b2Vec2(-VELOCITY_JUMP_X*SCALE_X_BIRD, VELOCITY_SINGLE_JUMP_Y*SCALE_Y_BIRD));
      break;
    default:
      bodyCharacter->SetLinearVelocity(b2Vec2(VELOCITY_JUMP_X*SCALE_X_BIRD, VELOCITY_SINGLE_JUMP_Y*SCALE_Y_BIRD));
      break;
  }
  status = JUMPING;
}

void Character::doubleJump() {
  if(bodyCharacter == NULL) { return; }
  if(this->getPositionY() >= nextPositionY) {
    b2Filter filter = bodyCharacter->GetFixtureList()[0].GetFilterData();
    filter.categoryBits = kJumpingCharacterCatBit;
    filter.maskBits = kJumpingCharacterMaskBit;
    bodyCharacter->GetFixtureList()[0].SetFilterData(filter);
    switch (runningDirection) {
      case RIGHT_LEFT:
        bodyCharacter->SetLinearVelocity(b2Vec2(-VELOCITY_JUMP_X*SCALE_X_BIRD, VELOCITY_DOUBLE_JUMP_Y*SCALE_Y_BIRD));
        break;
      default:
        bodyCharacter->SetLinearVelocity(b2Vec2(VELOCITY_JUMP_X*SCALE_X_BIRD, VELOCITY_DOUBLE_JUMP_Y*SCALE_Y_BIRD));
        break;
    }
  } else {
    if(delegate) { delegate->characterJumpFailed(); }
  }
}

void Character::contactWithGround() {
  if(status == JUMPING) {
    skeletonCharacter->clearTrack();
    skeletonCharacter->setAnimation(0, "walk", true);
    this->setPositionY(currentPositionYBeforeJump);
  }
  status = RUNNING;
}

void Character::contactWithWall() {
  if(status != JUMPING) { return; }
  switch (runningDirection) {
    case RIGHT_LEFT:
      this->setScaleX(1);
      bodyCharacter->SetLinearVelocity(b2Vec2(VELOCITY_JUMP_X*SCALE_X_BIRD, VELOCITY_JUMP_X*SCALE_Y_BIRD));
      runningDirection = LEFT_RIGHT;
      break;
    default:
      this->setScaleX(-1);
      bodyCharacter->SetLinearVelocity(b2Vec2(-VELOCITY_JUMP_X*SCALE_X_BIRD, VELOCITY_JUMP_X*SCALE_Y_BIRD));
      runningDirection = RIGHT_LEFT;
      break;
  }
}

void Character::contactWithBarrier(bool isNextLevel) {
  if(status != JUMPING) { return; }
  skeletonCharacter->clearTrack();
  skeletonCharacter->setAnimation(0, "walk", true);
  status = RUNNING;
  if(isNextLevel) {
    this->setPositionY(nextPositionY);
  } else {
    this->setPositionY(currentPositionYBeforeJump);
  }
}

void Character::dieCharacter() {
  skeletonCharacter->clearTrack();
  skeletonCharacter->setAnimation(0, "die", false);
  status = DIE;
}

bool Character::checkDieCharacter(const Vec2& position) {
  if(bodyCharacter == NULL) { return false;}
  Vec2 centerCharacter = Vec2(bodyCharacter->GetPosition().x*PTM_RATIO,
                              bodyCharacter->GetPosition().y*PTM_RATIO);
  float distance = centerCharacter.distance(position);
  return distance < BUFFER_SIZE_HEIGHT + 10;
}

void Character::defineBodyCharacter(b2World* worldPhysic) {
  b2BodyDef characterBodyDef;
  characterBodyDef.type = b2_dynamicBody;
  characterBodyDef.userData = this;
  characterBodyDef.bullet = true;
  characterBodyDef.position.Set((this->getPositionX())/PTM_RATIO ,(this->getPositionY() + BUFFER_SIZE_HEIGHT/2)/PTM_RATIO);
  worldPhysicManger = worldPhysic;
  bodyCharacter = worldPhysicManger->CreateBody(&characterBodyDef);
  bodyCharacter->SetFixedRotation(false);
  
  b2CircleShape circleForCharacter;
  circleForCharacter.m_radius = (22)/PTM_RATIO;
  b2FixtureDef c_FixDef;
  c_FixDef.density = 0;
  c_FixDef.friction = 0.0;
  c_FixDef.restitution = 0.0;
  c_FixDef.filter.categoryBits = kCharacterCatBit;
  c_FixDef.filter.maskBits = kCharacterMaskBit;
  c_FixDef.shape = &circleForCharacter;
  bodyCharacter->CreateFixture(&c_FixDef);
}

void Character::createCharacterBySpine() {
  skeletonCharacter = spine::SkeletonAnimation::createWithJsonFile("bird.json", "bird.atlas", RATIO_SCALE_CHARACTER);
  skeletonCharacter->setPosition(Vec2(0,0));
  skeletonCharacter->setAnimation(0, "walk", true);
  this->addChild(skeletonCharacter);
}


//
//  BaseObject.h
//  Letgobirdy
//
//  Created by Harry Nguyen on 12/13/16.
//
//

#ifndef BaseObject_h
#define BaseObject_h

#include <stdio.h>
#include "cocos2d.h"
#include <Box2D/Box2D.h>
USING_NS_CC;

class BaseObject : public Sprite {
protected:
  bool isPhysicBody;
  int state;
  bool isFlip;
  float veloCity;
  bool isUpdateStatus;
public:
  BaseObject();
  virtual ~BaseObject();
  virtual void createWithFileName(const char* fileName);
  virtual void createWithFrameName(const char* fileName);
  virtual void createWithAnimation();
  void setNewState(int newState);
  int getStateCurrent();
  void setFlipSprite(bool isFlip);
  void onEnter();
  void onExit();
  virtual void update(float dt);
  void setPositionObject(const Vec2& pos);
  Vec2 getPositionObject();
  
  void setVelocity(float velocity);
  float getVeloCity();
  void setScaleVeloCity(float scale);
  
  void setIsPhysicBody(bool isPhysicBody);
  bool getIsPhysicBody();
  
  void setEnableUpdate(bool isUpdateStatus);
  bool getEnableUpdate();
  
  
};


#endif /* BaseObject_h */

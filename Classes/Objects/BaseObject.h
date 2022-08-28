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
public:
  BaseObject();
  virtual ~BaseObject();
  virtual void createWithFileName(const char* fileName);
  virtual void createWithFrameName(const char* fileName);
  void setIsPhysicBody(bool isPhysicBody);
  bool getIsPhysicBody();
};


#endif /* BaseObject_h */

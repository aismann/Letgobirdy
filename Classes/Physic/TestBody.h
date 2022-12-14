//
//  TestBody.h
//  Letgobirdy
//
//  Created by Harry Nguyen on 5/26/15.
//
//

#ifndef __Letgobirdy__TestBody__
#define __Letgobirdy__TestBody__

#include "cocos2d.h"
#include <Box2D/Box2D.h>

USING_NS_CC;
using namespace cocos2d;

class TestBody: public b2Draw {
public:
  TestBody();
  /// Draw a closed polygon provided in CCW order.
  virtual void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color);
  
  /// Draw a solid closed polygon provided in CCW order.
  virtual void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color);
  
  /// Draw a circle.
  virtual void DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color);
  
  /// Draw a solid circle.
  virtual void DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color);
  
  /// Draw a line segment.
  virtual void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color);
  
  /// Draw a transform. Choose your own length scale.
  /// @param xf a transform.
  virtual void DrawTransform(const b2Transform& xf);
  
  virtual void DrawPoint(const b2Vec2& p, float32 size, const b2Color& color);
};

#endif /* defined(__Letgobirdy__TestBody__) */

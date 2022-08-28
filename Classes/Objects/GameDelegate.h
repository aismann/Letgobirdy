//
//  GameDelegate.h
//  LetGoBirdy
//
//  Created by Harry Nguyen on 28/8/22.
//

#ifndef GameDelegate_h
#define GameDelegate_h

class GameDelegate {
public:
  virtual void characterJumpSuccess() = 0;
  virtual void characterJumpFailed() = 0;
  virtual void blockTouchWhileContactWithWall() = 0;
};


#endif /* GameDelegate_h */

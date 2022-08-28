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
  virtual void characterDoubleJumpSuccess() = 0;
  virtual void characterDoubleJumpFailed() = 0;
  virtual void blockTouchWhileContactWithWall() = 0;
  virtual void gameOver() = 0;
};

#endif /* GameDelegate_h */

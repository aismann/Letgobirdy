//
//  GameDefineState.h
//  Letgobirdy
//
//  Created by Harry Nguyen on 1/16/17.
//
//

#ifndef GameDefineState_h
#define GameDefineState_h

#define NUMBER_BARRIER_ON_SCREEN 6
#define MAX_BARRIER_LEVEL 10
#define VELOCITY_CHARACTER 7.4
#define DELTA_VELOCITY_MONSTER 5
#define PTM_RATIO 32.0f
#define BUFFER_SIZE_HEIGHT 50.0f
#define WIDTH_RUN_BUFFER 60.0f
#define TIME_RUN_ACTION_SCREEN 0.6f
#define RATIO_SCALE_CHARACTER 0.104f
#define RATIO_SCALE_MONSTER 0.086f

#define VELOCITY_JUMP_X  6.0f
#define VELOCITY_SINGLE_JUMP_Y 16.2f
#define VELOCITY_DOUBLE_JUMP_Y 11

#define SCALE_X_BIRD 1.6
#define SCALE_Y_BIRD 2.08

typedef enum {
  RUNNING = 0,
  PAUSING = 1,
  JUMPING = 2,
  DIE = 3,
  IDLE = 4,
} CHARACTER_STATUS;

typedef enum {
  LEFT_RIGHT = 0,
  RIGHT_LEFT = 1,
}DIRECTION;

typedef enum {
  CONTACT_LANDING = 3,
  CONTACT_WALL = 4,
  CONTACT_MONSTER = 7,
}CONTACT;

enum{
  kCharacterCatBit = 1 << 0,
  kJumpingCharacterCatBit = 1<< 1,
  kGroundCatBit = 1 << 2,
  kPassedBarrierCatBit = 1 << 3,
  kNextBarrierCatBit = 1 << 4,
  kWallCatBit = 1 << 5,
};

enum{
  kCharacterMaskBit =  kWallCatBit | kGroundCatBit | kPassedBarrierCatBit,
  kJumpingCharacterMaskBit = kNextBarrierCatBit | kGroundCatBit | kWallCatBit,
  kGroundMaskBit = kCharacterCatBit | kJumpingCharacterCatBit,
  kPassedBarrierMaskBit = kCharacterCatBit,
  kNextBarrierMaskBit = kJumpingCharacterCatBit,
  kWallMasBit = kCharacterCatBit | kJumpingCharacterCatBit,
};

typedef enum {
  TAG_BTN_MENU_POPUP = 501,
  TAG_BTN_HOME_POPUP = 503,
  TAG_BTN_SOUND_ON_POPUP = 504,
  TAG_BTN_SOUND_OFF_POPUP = 505,
}TAG_POP_UP;


#define HEIGHT_DISTANCE 200.0f
#define HALF_PASS_HEIGHT_LANDING 58.0f

#endif /* GameDefineState_h */

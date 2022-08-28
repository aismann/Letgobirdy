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
#define RUN_VELOCITY_CHARACTER 7.4
#define VELOCITY_MONSTER 5
#define RADIUS_PHYSIC_CHARACTER 50.0f
#define X_MIN_POSITION_CHANGE_DIRECTION 60.0f
#define TIME_MOVE_UP_SCREEN 0.6f
#define RATIO_SCALE_CHARACTER_SKELETON 0.104f
#define RATIO_SCALE_MONSTER_SKELETON 0.086f

#define VELOCITY_JUMP_X  6.0f
#define VELOCITY_SINGLE_JUMP_Y 16.2f
#define VELOCITY_DOUBLE_JUMP_Y 11

#define HEIGHT_DISTANCE_BETWEEN_BARRIER 200.0f
#define HALF_PASS_HEIGHT_LANDING 58.0f


#define SCALE_X_BIRD 1.6
#define SCALE_Y_BIRD 2.08
#define PTM_RATIO 32.0f

#define TAG_LEVEL_LAYER 20
#define TAG_CHARACTER 21
#define COLOR_BG_POP_UP_START Color4B(50.0f/255.0f, 50.0f/255.0f, 50.0f/255.0f,100.0f)
#define COLOR_BG_POP_UP_END Color4B(115.0f/255.0f, 115.0f/255.0f, 115.0f/255.0f,200.0f)
#define TAG_START_LAYER 1234
#define TAG_BTN_SOUND_ON 145
#define TAG_BTN_SOUND_OFF 146
#define TAG_BTN_REPLAY  205
#define TAG_BTN_HOME  206
#define TAG_BTN_RATE  207

typedef enum {
  RUNNING = 0,
  PAUSING = 1,
  JUMPING = 2,
} CHARACTER_STATUS;

typedef enum {
  LEFT_RIGHT = 0,
  RIGHT_LEFT = 1,
}DIRECTION;

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
  TAG_BTN_HOME_POPUP = 501,
  TAG_BTN_SOUND_ON_POPUP = 502,
  TAG_BTN_SOUND_OFF_POPUP = 503,
}TAG_POP_UP;

#endif /* GameDefineState_h */

//
//  MainLayer.h
//  Letgobirdy
//
//  Created by Harry Nguyen on 12/13/16.
//
//
#include "cocos2d.h"
#include "MonsterLayer.h"
#include "GameContactListener.h"
#include "GameDefineState.h"
#include "Character.h"
#include "GameDelegate.h"
#include "ui/CocosGUI.h"
#include "GameManager.h"
USING_NS_CC;

class MainLayer : public cocos2d::Layer, GameDelegate {
private:
  Size visibleSize;
  Vec2 positionStartBarrier;
  MonsterLayer* monsterLayer;
  b2World* physicWorld;
  Character* character;
  CustomCommand debugCommand;
  
  cocos2d::ui::Button* buttonSoundOn;
  cocos2d::ui::Button* buttonSoundOff;
  cocos2d::ui::Button* buttonHome;
  Label* lbScore;
  
  GameManager* gameManager;
  float heightCenterScreen;
  bool isTouchAvailable;
  int gameScore;
  int barrierIndexWithNoMonster;
  int numberScreens;
private:
  void updateUIWhenCharacterDoubleJumpSuccess();
public:
  virtual bool init();
  CREATE_FUNC(MainLayer);
  void update(float dt);
  void onEnter();
  void onExit();
  
  void createGameLayer();
  
  /* Render Body i can see real contact for every thing*/
  virtual void draw(Renderer *renderer, const Mat4 &transform, uint32_t flags);
  virtual void drawPhysics(Renderer *renderer, const Mat4 &transform, uint32_t flags);
  
  bool onTouchOneByOneBegan(Touch* mTouch, Event* pEvent);
  
  void createUIAfterClickPlayButton();
  void createGameOverLayer();
  void handleClickPlayGameButton(Ref* pSender);
  void handleClickButtonOnGameOverPopUp(Ref* pSender);
  void handleClickHomeAndSoundButton(Ref* pSender);
  
  virtual void characterDoubleJumpSuccess();
  virtual void characterDoubleJumpFailed();
  virtual void blockTouchWhileContactWithWall();
  virtual void gameOver();
};


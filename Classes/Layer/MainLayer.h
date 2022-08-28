

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
  int barrierIndexWithNoMonster;
  int numberScreens;
  Size visibleSize;
  Vec2 origin;
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
  int gameScore;
  bool isTouchAvailable;
private:
  void updateUIWhenCharacterJumpSuccess();
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
  
  void createHomeAndSoundButtonOnTheRightScreen();
  void btnClickHomeAndSoundButton(Ref* pSender);
  void createGameOverLayer();
  void btnClickButtonOnGameOverPopUp(Ref* pSender);
  
  
  virtual void characterJumpSuccess();
  virtual void characterJumpFailed();
  virtual void blockTouchWhileContactWithWall();
};


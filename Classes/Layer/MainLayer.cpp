//
//  MainLayer.cpp
//  Letgobirdy
//
//  Created by Harry Nguyen on 12/13/16.
//
//
#include "MainLayer.h"
#include "SimpleAudioEngine.h"
#include "GameScene.h"
#include "GameDefineState.h"
#include "HomeScene.h"

static char bufferScore[512] = {0};

bool MainLayer::init() {
  if (!Layer::init()) {
    return false;
  }
  isTouchAvailable = true;
  gameScore = 0;
  numberScreens = 0;
  barrierIndexWithNoMonster = 0;
  
  visibleSize = Director::getInstance()->getWinSize();
  heightCenterScreen = visibleSize.height/2;
  
  physicWorld = new b2World(b2Vec2(0.0f, -80.0f));
  GameContactListener* listenerContact = new GameContactListener();
  listenerContact->setDelegate(this);
  physicWorld->SetContactListener(listenerContact);
  physicWorld->SetAllowSleeping(false);
  
  gameManager = new GameManager();
  
  monsterLayer = MonsterLayer::create();
  monsterLayer->setAnchorPoint(Vec2(0, 0));
  monsterLayer->setPosition(Vec2(0,0));
  monsterLayer->setGameManager(gameManager);
  monsterLayer->initPhysicForMap(physicWorld);
  
  character = new Character();
  character->setPosition(Vec2(visibleSize.width/2.0, monsterLayer->landingSprite->getContentSize().height/2.0));
  character->createCharacterBySpine();
  character->defineBodyCharacter(physicWorld);
  character->setDelegate(this);
  this->addChild(character,TAG_CHARACTER);
  gameManager->setCharacter(character);
  
  monsterLayer->createMonstersAndBarriersLevel(numberScreens, MAX_BARRIER_LEVEL, HALF_PASS_HEIGHT_LANDING, barrierIndexWithNoMonster);
  this->addChild(monsterLayer,TAG_LEVEL_LAYER);
  
  lbScore = Label::createWithTTF("Score 0", "HGEOSLAB.TTF", 40);
  lbScore->setVisible(false);
  lbScore->setPosition(Vec2(visibleSize.width/2, visibleSize.height*0.86f));
  this->addChild(lbScore,100);
  
  this->createGameLayer();
  
  this->scheduleUpdate();
  return true;
}

void MainLayer::createGameLayer() {
  LayerColor* colorBgStart = LayerColor::create(COLOR_BG_POP_UP_START, visibleSize.width, visibleSize.height);
  colorBgStart->setPosition(Vec2::ZERO);
  colorBgStart->setTag(TAG_START_LAYER);
  this->addChild(colorBgStart,500);
  
  auto spriteHelp = Sprite::create("help_dialog.png");
  spriteHelp->setPosition(Vec2(visibleSize.width/2, visibleSize.height*0.56f));
  colorBgStart->addChild(spriteHelp,1);
  
  auto btnPlay = ui::Button::create("button_click.png","","",ui::Widget::TextureResType::LOCAL);
  btnPlay->setPosition(Vec2(spriteHelp->getPositionX(),spriteHelp->getPositionY() - spriteHelp->getContentSize().height/2 - btnPlay->getContentSize().height));
  btnPlay->addClickEventListener(CC_CALLBACK_1(MainLayer::handleClickPlayGameButton,this));
  btnPlay->setSwallowTouches(true);
  btnPlay->setZoomScale(0.1f);
  colorBgStart->addChild(btnPlay,3);
  
  auto touchListerStart = EventListenerTouchOneByOne::create();
  touchListerStart->setSwallowTouches(true);
  touchListerStart->onTouchBegan = [=](Touch* pTouch, Event* mEvent){ return true; };
  Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListerStart, colorBgStart);
}

void MainLayer::update(float dt){
  if(physicWorld) { physicWorld->Step(dt, 100, 1); }
  if(monsterLayer) { monsterLayer->update(dt); }
  if(gameManager->getCountBarrier() < NUMBER_BARRIER_ON_SCREEN) {
    numberScreens ++;
    srand( (unsigned)time(NULL) );
    barrierIndexWithNoMonster = rand() % MAX_BARRIER_LEVEL - 1;
    monsterLayer->createMonstersAndBarriersLevel(numberScreens,
                                                 MAX_BARRIER_LEVEL,
                                                 HALF_PASS_HEIGHT_LANDING + MAX_BARRIER_LEVEL*HEIGHT_DISTANCE_BETWEEN_BARRIER * numberScreens,
                                                 barrierIndexWithNoMonster);
  }
}

void MainLayer::updateUIWhenCharacterDoubleJumpSuccess() {
  sprintf(bufferScore, "Score %d", gameScore);
  lbScore->setString(bufferScore);
  
  heightCenterScreen += HEIGHT_DISTANCE_BETWEEN_BARRIER;
  auto moveAction = MoveTo::create(TIME_MOVE_UP_SCREEN, Vec2(this->getPositionX(), this->getPositionY() - HEIGHT_DISTANCE_BETWEEN_BARRIER));
  auto visibleAction = CallFunc::create([=](){ gameManager->setVisibleForBarrierAndMonster(); });
  this->runAction(Sequence::create(moveAction, visibleAction, NULL));
  
  lbScore->runAction(MoveTo::create(TIME_MOVE_UP_SCREEN,
                                    Vec2(lbScore->getPositionX(), lbScore->getPositionY() + HEIGHT_DISTANCE_BETWEEN_BARRIER)));
  
  buttonHome->runAction(MoveTo::create(TIME_MOVE_UP_SCREEN,
                                       Vec2(buttonHome->getPositionX(), buttonHome->getPositionY() + HEIGHT_DISTANCE_BETWEEN_BARRIER)));
  
  
  buttonSoundOn->runAction(MoveTo::create(TIME_MOVE_UP_SCREEN,
                                          Vec2(buttonSoundOn->getPositionX(), buttonSoundOn->getPositionY() + HEIGHT_DISTANCE_BETWEEN_BARRIER)));
  
  buttonSoundOff->runAction(MoveTo::create(TIME_MOVE_UP_SCREEN,
                                           Vec2(buttonSoundOff->getPositionX(), buttonSoundOff->getPositionY() + HEIGHT_DISTANCE_BETWEEN_BARRIER)));
}

void MainLayer::handleClickHomeAndSoundButton(Ref* pSender){
  int tag = ((ui::Button*)pSender)->getTag();
  if(tag == TAG_BTN_HOME_POPUP) {
    Director::getInstance()->replaceScene(HomeScene::createHomeScene());
  }
  if(tag == TAG_BTN_SOUND_ON_POPUP){
    buttonSoundOff->setVisible(true);
    buttonSoundOn->setVisible(false);
    UserDefault::getInstance()->setBoolForKey("SOUND_TURN_ON", false);
    CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0.0f);
    CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(0.0f);
  }
  if(tag == TAG_BTN_SOUND_OFF_POPUP){
    buttonSoundOff->setVisible(false);
    buttonSoundOn->setVisible(true);
    UserDefault::getInstance()->setBoolForKey("SOUND_TURN_ON", true);
    CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(1.0f);
    CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(1.0f);
  }
}

void MainLayer::handleClickPlayGameButton(Ref* pSender) {
  lbScore->setVisible(true);
  if(this->getChildByTag(TAG_START_LAYER)) {
    this->removeChildByTag(TAG_START_LAYER);
  }
  auto touchLister = EventListenerTouchOneByOne::create();
  touchLister->setSwallowTouches(true);
  touchLister->onTouchBegan = CC_CALLBACK_2(MainLayer::onTouchOneByOneBegan, this);
  Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchLister, this);
  this->createUIAfterClickPlayButton();
}

void MainLayer::createUIAfterClickPlayButton() {
  buttonHome = cocos2d::ui::Button::create("btn_home_normal.png","","",ui::Widget::TextureResType::LOCAL);
  buttonHome->setPosition(Vec2(visibleSize.width*0.9, visibleSize.height*0.86));
  buttonHome->setZoomScale(0.1f);
  buttonHome->setScale(1.2);
  buttonHome->setSwallowTouches(true);
  buttonHome->setTag(TAG_BTN_HOME_POPUP );
  buttonHome->addClickEventListener(CC_CALLBACK_1(MainLayer::handleClickHomeAndSoundButton,this));
  buttonHome->setSwallowTouches(true);
  this->addChild(buttonHome,600);
  
  buttonSoundOn = cocos2d::ui::Button::create("btn_sound_on_normal.png","","",ui::Widget::TextureResType::LOCAL);
  buttonSoundOn->setPosition(Vec2(buttonHome->getPositionX(), buttonHome->getPositionY() - buttonSoundOn->getContentSize().height*1.4));
  buttonSoundOn->setSwallowTouches(true);
  buttonSoundOn->setScale(1.2);
  buttonSoundOn->setZoomScale(0.1f);
  buttonSoundOn->setTag(TAG_BTN_SOUND_ON_POPUP );
  buttonSoundOn->addClickEventListener(CC_CALLBACK_1(MainLayer::handleClickHomeAndSoundButton,this));
  buttonSoundOn->setSwallowTouches(true);
  this->addChild(buttonSoundOn,600);
  
  buttonSoundOff = cocos2d::ui::Button::create("btn_sound_off_normal.png","","",ui::Widget::TextureResType::LOCAL);
  buttonSoundOff->setPosition(buttonSoundOn->getPosition());
  buttonSoundOff->setSwallowTouches(true);
  buttonSoundOff->setScale(1.2);
  buttonSoundOff->setZoomScale(0.1f);
  buttonSoundOff->setTag(TAG_BTN_SOUND_OFF_POPUP );
  buttonSoundOff->addClickEventListener(CC_CALLBACK_1(MainLayer::handleClickHomeAndSoundButton,this));
  buttonSoundOff->setSwallowTouches(true);
  this->addChild(buttonSoundOff,600);
  
  if(UserDefault::getInstance()->getBoolForKey("SOUND_TURN_ON", true)) {
    buttonSoundOff->setVisible(false);
    buttonSoundOn->setVisible(true);
  } else {
    buttonSoundOff->setVisible(true);
    buttonSoundOn->setVisible(false);
  }
}

void MainLayer::onEnter() {
  Layer::onEnter();
  EventListenerKeyboard *keyboardEvent = EventListenerKeyboard::create();
  keyboardEvent->onKeyReleased = [](EventKeyboard::KeyCode keyCode, Event*){
    if(keyCode == EventKeyboard::KeyCode::KEY_BACK){
      Director::getInstance()->replaceScene(HomeScene::createHomeScene());
    }
  };
  Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(keyboardEvent, this);
}

void MainLayer::onExit() {
  Layer::onExit();
  Director::getInstance()->getEventDispatcher()->removeEventListenersForTarget(this);
}

void MainLayer::draw(Renderer *renderer, const Mat4 &transform, uint32_t flags){
  Layer::draw(renderer, transform, flags);
  debugCommand.init(INT_MAX);
  debugCommand.func = CC_CALLBACK_0(MainLayer::drawPhysics, this, renderer, transform, flags);
  renderer->addCommand(&debugCommand);
}

void MainLayer::drawPhysics(Renderer *renderer, const Mat4 &transform, uint32_t flags){
  Director::getInstance()->pushMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
  Director::getInstance()->loadMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW, transform);
  //  physicWorld->DrawDebugData();
  Director::getInstance()->popMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
}

void MainLayer::handleClickButtonOnGameOverPopUp(Ref* pSender) {
  if(UserDefault::getInstance()->getBoolForKey("SOUND_TURN_ON", true)) {
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("soundClick.mp3");
  }
  int tag = ((ui::Button*)pSender)->getTag();
  if(tag == TAG_BTN_REPLAY) {
    gameManager->restartGameManager();
    Director::getInstance()->replaceScene(GameScene::createGameScene());
  }
  if(tag == TAG_BTN_HOME) {
    gameManager->restartGameManager();
    Director::getInstance()->replaceScene(HomeScene::createHomeScene());
  }
  if(tag == TAG_BTN_RATE) {
    Application::getInstance()->openURL("https://itunes.apple.com/us/developer/nguyen-chi-hoang/id1208114740");
  }
}

void MainLayer::createGameOverLayer() {
  buttonSoundOff->setVisible(false);
  buttonSoundOn->setVisible(false);
  buttonHome->setVisible(false);
  int hightScore = UserDefault::getInstance()->getIntegerForKey("Hight-Score", 0);
  if(gameScore >= hightScore) {
    hightScore = gameScore;
    UserDefault::getInstance()->setIntegerForKey("Hight-Score", gameScore);
  }
  
  LayerColor* layerGameOver = LayerColor::create(COLOR_BG_POP_UP_END, visibleSize.width, visibleSize.height);
  layerGameOver->setPosition(Vec2(0, heightCenterScreen - visibleSize.height/2));
  this->addChild(layerGameOver,600);
  
  auto spLogo = Sprite::create("logoGame.png");
  spLogo->setPosition(Vec2(visibleSize.width/2, visibleSize.height*0.76));
  layerGameOver->addChild(spLogo,1);
  
  auto spEndGame = Sprite::create("end_dialog.png");
  spEndGame->setPosition(Vec2(visibleSize.width/2, visibleSize.height*0.4));
  layerGameOver->addChild(spEndGame,1);
  
  char bufferTotalScore[512] = {0};
  sprintf(bufferTotalScore, "%d", gameScore);
  Label* totalScore = Label::createWithTTF(bufferTotalScore, "HGEOSLAB.TTF", 32);
  totalScore->setPosition(Vec2(spEndGame->getPositionX(), spEndGame->getPositionY() + totalScore->getContentSize().height*1.6));
  totalScore->setTextColor(Color4B::WHITE);
  layerGameOver->addChild(totalScore,2);
  
  Label* lbTitleScore = Label::createWithTTF("Score:", "HGEOSLAB.TTF", 36);
  lbTitleScore->setPosition(Vec2(totalScore->getPositionX(), totalScore->getPositionY() + lbTitleScore->getContentSize().height*0.8));
  lbTitleScore->setTextColor(Color4B::YELLOW);
  layerGameOver->addChild(lbTitleScore,2);
  
  Label* lbTitleBestScore = Label::createWithTTF("Best score:", "HGEOSLAB.TTF", 36);
  lbTitleBestScore->setPosition(Vec2(spEndGame->getPositionX(), spEndGame->getPositionY() - totalScore->getContentSize().height/6));
  lbTitleBestScore->setTextColor(Color4B::YELLOW);
  layerGameOver->addChild(lbTitleBestScore,2);
  
  char bufferEndGame[512] = {0};
  sprintf(bufferEndGame, "%d", hightScore);
  Label* lbHightScoreEndGame = Label::createWithTTF(bufferEndGame, "HGEOSLAB.TTF", 32);
  lbHightScoreEndGame->setPosition(Vec2(totalScore->getPositionX(), lbTitleBestScore->getPositionY() - lbHightScoreEndGame->getContentSize().height*0.8));
  lbHightScoreEndGame->setTextColor(Color4B::WHITE);
  layerGameOver->addChild(lbHightScoreEndGame,2);
  
  /** Replay */
  auto btnReplay = ui::Button::create("button_replay_normal.png","","",ui::Widget::TextureResType::LOCAL);
  btnReplay->setPosition(Vec2(spEndGame->getPositionX() - 5.0f, spEndGame->getPositionY() - spEndGame->getContentSize().height*0.3));
  btnReplay->addClickEventListener(CC_CALLBACK_1(MainLayer::handleClickButtonOnGameOverPopUp,this));
  btnReplay->setSwallowTouches(true);
  btnReplay->setZoomScale(0.1f);
  btnReplay->setTag(TAG_BTN_REPLAY);
  layerGameOver->addChild(btnReplay,3);
  
  auto btnHome = ui::Button::create("button_home_normal.png","","",ui::Widget::TextureResType::LOCAL);
  btnHome->setPosition(Vec2(btnReplay->getPositionX() - btnHome->getContentSize().width*1.4,btnReplay->getPositionY() + btnHome->getContentSize().height*0.3));
  btnHome->addClickEventListener(CC_CALLBACK_1(MainLayer::handleClickButtonOnGameOverPopUp,this));
  btnHome->setSwallowTouches(true);
  btnHome->setZoomScale(0.1f);
  btnHome->setTag(TAG_BTN_HOME);
  layerGameOver->addChild(btnHome,3);
  
  auto btnRate = ui::Button::create("button_share_normal.png","","",ui::Widget::TextureResType::LOCAL);
  btnRate->setPosition(Vec2(btnReplay->getPositionX() + btnRate->getContentSize().width*1.4,btnReplay->getPositionY() + btnRate->getContentSize().height*0.3));
  btnRate->addClickEventListener(CC_CALLBACK_1(MainLayer::handleClickButtonOnGameOverPopUp,this));
  btnRate->setSwallowTouches(true);
  btnRate->setZoomScale(0.1f);
  btnRate->setTag(TAG_BTN_RATE);
  layerGameOver->addChild(btnRate,3);
  
  auto touchGameOver = EventListenerTouchOneByOne::create();
  touchGameOver->setSwallowTouches(true);
  touchGameOver->onTouchBegan = [=](Touch* mtouch, Event* pEvent){ return true; };
  Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchGameOver, layerGameOver);
}

void MainLayer::gameOver() {
  auto action1 = DelayTime::create(1.0f);
  auto action2 = CallFunc::create([=](){ this->createGameOverLayer(); });
  this->runAction(Sequence::create(action1,action2, NULL));
}

void MainLayer::characterDoubleJumpSuccess() {
  isTouchAvailable = true;
  gameScore ++;
  this->updateUIWhenCharacterDoubleJumpSuccess();
}

void MainLayer::characterDoubleJumpFailed() {
  isTouchAvailable = true;
}

void MainLayer::blockTouchWhileContactWithWall() {
  isTouchAvailable = false;
}

bool MainLayer::onTouchOneByOneBegan(Touch* mTouch, Event* pEvent) {
  if(character == NULL) { return false; }
  if(isTouchAvailable == false) { return false; }
  if(character->status == RUNNING) {
    character->singleJump();
    character->saveCurrentPositionYBeforeJump();
  } else if (character->status == JUMPING) {
    character->doubleJump();
    isTouchAvailable = false;
  }
  return true;
}

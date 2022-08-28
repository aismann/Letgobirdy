#include "MainLayer.h"
#include "SimpleAudioEngine.h"
#include "GameScene.h"
#include "GameDefineState.h"
#include "HomeScene.h"
#define TAG_CHARACTER 21
#define TAG_LEVEL_LAYER 20
#define COLOR_BG_POP_UP_START Color4B(50.0f/255.0f, 50.0f/255.0f, 50.0f/255.0f,100.0f)
#define COLOR_BG_POP_UP_END Color4B(115.0f/255.0f, 115.0f/255.0f, 115.0f/255.0f,200.0f)
#define TAG_START_LAYER 1234
#define TAG_BTN_SOUND_ON 145
#define TAG_BTN_SOUND_OFF 146
#define TAG_BTN_REPLAY  205
#define TAG_BTN_HOME  206
#define TAG_BTN_SHARE  207
#define TAG_BTN_CLOSE  208
#define TAG_BTN_PLAY  209

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
  origin = Director::getInstance()->getVisibleOrigin();
  heightCenterScreen = visibleSize.height/2;
  
  physicWorld = new b2World(b2Vec2(0.0f, -80.0f));
  GameContactListener* listenerContact = new GameContactListener();
  listenerContact->setDelegate(this);
  physicWorld->SetContactListener(listenerContact);
  physicWorld->SetAllowSleeping(false);
  
  /* LevelGameScene */
  gameManager = new GameManager();
  
  monsterLayer = MonsterLayer::create();
  monsterLayer->setAnchorPoint(Vec2(0, 0));
  monsterLayer->setPosition(Vec2(0,0));
  monsterLayer->setGameManager(gameManager);
  monsterLayer->initPhysicForMap(physicWorld);
  
  /// Create character
  character = new Character();
  character->setPosition(Vec2(visibleSize.width/2.0, monsterLayer->landingSprite->getContentSize().height/2.0));
  character->createCharacterBySpine();
  character->defineBodyCharacter(physicWorld);
  character->setDelegate(this);
  this->addChild(character,TAG_CHARACTER);
  gameManager->setCharacter(character);
  
  monsterLayer->createMonstersAndBarriersLevel(numberScreens, MAX_BARRIER_LEVEL, HALF_PASS_HEIGHT_LANDING, barrierIndexWithNoMonster);
  this->addChild(monsterLayer,TAG_LEVEL_LAYER);
  
  /* Label Score*/
  lbScore = Label::createWithTTF("Score 0", "HGEOSLAB.TTF", 40);
  lbScore->setVisible(false);
  lbScore->setPosition(Vec2(visibleSize.width/2, visibleSize.height*0.86f));
  this->addChild(lbScore,100);
  
  /*Start Layer*/
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
  btnPlay->addClickEventListener(CC_CALLBACK_1(MainLayer::btnClickButtonOnGameOverPopUp,this));
  btnPlay->setSwallowTouches(true);
  btnPlay->setZoomScale(0.1f);
  btnPlay->setTag(TAG_BTN_PLAY);
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
                                                 HALF_PASS_HEIGHT_LANDING + MAX_BARRIER_LEVEL*HEIGHT_DISTANCE * numberScreens,
                                                 barrierIndexWithNoMonster);
  }
}

void MainLayer::updateUIWhenCharacterJumpSuccess() {
  sprintf(bufferScore, "Score %d", gameScore);
  lbScore->setString(bufferScore);
  
  heightCenterScreen += HEIGHT_DISTANCE;
  auto moveAction = MoveTo::create(TIME_RUN_ACTION_SCREEN, Vec2(this->getPositionX(), this->getPositionY() - HEIGHT_DISTANCE));
  auto visibleAction = CallFunc::create([=](){ gameManager->setVisibleForBarrierAndMonster(); });
  this->runAction(Sequence::create(moveAction, visibleAction, NULL));
  
  lbScore->runAction(MoveTo::create(TIME_RUN_ACTION_SCREEN,
                                    Vec2(lbScore->getPositionX(), lbScore->getPositionY() + HEIGHT_DISTANCE)));
  
  buttonHome->runAction(MoveTo::create(TIME_RUN_ACTION_SCREEN,
                                       Vec2(buttonHome->getPositionX(), buttonHome->getPositionY() + HEIGHT_DISTANCE)));
  
  
  buttonSoundOn->runAction(MoveTo::create(TIME_RUN_ACTION_SCREEN,
                                          Vec2(buttonSoundOn->getPositionX(), buttonSoundOn->getPositionY() + HEIGHT_DISTANCE)));
  
  buttonSoundOff->runAction(MoveTo::create(TIME_RUN_ACTION_SCREEN,
                                           Vec2(buttonSoundOff->getPositionX(), buttonSoundOff->getPositionY() + HEIGHT_DISTANCE)));
}


void MainLayer::btnClickButtonOnGameOverPopUp(Ref* pSender) {
  int tag = ((ui::Button*)pSender)->getTag();
  CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("soundClick.mp3");
  if(tag == TAG_BTN_SOUND_ON) {
    CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0.0f);
    CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(0.0f);
    UserDefault::getInstance()->setBoolForKey("Sound-OnGame", false);
  }
  if(tag == TAG_BTN_SOUND_OFF) {
    CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(1.0f);
    CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(1.0f);
    UserDefault::getInstance()->setBoolForKey("Sound-OnGame", true);
  }
  if(tag == TAG_BTN_REPLAY) {
    gameManager->restartGameManager();
    Director::getInstance()->replaceScene(GameScene::createGameScene());
  }
  if(tag == TAG_BTN_HOME) {
    gameManager->restartGameManager();
    Director::getInstance()->replaceScene(HomeScene::createHomeScene());
  }
  
  if(tag == TAG_BTN_SHARE) {
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    Application::getInstance()->openURL("https://itunes.apple.com/us/developer/nguyen-chi-hoang/id1208114740");
#endif
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    Application::getInstance()->openURL("https://play.google.com/store/apps/developer?id=MacroGame&hl=en");
#endif
  }
  if(tag == TAG_BTN_CLOSE) {
    if(this->getChildByTag(999)) {
      this->removeChildByTag(999);
    }
  }
  if(tag == TAG_BTN_PLAY) {
    lbScore->setVisible(true);
    if(this->getChildByTag(TAG_START_LAYER)) {
      this->removeChildByTag(TAG_START_LAYER);
    }
    auto touchLister = EventListenerTouchOneByOne::create();
    touchLister->setSwallowTouches(true);
    touchLister->onTouchBegan = CC_CALLBACK_2(MainLayer::onTouchOneByOneBegan, this);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchLister, this);
    this->createHomeAndSoundButtonOnTheRightScreen();
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
  
  /*Icon game here*/
  auto spLogo = Sprite::create("logoGame.png");
  spLogo->setPosition(Vec2(visibleSize.width/2, visibleSize.height*0.76));
  layerGameOver->addChild(spLogo,1);
  
  /*Icon game here*/
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
  btnReplay->addClickEventListener(CC_CALLBACK_1(MainLayer::btnClickButtonOnGameOverPopUp,this));
  btnReplay->setSwallowTouches(true);
  btnReplay->setZoomScale(0.1f);
  btnReplay->setTag(TAG_BTN_REPLAY);
  layerGameOver->addChild(btnReplay,3);
  
  auto btnHome = ui::Button::create("button_home_normal.png","","",ui::Widget::TextureResType::LOCAL);
  btnHome->setPosition(Vec2(btnReplay->getPositionX() - btnHome->getContentSize().width*1.4,btnReplay->getPositionY() + btnHome->getContentSize().height*0.3));
  btnHome->addClickEventListener(CC_CALLBACK_1(MainLayer::btnClickButtonOnGameOverPopUp,this));
  btnHome->setSwallowTouches(true);
  btnHome->setZoomScale(0.1f);
  btnHome->setTag(TAG_BTN_HOME);
  layerGameOver->addChild(btnHome,3);
  
  auto btnShare = ui::Button::create("button_share_normal.png","","",ui::Widget::TextureResType::LOCAL);
  btnShare->setPosition(Vec2(btnReplay->getPositionX() + btnShare->getContentSize().width*1.4,btnReplay->getPositionY() + btnShare->getContentSize().height*0.3));
  btnShare->addClickEventListener(CC_CALLBACK_1(MainLayer::btnClickButtonOnGameOverPopUp,this));
  btnShare->setSwallowTouches(true);
  btnShare->setZoomScale(0.1f);
  btnShare->setTag(TAG_BTN_SHARE);
  layerGameOver->addChild(btnShare,3);
  
  auto touchGameOver = EventListenerTouchOneByOne::create();
  touchGameOver->setSwallowTouches(true);
  touchGameOver->onTouchBegan = [=](Touch* mtouch, Event* pEvent){ return true; };
  Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchGameOver, layerGameOver);
}

void MainLayer::btnClickHomeAndSoundButton(Ref* pSender){
  int tag = ((ui::Button*)pSender)->getTag();
  if(tag == TAG_BTN_HOME_POPUP) {
    Director::getInstance()->replaceScene(HomeScene::createHomeScene());
  }
  if(tag == TAG_BTN_SOUND_ON_POPUP){
    buttonSoundOff->setVisible(true);
    buttonSoundOn->setVisible(false);
    UserDefault::getInstance()->setBoolForKey("Sound-OnGame", false);
    CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0.0f);
    CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(0.0f);
  }
  if(tag == TAG_BTN_SOUND_OFF_POPUP){
    buttonSoundOff->setVisible(false);
    buttonSoundOn->setVisible(true);
    UserDefault::getInstance()->setBoolForKey("Sound-OnGame", true);
    CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(1.0f);
    CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(1.0f);
  }
}

void MainLayer::createHomeAndSoundButtonOnTheRightScreen() {
  buttonHome = cocos2d::ui::Button::create("btn_home_normal.png","","",ui::Widget::TextureResType::LOCAL);
  buttonHome->setPosition(Vec2(visibleSize.width*0.9, visibleSize.height*0.86));
  buttonHome->setZoomScale(0.1f);
  buttonHome->setScale(1.2);
  buttonHome->setSwallowTouches(true);
  buttonHome->setTag(TAG_BTN_HOME_POPUP );
  buttonHome->addClickEventListener(CC_CALLBACK_1(MainLayer::btnClickHomeAndSoundButton,this));
  buttonHome->setSwallowTouches(true);
  this->addChild(buttonHome,600);
  
  buttonSoundOn = cocos2d::ui::Button::create("btn_sound_on_normal.png","","",ui::Widget::TextureResType::LOCAL);
  buttonSoundOn->setPosition(Vec2(buttonHome->getPositionX(), buttonHome->getPositionY() - buttonSoundOn->getContentSize().height*1.4));
  buttonSoundOn->setSwallowTouches(true);
  buttonSoundOn->setScale(1.2);
  buttonSoundOn->setZoomScale(0.1f);
  buttonSoundOn->setTag(TAG_BTN_SOUND_ON_POPUP );
  buttonSoundOn->addClickEventListener(CC_CALLBACK_1(MainLayer::btnClickHomeAndSoundButton,this));
  buttonSoundOn->setSwallowTouches(true);
  this->addChild(buttonSoundOn,600);
  
  buttonSoundOff = cocos2d::ui::Button::create("btn_sound_off_normal.png","","",ui::Widget::TextureResType::LOCAL);
  buttonSoundOff->setPosition(buttonSoundOn->getPosition());
  buttonSoundOff->setSwallowTouches(true);
  buttonSoundOff->setScale(1.2);
  buttonSoundOff->setZoomScale(0.1f);
  buttonSoundOff->setTag(TAG_BTN_SOUND_OFF_POPUP );
  buttonSoundOff->addClickEventListener(CC_CALLBACK_1(MainLayer::btnClickHomeAndSoundButton,this));
  buttonSoundOff->setSwallowTouches(true);
  this->addChild(buttonSoundOff,600);
  
  if(UserDefault::getInstance()->getBoolForKey("Sound-OnGame", true)) {
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
  physicWorld->DrawDebugData();
  Director::getInstance()->popMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
}

void MainLayer::characterJumpSuccess() {
  isTouchAvailable = true;
  gameScore ++;
  this->updateUIWhenCharacterJumpSuccess();
}

void MainLayer::characterJumpFailed() {
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

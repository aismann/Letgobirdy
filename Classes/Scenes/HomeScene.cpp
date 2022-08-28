//
//  HomeScene.cpp
//  Letgobirdy
//
//  Created by Nguyen Chi Hoang on 4/5/17.
//
//

#include "HomeScene.h"
#include "ui/CocosGUI.h"
#include "GameScene.h"
#include "SimpleAudioEngine.h"
#define TAG_BTN_PLAY 101

Scene* HomeScene::createHomeScene() {
  auto scene = Scene::create();
  auto layer = HomeScene::create();
  scene->addChild(layer);
  return scene;
}

bool HomeScene::init() {
  if(!Layer::init()) {
    return false;
  }
  winSize = Director::getInstance()->getWinSize();
  auto bg = Sprite::create("bg_start.png");
  bg->setPosition(Vec2(winSize.width/2, winSize.height/2));
  this->addChild(bg,1);
  
  auto spLogo = Sprite::create("logoGame.png");
  spLogo->setPosition(Vec2(winSize.width/2, winSize.height*0.56));
  spLogo->setScale(0.8);
  this->addChild(spLogo,2);
  
  ui::Button* btnPlay = ui::Button::create("button_play_normal.png","","",ui::Widget::TextureResType::LOCAL);
  btnPlay->setZoomScale(0.1f);
  btnPlay->setPosition(Vec2(winSize.width/2, spLogo->getPositionY() - btnPlay->getContentSize().height*2.0));
  btnPlay->setTag(TAG_BTN_PLAY);
  
  btnPlay->addClickEventListener(CC_CALLBACK_1(HomeScene::eventClickPlay,this));
  btnPlay->setSwallowTouches(true);
  this->addChild(btnPlay,3);
  return true;
}

void HomeScene::eventClickPlay(Ref* pSender) {
  if(UserDefault::getInstance()->getBoolForKey("SOUND_TURN_ON", true)) {
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("soundClick.mp3");
  }
  int tag = ((ui::Button*)pSender)->getTag();
  if(tag == TAG_BTN_PLAY) {
    Director::getInstance()->replaceScene(GameScene::createGameScene());
  }
}

void HomeScene::onEnter() {
  Layer::onEnter();
  EventListenerKeyboard *keyboardEvent = EventListenerKeyboard::create();
  keyboardEvent->onKeyReleased = [](EventKeyboard::KeyCode keyCode, Event*){
    if(keyCode == EventKeyboard::KeyCode::KEY_BACK){
      Director::getInstance()->end();
    }
  };
  Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(keyboardEvent, this);
}

void HomeScene::onExit() {
  Layer::onExit();
  Director::getInstance()->getEventDispatcher()->removeEventListenersForTarget(this);
}

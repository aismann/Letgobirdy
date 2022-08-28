//
//  AndroidScene.cpp
//  SlideBlock
//
//  Created by Harry Nguyen on 12/10/16.
//
//

#include "AndroidScene.h"
#include "HomeScene.h"

Scene* AndroidScene::CreateScene() {
  auto newScene = Scene::create();
  auto layer = AndroidScene::create();
  newScene->addChild(layer);
  return newScene;
}

bool AndroidScene::init() {
  if(!Layer::init()){
    return false;
  }
  
  LayerColor* backgroundLayer = LayerColor::create(Color4B(255.0f,255.0f,255.0f,255.0f), Director::getInstance()->getWinSize().width, Director::getInstance()->getWinSize().height);
  this->addChild(backgroundLayer,100);
  
  auto backGroundSp = Sprite::create("logo.png");
  backGroundSp->setPosition(Vec2( Director::getInstance()->getWinSize().width/2,
                                 Director::getInstance()->getWinSize().height/2 ));
  backgroundLayer->addChild(backGroundSp);
  scheduleOnce(schedule_selector(AndroidScene::update), 3.0f);
  return true;
}

void AndroidScene::update(float dt) {
  Director::getInstance()->replaceScene(HomeScene::createHomeScene());
}

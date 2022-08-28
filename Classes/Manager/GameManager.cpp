//
//  GameManager.cpp
//  Letgobirdy
//
//  Created by Harry Nguyen on 1/16/17.
//
//

#include "GameManager.h"
#include "GameDefineState.h"

GameManager::GameManager() {
  isGameOver = false;
  if(!arrayBarrier.empty()) {
    arrayBarrier.clear();
  }
  if(!arrayMonster.empty()) {
    arrayMonster.clear();
  }
}
GameManager::~GameManager() {
  this->clearDataForGame();
}

void GameManager::addBarrierAndMonster(Barrier* barrier , Monster* monster) {
  if(barrier != NULL) {
    arrayBarrier.push_back(barrier);
  }
  if(monster != NULL) {
    arrayMonster.push_back(monster);
  }
}

void GameManager::setCharacter(Character* character) {
  this->character = character;
}

void GameManager::restartGameManager() {
  isGameOver = false;
  if(!arrayBarrier.empty()) {
    arrayBarrier.clear();
  }
  if(!arrayMonster.empty()) {
    arrayMonster.clear();
  }
}

void GameManager::update(float dt) {
  if(isGameOver == true) { return; }
  character->update(dt);
  
  /// Barrier update
  if(!arrayBarrier.empty()) {
    for(std::vector<Barrier*>::iterator it = arrayBarrier.begin(); it != arrayBarrier.end(); ++it) {
      if(!(*it)->getIsPhysicBody()) {
        if(character->getPositionY() > ((*it)->getPositionY())) {
          character->setNextPositionY((*it)->getPositionY());
          (*it)->initBodyWithPhysicManager(worldPhysicManger, (*it)->getPosition());
          (*it)->activeUpdatePhysic();
        }
      }
    }
  }
  
  /// Monsters update
  if(!arrayMonster.empty()) {
    for(std::vector<Monster*>::iterator it = arrayMonster.begin(); it != arrayMonster.end(); ++it) {
      (*it)->update(dt);
      if(character->checkDieCharacter((*it)->getPosition())) {
        isGameOver = true;
        character->dieCharacter();
      }
    }
  }
}

void GameManager::setVisibleForBarrierAndMonster() {
  for(std::vector<Barrier*>::iterator it = arrayBarrier.begin(); it != arrayBarrier.end(); ++it) {
    if((*it)->getPositionY() > character->getPositionY() + HEIGHT_DISTANCE_BETWEEN_BARRIER * 5) {
      (*it)->setVisible(false);
    } else if((*it)->getPositionY() < character->getPositionY() - HEIGHT_DISTANCE_BETWEEN_BARRIER *1.5) {
      arrayBarrier.erase(it);
    } else {
      (*it)->setVisible(true);
    }
  }
  for(std::vector<Monster*>::iterator it = arrayMonster.begin(); it != arrayMonster.end(); ++it) {
    if((*it)->getPositionY() > character->getPositionY() + HEIGHT_DISTANCE_BETWEEN_BARRIER * 5) {
      (*it)->setVisible(false);
    } else if((*it)->getPositionY() < character->getPositionY() - HEIGHT_DISTANCE_BETWEEN_BARRIER*1.5 ) {
      arrayMonster.erase(it);
    } else {
      (*it)->setVisible(true);
    }
  }
}

int GameManager::getCountBarrier() {
  return (int)arrayBarrier.size();
}

void GameManager::clearDataForGame() {
  if(!arrayBarrier.empty()) {
    arrayBarrier.clear();
  }
  if(!arrayMonster.empty()) {
    arrayMonster.clear();
  }
}

void GameManager::setPhysic(b2World* world) {
  worldPhysicManger = world;
}


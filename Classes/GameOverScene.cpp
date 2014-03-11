#include "GameOverScene.h"
#include "NeuronMenuScene.h"

USING_NS_CC;


Scene* GameOver::createScene(int time_finished) {
  auto scene = Scene::create();
  auto layer = GameOver::create(time_finished);
  scene->addChild(layer);
  return scene;
}


GameOver* GameOver::create(int time_finished) {
  GameOver* pRet = new GameOver();
  pRet->setTimeFinished(time_finished);
  if(pRet && pRet->init()) {
    pRet->autorelease();
    return pRet;
  }

  delete pRet;
  pRet = NULL;
  return NULL;
}

bool GameOver::init() {
  if(!Layer::init()){
    return false;
  }
  auto visibleSize = Director::getInstance()->getVisibleSize();
  auto origin = Director::getInstance()->getVisibleOrigin();
  LabelTTF* thanks = LabelTTF::create("Thanks for playing +Neuron", "Serif", 50);
  thanks->setPosition(Point(visibleSize.width/2, visibleSize.height - thanks->getContentSize().height*2));
  this->addChild(thanks);

  auto _nickname_input = new TextFieldTTFDefault();
  _nickname_input->autorelease();
  _nickname_input->onEditingEnd = CC_CALLBACK_1(GameOver::getNickName, this);
  this->addChild(_nickname_input);

  /*
   *Todo accion por fuera de INIT genera un fallo de segmentacion
   *??como hacer para aparecer el boton despues de escribir el nombre?
   * -intente con updateGame, fallo de segmentacion
   * -intente desde getNickname, fallo de segmentacion
   */
  /*
  auto endGameItem = MenuItemImage::create(
				 "botones/salir.png",
				 "botones/salir.png",
				 CC_CALLBACK_0(GameOver::closeAndSave, this)
				 );
  endGameItem->setPosition(Point(visibleSize.width - endGameItem->getContentSize().width + origin.x, visibleSize.height/5 + origin.y));
  endGameItem->runAction(Hide::create());
  auto menu = Menu::create(endGameItem, NULL);
  menu->setPosition(Point::ZERO);
  _nickname_input->addChild(menu);*/

  return true;
}


void GameOver::getNickName(const std::string nickname) {
  _nickname = nickname;
  this->closeAndSave();
}

void GameOver::closeAndSave() {
  //_nickname_input->getString();
  CCLog("CERRANDO Y GUARDANDO %s", _nickname.c_str());
  int score_pos = UserDefault::sharedUserDefault()->getIntegerForKey("score_pos", 0);
  char score_name[80];
  char score_name_key[80];
  char score_pos_key[80];

  UserDefault::sharedUserDefault()->setIntegerForKey(score_name, _time_finished);
  for(int iScore = 0; iScore < MAX_ITEMS_SCORE; iScore++) {
    sprintf(score_pos_key,"score_pos_%d", iScore);
    int score_time = UserDefault::sharedUserDefault()->getIntegerForKey(score_pos_key, 0);
    
    if(_time_finished > score_time) {
      score_pos = iScore;
    }
  }
  sprintf(score_name_key, "score_name_%d", score_pos);
  sprintf(score_pos_key,"score_pos_%d", score_pos);
  UserDefault::sharedUserDefault()->setIntegerForKey(score_pos_key, _time_finished);
  UserDefault::sharedUserDefault()->setStringForKey(score_name_key, _nickname);
  UserDefault::sharedUserDefault()->flush();
  //VOLVIENDO AL MENU
  stopAllActions();
  unscheduleAllSelectors();
  removeFromParentAndCleanup(true);
  Scene* newScene = TransitionFade::create(0.7, NeuronMenu::createScene()); 
  Director::sharedDirector()->replaceScene(newScene);
}










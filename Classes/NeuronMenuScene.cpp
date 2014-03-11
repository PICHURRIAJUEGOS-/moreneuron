#include "NeuronMenuScene.h"
#include "MoreNeuronScene.h"
#include "SimpleAudioEngine.h"
#include "HelpScene.h"
//#include "PluginManager.h"
#include "Config.h"

USING_NS_CC;
//using namespace cocos2d::plugin;

Scene* NeuronMenu::createScene() {
  auto scene = Scene::create();
  auto layer = NeuronMenu::create();
  scene->addChild(layer);
  return scene;
}


bool NeuronMenu::init() {
  if(!Layer::init()){
    return false;
  }
  if(!CocosDenshion::SimpleAudioEngine::sharedEngine()->isBackgroundMusicPlaying())
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("menu/menu.ogg", true);
  Size visibleSize = Director::getInstance()->getVisibleSize();
  Point origin = Director::getInstance()->getVisibleOrigin();

  Sprite* bg = Sprite::create("menu/menu.png");
  bg->setPosition(Point(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
  bg->setScaleX(visibleSize.width/bg->getContentSize().width);
  this->addChild(bg);

  auto startGameItem = MenuItemImage::create(
					     "botones/boton_on.png",
					     "botones/boton_off.png",
					     CC_CALLBACK_1(NeuronMenu::startGame, this)
					     );
  startGameItem->setPosition(Point(visibleSize.width - startGameItem->getContentSize().width, visibleSize.height/2 - startGameItem->getContentSize().height/2));

  
 
  auto creditGameItem = MenuItemImage::create(
					      "botones/boton_credito_on.png",
					      "botones/boton_credito_off.png",
					      CC_CALLBACK_1(NeuronMenu::credits, this)
					      );
  creditGameItem->setPosition(Point(visibleSize.width/2 + origin.x,origin.y + creditGameItem->getContentSize().height - 10 ));

 auto endGameItem = MenuItemImage::create(
				 "botones/salir.png",
				 "botones/salir_off.png",
				 CC_CALLBACK_1(NeuronMenu::endGame, this)
				 );
 endGameItem->setPosition(Point(visibleSize.width - endGameItem->getContentSize().width + origin.x, creditGameItem->getContentSize().height + origin.y - 10));

 auto helpItem = MenuItemImage::create(
				       "ayuda/tutorial.png",
				       "ayuda/tutorial2.png",
				       CC_CALLBACK_1(NeuronMenu::help, this)
				       );
 helpItem->setPosition(Point(visibleSize.width - startGameItem->getContentSize().width, visibleSize.height/2 - startGameItem->getContentSize().height/2 - helpItem->getContentSize().height));

 _menu = Menu::create(startGameItem, endGameItem, creditGameItem, helpItem, NULL);

 _menu->setPosition(Point::ZERO);
 this->addChild(_menu, 1);

 ADMOB_NEURON_SHOW_TOP;
 //ADMOB_NEURON_LOAD;
   
 return true;
}


void NeuronMenu::startGame(Object* pSender) {
  CCLog("STARTING GAME");
  if(!BigScreen){
    ADMOB_NEURON_HIDE;
  }

  stopAllActions();
  unscheduleAllSelectors();
  removeFromParentAndCleanup(true);
  Scene* newScene = TransitionFade::create(0.7, MoreNeuron::createScene()); 
  Director::sharedDirector()->replaceScene(newScene);
}

void NeuronMenu::endGame(Object* pSender) {
  CCLog("ENDING GAME");
  ADMOB_NEURON_UNLOAD;

  stopAllActions();
  unscheduleAllSelectors();
  removeFromParentAndCleanup(true);
  Director::getInstance()->end();
}

void NeuronMenu::credits(Object* pSender) {
  Size visibleSize = Director::getInstance()->getVisibleSize();
  Point origin = Director::getInstance()->getVisibleOrigin();

  Layer* credits = Layer::create();
  if(Layer::init()) {
    _menu->runAction(Hide::create());
    Sprite* bg = Sprite::create("creditos/creditos.png");
    bg->setPosition(Point(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
    bg->runAction(Sequence::create(FadeIn::create(1.0f), DelayTime::create(3.0f), FadeOut::create(1.0f), RemoveSelf::create(),NULL));
    credits->addChild(bg, 99);
    _menu->runAction(Sequence::create(DelayTime::create(5.0f), Show::create(), FadeIn::create(1.0f), NULL));
    credits->runAction(Sequence::create(DelayTime::create(5.0f), RemoveSelf::create(), NULL));
    this->addChild(credits, 99);
  }
}

void NeuronMenu::help(Object *pSender) {
  stopAllActions();
  unscheduleAllSelectors();
  removeFromParentAndCleanup(true);
  Scene* newScene = TransitionFade::create(0.7, Help::createScene()); 
  Director::sharedDirector()->replaceScene(newScene);
}

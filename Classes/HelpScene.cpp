#include "HelpScene.h"
#include "NeuronMenuScene.h"

USING_NS_CC;
Scene* Help::createScene() {
  auto scene = Scene::create();
  auto layer = Help::create();
  scene->addChild(layer);
  return scene;
}


bool Help::init() {
  if(!Layer::init()) {
    return false;
  }
  Size visibleSize = Director::getInstance()->getVisibleSize();
  Point origin = Director::getInstance()->getVisibleOrigin();

  auto help_bg = Sprite::create("ayuda/ejemplo.png");
  auto help_fg = Sprite::create("ayuda/ejemplo1.png");

  this->addChild(help_bg);
  help_bg->setPosition(Point(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
  this->addChild(help_fg);
  help_fg->setPosition(Point(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));  
  help_fg->runAction(RepeatForever::create(Sequence::create(FadeIn::create(0.3f), DelayTime::create(2.0f), FadeOut::create(0.3f), DelayTime::create(1.0f), NULL)));

  auto dispatcher = Director::getInstance()->getEventDispatcher();

  auto _listener = EventListenerTouchOneByOne::create();
  _listener->onTouchBegan = CC_CALLBACK_2(Help::onTouchBegan, this);
  _listener->onTouchMoved = CC_CALLBACK_2(Help::onTouchMoved, this);
  _listener->onTouchEnded = CC_CALLBACK_2(Help::onTouchEnded, this);

  dispatcher->addEventListenerWithSceneGraphPriority(_listener, this);

  return true;
}


bool Help::onTouchBegan(Touch* touch, Event* ev) {
   stopAllActions();
   unscheduleAllSelectors();
   removeFromParentAndCleanup(true);
   Scene* newScene = TransitionFade::create(0.7, NeuronMenu::createScene()); 
   Director::sharedDirector()->replaceScene(newScene);
  return false;
}

/**
 *Presentacion.
 *
 *@todo hay veces que el cambio de escenera genera fallo de segmentacion en gnu/linux.
 */
#include "PresentationScene.h"
#include "NeuronMenuScene.h"
#include "Config.h"
//#include "ProtocolAds.h"
//#include "PluginManager.h"

USING_NS_CC;
//using namespace cocos2d::plugin;

Scene* Presentation::createScene() {
  auto scene = Scene::create();
  auto layer = Presentation::create();
  scene->addChild(layer);
  return scene;
}

bool Presentation::init() {
  if(!LayerColor::initWithColor(ccc4(0,0,0,255))){
    return false;
  }
  toNextSceneP = false;
  Size visibleSize = Director::getInstance()->getVisibleSize();
  Point origin = Director::getInstance()->getVisibleOrigin();
  auto estrellaSprite = Sprite::create("logo/estrella.png");
  auto yPos = estrellaSprite->getContentSize().height;
  estrellaSprite->setPosition(Point(0 - estrellaSprite->getContentSize().width, (visibleSize.height  - estrellaSprite->getContentSize().height) + origin.y));
  auto actionMove = MoveTo::create((float)1,
				   Point(visibleSize.width/2 + origin.x, (visibleSize.height - estrellaSprite->getContentSize().height) + origin.y));
  auto nextSceneAction = CallFuncN::create(this,
					  callfuncN_selector(Presentation::changeScene));

  auto delayAction =  DelayTime::create(3.0);

  
  estrellaSprite->runAction(Sequence::create(actionMove,  delayAction, nextSceneAction, NULL));
  this->addChild(estrellaSprite);

  auto juego = Sprite::create("logo/juegos.png");
  yPos += juego->getContentSize().height + 30;
  juego->setPosition(Point(0 - juego->getContentSize().width, (visibleSize.height - yPos + origin.x)));
  actionMove = MoveTo::create((float)0.7,
			      Point(visibleSize.width/2 + origin.x, (visibleSize.height - yPos + origin.y)));
  juego->runAction(Sequence::create(actionMove, NULL));
  this->addChild(juego);

  auto pichurria = Sprite::create("logo/pichurria.png");
  yPos += pichurria->getContentSize().height + 30;
  pichurria->setPosition(Point(visibleSize.width + pichurria->getContentSize().width, (visibleSize.height - yPos + origin.y)));
  actionMove = MoveTo::create((float)0.5,
			      Point(visibleSize.width/2 + origin.x, (visibleSize.height - yPos + origin.y)));
  pichurria->runAction(Sequence::create(actionMove, NULL));
  this->addChild(pichurria);
  this->schedule(schedule_selector(Presentation::update), 1.0);
  //Ads
  ADMOB_NEURON_LOAD

  return true;
}

void Presentation::update(float dt) {
  if(toNextSceneP) {
    stopAllActions();
    unscheduleAllSelectors();
    removeFromParentAndCleanup(true);
    Scene* newScene = TransitionFade::create(0.7, NeuronMenu::createScene()); 
    Director::sharedDirector()->replaceScene(newScene);
  }
}
void Presentation::changeScene(Node* sender) {
  toNextSceneP = true;
}

void Presentation::onTransitionDidFinished() {
  CCLog("TRANSITION DID FINISHED");
}








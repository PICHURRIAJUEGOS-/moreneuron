#ifndef NEURONMENUSCENE_H
#define NEURONMENUSCENE_H

#include "cocos2d.h"
//#include "ProtocolAds.h"

class NeuronMenu : public cocos2d::Layer
{
 public:

  static cocos2d::Scene* createScene();
  virtual bool init();
  CREATE_FUNC(NeuronMenu);

  void startGame(Object*);			
  void endGame(Object*);
  void credits(Object*);
  void help(Object*);
  cocos2d::Sprite* _bg_credits;
  cocos2d::Menu* _menu;

};

#endif

#ifndef HELPSCENE_H
#define HELPSCENE_H

#include "cocos2d.h"

class Help : public cocos2d::Layer
{
 public:
  static cocos2d::Scene* createScene();
  virtual bool init();
  CREATE_FUNC(Help);
  virtual bool onTouchBegan(cocos2d::Touch*, cocos2d::Event*);
};

#endif

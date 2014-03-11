#ifndef CRUX_H
#define CRUX_H

#include "cocos2d.h"

class Crux : public cocos2d::Node
{
 public:

  virtual bool init();
  void updateConnections(const Crux*);
 
  void setPerlaRight(int);
  void setPerlaLeft(int);
  void setPerlaBottom(int);
  void setPerlaTop(int);
  int getPerlaRight() { return perla_right; }
  int getPerlaLeft() { return perla_left; }
  int getPerlaTop() { return perla_top; }
  int getPerlaBottom() { return perla_bottom; }
  
  CREATE_FUNC(Crux);
 private:
  int center_x;
  int center_y;
  int perla_right;
  int perla_left;
  int perla_top;
  int perla_bottom;

  cocos2d::Sprite* _crux;
  cocos2d::Sprite* _perla_top;
  cocos2d::Sprite* _perla_left;
  cocos2d::Sprite* _perla_bottom;
  cocos2d::Sprite* _perla_right;

  cocos2d::Sprite* _sprite;
  cocos2d::Point position;

  cocos2d::Sprite* createSpriteRandomPerla();
  cocos2d::Sprite* createSpritePerlaKey(int);
  cocos2d::Texture2D* createTexturePerlaKey(int k);
  int getRandomPerla();
};

#endif

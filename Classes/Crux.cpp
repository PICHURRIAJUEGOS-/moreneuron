#include "Crux.h"
#include <ctime>
#include <cstdlib>

USING_NS_CC;

String pathPerlaKey(int k) {
  char skey[80];
  sprintf(skey, "perla%d", k);
  return String(skey);
}



Sprite* Crux::createSpritePerlaKey(int k) {
  char skey[80];
  sprintf(skey, "perla%d", k);
  CCLog("\tCreatePerla %d", k);
  return Sprite::createWithSpriteFrameName(skey);
}

int Crux::getRandomPerla() {
  return (rand()%8);
}
Sprite* Crux::createSpriteRandomPerla() {

  return createSpritePerlaKey(getRandomPerla());
}


Texture2D* Crux::createTexturePerlaKey(int k) {
  Texture2D* tex = new Texture2D();
  Image* img = new Image();
  //@todo como contralar si no carga imagen??
  Sprite* tmps = createSpritePerlaKey(k);
  return tmps->getTexture();
}

bool Crux::init(){

  if(!Node::init()){
    return false;
  }

  setContentSize(Size(192,192));

  _sprite = NULL;

  perla_right = 0;
  perla_left = 0;
  perla_top = 0;
  perla_bottom = 0;
  _crux = Sprite::create("objetos/tubo.png");
  center_x = getContentSize().width/2;
  center_y = getContentSize().height/2;
  _crux->setPosition(Point(center_x, center_y));
  this->addChild(_crux);
  
  this->setPerlaRight(getRandomPerla());
  this->setPerlaLeft(getRandomPerla());
  this->setPerlaTop(getRandomPerla());
  this->setPerlaBottom(getRandomPerla());

  return true;
}

/**
 *Cambia coneccion o bien el color de las perlas
 *para que conecte con la crux
 @deprecated
 */
__attribute__((deprecated)) void Crux::updateConnections(const Crux* crux) {

  //si la otra crux esta arriba
  if(crux->getPosition().y > getPosition().y && (crux->getPosition().y - getPosition().y) == 1) {
    perla_top = crux->perla_bottom;
    CC_SAFE_RELEASE(_perla_top);
    _perla_top = createSpritePerlaKey(perla_top);
  }
  
}


void Crux::setPerlaRight(int pk) {
  if(_perla_right) {
    _perla_right->removeFromParent();
    CC_SAFE_RELEASE(_perla_right);
  }
  perla_right = pk;
  _perla_right = createSpritePerlaKey(pk);
  _perla_right->setPosition(Point(center_x + _perla_right->getContentSize().width,center_y));
  this->addChild(_perla_right);
}

void Crux::setPerlaLeft(int pk) {
  if(_perla_left) {
    _perla_left->removeFromParent();
    CC_SAFE_RELEASE(_perla_left);
  }
  perla_left = pk;
  _perla_left = createSpritePerlaKey(pk);
  _perla_left->setPosition(Point(center_x - _perla_left->getContentSize().width,center_y));
  this->addChild(_perla_left);
}

void Crux::setPerlaTop(int pk) {
  if(_perla_top) {
    _perla_top->removeFromParent();
    CC_SAFE_RELEASE(_perla_top);
  }

  //CC_SAFE_RELEASE(_perla_top);
  perla_top = pk;
  _perla_top = createSpritePerlaKey(pk);
  _perla_top->setPosition(Point(center_x,center_y + _perla_top->getContentSize().height));
  this->addChild(_perla_top);
}

void Crux::setPerlaBottom(int pk) {
  if(_perla_bottom) {
    _perla_bottom->removeFromParent();
    CC_SAFE_RELEASE(_perla_bottom);
  }

  perla_bottom = pk;
  _perla_bottom = createSpritePerlaKey(pk);
  _perla_bottom->setPosition(Point(center_x, center_y - _perla_bottom->getContentSize().height));
  this->addChild(_perla_bottom);
}

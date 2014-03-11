#ifndef PRESENTATIONSCENE_H
#define PRESENTATIONSCENE_H

#include "cocos2d.h"

/**
 *Presentacion de pichurriajuegos
 *Basicamente es letro izquierda y derecha hasta el centro
 *luego aparece la estrella, dura unos 5 segundos y cambia de escena
 */
class Presentation : public cocos2d::LayerColor
{
 private:
  bool toNextSceneP;
 public:
  static cocos2d::Scene* createScene();
  virtual bool init();
  CREATE_FUNC(Presentation);

  void onTransitionDidFinished();
  void update(float);
  void changeScene(cocos2d::Node*);
};

#endif

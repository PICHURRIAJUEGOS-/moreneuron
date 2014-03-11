#ifndef GAMEOVERSCENE_H
#define GAMEOVERSCENE_H

#include "cocos2d.h"
#include "TextInput.h"
#define MAX_ITEMS_SCORE 3

/**
 *Escena que muestra el tiempo.
 *Y pide nombre para guardar registro
 */
class GameOver : public cocos2d::Layer
{
 public:
  static cocos2d::Scene* createScene(int);
  static GameOver* create(int);
  virtual bool init();
  void closeAndSave();
  CC_SYNTHESIZE(int, _time_finished, TimeFinished);
  void getNickName(const std::string);
 private:
  std::string _nickname;
  KeyboardNotificationLayer * _nickname_input;
};

#endif

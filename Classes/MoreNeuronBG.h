#ifndef MORENEURONBG_H
#define MORENEURONBG_H
#include "cocos2d.h"
#include <string>

class MoreNeuronBG : public cocos2d::Object
{
 public:
  static MoreNeuronBG* create(int);
  std::string bg_path;
  std::string slot_holder_path;
  std::string slot_game_path;
  std::string middle_path;
  std::string logo_path;
};

#endif

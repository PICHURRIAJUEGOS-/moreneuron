#include "MoreNeuronBG.h"
#include <cstdio>


MoreNeuronBG* MoreNeuronBG::create(int pos)
{
  MoreNeuronBG *theme = new MoreNeuronBG();
  if(theme){
    char path[200];
    sprintf(path, "mapa/base%d/", pos);
    std::string spath = path;
    theme->bg_path = spath + "base1.png";
    theme->slot_holder_path = spath + "arriba5.png";
    theme->slot_game_path = spath + "abajo4.png";
    theme->middle_path = spath + "barra_horizontal2.png";
    theme->logo_path = spath + "barra_lateral3.png";

    theme->autorelease();
    return theme;
  }
  CC_SAFE_DELETE(theme);
  return NULL;


}

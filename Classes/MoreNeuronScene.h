#ifndef MORENEURONSCENE_H
#define MORENEURONSCENE_H

#include "cocos2d.h"
#include "Crux.h"
#include "CruxBoard.h"
#include "MoreNeuronBG.h"
#include <vector>
//#include "ProtocolAds.h"
#define SIZE_OF_CRUX 3 //en tamano de celda osea 3x3
#define DESIGN_WIDTH 768.0f
#define DESIGN_HEIGHT 1280.0f
class MoreNeuron : public cocos2d::Layer//, public cocos2d::plugin::AdsListener
{
 public:
  static cocos2d::Scene* createScene();
  virtual bool init();
  void menuCloseCallback(Object* pSender);
  CREATE_FUNC(MoreNeuron);
  virtual void onExit();
  virtual bool onTouchBegan(cocos2d::Touch* , cocos2d::Event*);
  virtual void onTouchMoved(cocos2d::Touch* , cocos2d::Event*);

  /**
   *Realiza la logica principal del juego.
   *Mueve la *Crux* ha su posicion final y verifica
   *si completo el tablero o no.
   *y notifica estado para terminar juego, etc..
   */
  virtual void onTouchEnded(cocos2d::Touch* , cocos2d::Event* );
  void onGameComplete();
  void updateClock(float dt);
  virtual bool isTouchEnabled();
  void closeGame(cocos2d::Object*);
  void showMenu();
  //Ads
  //virtual void onAdsResult(cocos2d::plugin::AdsResultCode code, const char* msg);
  //virtual void onPlayerGetPoints(cocos2d::plugin::ProtocolAds* pAdsPlugin, int points);

 private:
  cocos2d::Size visibleSize;
  cocos2d::Point origin;
  int cellWidth;
  int cellHeight;

  CruxBoard board_crux;
  CruxBoard board_game_crux;
  Crux* _cruxs[SIZE_OF_CRUX][SIZE_OF_CRUX]; //desde donde se toman
  Crux* _cruxs_board[SIZE_OF_CRUX][SIZE_OF_CRUX]; //las crux de ya puestas
  Crux* selected_crux; //actualmente escogida
  cocos2d::Point selected_crux_begin_grid; //posicion de donde se saco
  CruxBoard* selected_crux_board; //tablero de donde se saco
  void initializePerlas();
  void initializeCruxs();
  cocos2d::Sprite* createSpriteRandomBackground();
  bool translateFromCruxBoardGame(cocos2d::Point pos, cocos2d::Point pos_crux);
  bool translateFromCruxBoard(cocos2d::Point pos, cocos2d::Point pos_game);

  cocos2d::String _music_background;
  cocos2d::String _effect_grab;
  cocos2d::String _effect_drop;
  std::vector<cocos2d::String*> _music_gallery;
  std::vector<cocos2d::String*> _background_gallery;
  int _time_elapsed;
  bool _finished; //si termino el juego
  cocos2d::LabelTTF* _timer;
  cocos2d::EventListenerTouchOneByOne* _listener;
  
  MoreNeuronBG *theme;
  MoreNeuronBG* createRandomTheme();
  //Ads
  //cocos2d::plugin::ProtocolAds* _admob;
  //cocos2d::plugin::ProtocolAds::AdsPos _posAdmob;
  //cocos2d::plugin::TAdsInfo adInfo;
};


#endif

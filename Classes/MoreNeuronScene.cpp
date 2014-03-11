#include "MoreNeuronScene.h"
#include <cstdio>
#include <string>
#include <cassert>
#include <algorithm>
#include <random>
#include <iterator>
#include <ctime>
#include <cstdlib>
#include "Config.h"
#include "Crux.h"
#include "SimpleAudioEngine.h"
#include "GameOverScene.h"
#include "NeuronMenuScene.h"
//#include "ProtocolAds.h"
//#include "PluginManager.h"

#define SCREEN_COLS 12
#define SCREEN_ROWS 20

#define MAX_CRUX 6
#define RANDOM_INT(lenght) random()%lenght
#define RANDOM_INIT() srandom(time(NULL))
USING_NS_CC;
//using namespace cocos2d::plugin;



Scene* MoreNeuron::createScene() {
  auto scene = Scene::create();
  auto layer = MoreNeuron::create();
  scene->addChild(layer);
  return scene;
}

bool MoreNeuron::init() {
  if(!Layer::init()){
    return false;
  }

  //@todo aun no se leer desde carpeta de recursos
  //la otra alternativa podria ser un .plist
  RANDOM_INIT();
  _music_gallery.push_back(String::create("musica/mapa1.ogg"));
  _music_gallery.push_back(String::create("musica/mapa2.ogg"));
  _background_gallery.push_back(String::create("mapa/mapa1.png"));
  /* _background_gallery.push_back(String::create("mapa/mapa2.png"));
  _background_gallery.push_back(String::create("mapa/mapa3.png"));
  _background_gallery.push_back(String::create("mapa/mapa4.png"));
  _background_gallery.push_back(String::create("mapa/mapa5.png"));
  _background_gallery.push_back(String::create("mapa/mapa6.png"));*/
  _music_background = _music_gallery[RANDOM_INT(2)]->getCString();
  _effect_grab = String("objetos/agarre.ogg");
  _effect_drop = String("objetos/suelta.ogg");
  visibleSize = Director::getInstance()->getVisibleSize();
  origin = Director::getInstance()->getVisibleOrigin();

  cellWidth = visibleSize.width/SCREEN_COLS;
  cellHeight = visibleSize.height/SCREEN_ROWS;
  CCLog("VisibleSize: %f %f", visibleSize.width, visibleSize.height);
  CCLog("cellWithInit:%d", cellWidth);
  _time_elapsed = 0;
  board_crux.setCellWidth(cellWidth);
  board_crux.setCellHeight(cellHeight);
  board_crux.setGrid(SIZE_OF_CRUX);
  board_game_crux.setCellWidth(cellWidth);
  board_game_crux.setCellHeight(cellHeight);
  board_game_crux.setGrid(SIZE_OF_CRUX);
  selected_crux = NULL;
  _finished = false;
  //font
  _timer = LabelTTF::create("00", "Serif", 60);
  //@todo esto es ubicado a ojo
  this->addChild(_timer, 99999);
  _timer->setPosition(Point(visibleSize.width - _timer->getContentSize().width * 1.3 + origin.x, visibleSize.height/2 + origin.y + _timer->getContentSize().height));
  //background
  theme = createRandomTheme();

  Sprite* bg = Sprite::create(theme->bg_path);
  bg->setPosition(Point(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
  bg->setScaleX(visibleSize.width/bg->getContentSize().width);
  bg->setScaleY(visibleSize.height/bg->getContentSize().height);
  this->addChild(bg);
  Sprite* logo = Sprite::create(theme->logo_path);
  logo->setPosition(Point(visibleSize.width + origin.x - logo->getContentSize().width/2, visibleSize.height/2 + origin.y));
  this->addChild(logo, 999);
  Sprite* middle = Sprite::create(theme->middle_path);
  this->addChild(middle);
  if(BigScreen){
    middle->setPosition(Point(middle->getContentSize().width/2 + origin.x,visibleSize.height/2 + origin.y - 70));
  }else{
    middle->setPosition(Point(middle->getContentSize().width/2 + origin.x,visibleSize.height/2 + origin.y - 40));
  }
  auto endGameItem = MenuItemImage::create(
				 "botones/salir.png",
				 "botones/salir_off.png",
				 CC_CALLBACK_1(MoreNeuron::closeGame, this)
				 );
  endGameItem->setPosition(Point(visibleSize.width - endGameItem->getContentSize().width/2 + origin.x, endGameItem->getContentSize().height/2 + origin.y));
  auto menu = Menu::create(endGameItem, NULL);

  menu->setPosition(Point::ZERO);
  this->addChild(menu, 9999);

  initializePerlas();
  initializeCruxs(); 
  translateFromCruxBoard(Point(0,1), Point(0,0));
  translateFromCruxBoardGame(Point(0,0), Point(0,1));
						
  auto dispatcher = Director::getInstance()->getEventDispatcher();
  //auto listener = EventListenerTouchOneByOne::create();
  _listener = EventListenerTouchOneByOne::create();
  _listener->onTouchBegan = CC_CALLBACK_2(MoreNeuron::onTouchBegan, this);
  _listener->onTouchMoved = CC_CALLBACK_2(MoreNeuron::onTouchMoved, this);
  _listener->onTouchEnded = CC_CALLBACK_2(MoreNeuron::onTouchEnded, this);

  dispatcher->addEventListenerWithSceneGraphPriority(_listener, this);
  CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic(_music_background.getCString(), true);
  this->schedule(schedule_selector(MoreNeuron::updateClock), 1.5f);

  //Ads
  //_admob = dynamic_cast<ProtocolAds*>(PluginManager::getInstance()->loadPlugin("AdsAdmob"));
  //TAdsDeveloperInfo devInfo;
  //devInfo["AdmobID"] = ADMOB_ID_ANDROID;
  //_admob->configDeveloperInfo(devInfo);
  //_admob->setAdsListener(this);
  //_admob->setDebugMode(false);
  //_posAdmob = ProtocolAds::kPosBottom;
  // init the AdsInfo
  //adInfo["AdmobType"] = "1";
  //adInfo["AdmobSizeEnum"] = "1";
  //_admob->showAds(adInfo,  ProtocolAds::kPosBottom);
  //_admob->hideAds(adInfo);
  return true;
}

Sprite* MoreNeuron::createSpriteRandomBackground() {
  //return Sprite::create(_background_gallery[RANDOM_INT(6)]->getCString());
  return Sprite::create(_background_gallery[0]->getCString());
}


void MoreNeuron::initializePerlas() {
  SpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("objetos/perlas.plist");
}

void MoreNeuron::initializeCruxs() {
  for(int cy = 0; cy < SIZE_OF_CRUX; cy++) {
    for(int cx = 0; cx < SIZE_OF_CRUX; cx++) {
      Crux* crux = Crux::create();
      
      if(!crux->init()){
	CCLog("Can't add crux\n");
      }
      board_crux.addCrux(Point(cy, cx), crux);

    
      _cruxs_board[cy][cx] = NULL;
    }
  }
  //conecta cruxes
  for(int cy = 0; cy < SIZE_OF_CRUX; cy++) {
    for(int cx = 0; cx < SIZE_OF_CRUX; cx++) {
      int top = cy - 1;
      if(top < 0) top = 0;
      int bottom = cy + 1;
      if(bottom >= SIZE_OF_CRUX - 1) bottom = SIZE_OF_CRUX - 1;
      int left = cx - 1;
      if(left < 0) left = 0;
      int right = cx + 1;
      if(right >= SIZE_OF_CRUX - 1) right = SIZE_OF_CRUX - 1;
      board_crux.connectCruxToCrux(Point(cx, cy), Point(cx, top));
      board_crux.connectCruxToCrux(Point(cx, cy), Point(cx, bottom));
      board_crux.connectCruxToCrux(Point(cx, cy), Point(left, cy));
      board_crux.connectCruxToCrux(Point(cx, cy), Point(right, cy));
    }
  }
  assert(board_crux.cruxAllConected() == true);
  //revuelve cruxces
  while(board_crux.cruxAllConected()) {
    for(int cy = 0; cy < SIZE_OF_CRUX; cy++) {
      RANDOM_INIT();
      for(int cx = 0; cx < SIZE_OF_CRUX; cx++) {
	int nx = 0;
	int ny = 0;
	do{
	  nx = RANDOM_INT(SIZE_OF_CRUX);
	}while(nx < cx);
	do{
	  ny = RANDOM_INT(SIZE_OF_CRUX);
	}while(ny < cy);
	if(nx == SIZE_OF_CRUX) nx = SIZE_OF_CRUX - 1;
	if(ny == SIZE_OF_CRUX) ny = SIZE_OF_CRUX - 1;
	board_crux.transposeCrux(Point(cx, cy), Point(nx, ny));
      }
    }
  }
    //assert(board_crux.cruxAllConected() == false);

  for(int cy = 0; cy < SIZE_OF_CRUX; cy++) {
    for(int cx = 0; cx < SIZE_OF_CRUX; cx++) {
      Sprite* bg_game = Sprite::create(theme->slot_holder_path.c_str());
      Point pos = board_game_crux.GridToXY(Point(cx, cy));
      board_game_crux.getNode()->addChild(bg_game, -10);
      bg_game->setAnchorPoint(Point(0,0));
      bg_game->setPosition(pos);

      Sprite* bg_holder = Sprite::create(theme->slot_game_path.c_str());
      pos = board_crux.GridToXY(Point(cx, cy));
      board_crux.getNode()->addChild(bg_holder, -10);
      bg_holder->setAnchorPoint(Point(0,0));
      bg_holder->setPosition(pos);

    }
  }
    
  if(BigScreen){
    board_crux.getNode()->setPosition(Point(0, visibleSize.height/2 - 100));
    board_game_crux.getNode()->setPosition(Point(0, -100));
  }else{
    board_crux.getNode()->setPosition(Point(0, visibleSize.height/2 - 60));
    board_game_crux.getNode()->setPosition(Point(0, -60));
  }
  this->addChild(board_crux.getNode());
  this->addChild(board_game_crux.getNode());
}


/**
 *Transla crux de placeholder a juego
 */
bool MoreNeuron::translateFromCruxBoard(Point pos, Point pos_game) {
  if(board_crux.getCrux(pos) == NULL) {
    return false;
  }
  if(board_game_crux.getCrux(pos_game) != NULL) {
    return false;
  }

  Crux* crux = board_crux.translateCrux(pos);
  board_game_crux.addCrux(pos_game, crux);
  return true;
}

/**
 *Transla crux desde juego a placeholder
 */
bool MoreNeuron::translateFromCruxBoardGame(Point pos, Point pos_crux) {
  if(board_crux.getCrux(pos_crux) != NULL) return false;
  if(board_game_crux.getCrux(pos) == NULL) return false;
  
  Crux* crux = board_game_crux.translateCrux(pos);
  board_crux.addCrux(pos_crux, crux);
  return true;
}

bool MoreNeuron::onTouchBegan(Touch* touch, Event* event) {
  if(_finished) {
    //VUELVE A EMPEZAR SI TOCA PANTALLA
    //Ads Cierre
    if(!BigScreen){
      ADMOB_NEURON_HIDE;
    }
    Scene* newScene = TransitionFade::create(0.7, MoreNeuron::createScene()); 
    Director::sharedDirector()->replaceScene(newScene);
    return false;
  }
  
  Point tap = touch->getLocation();
  CCLog("Touch began %f %f \n", tap.x, tap.y);
  CCLog("Board Placeholder %f %f\n", board_crux.getNode()->getPosition().x, board_crux.getNode()->getPosition().y);
  Point base = tap - board_crux.getNode()->getPosition();
  Point base_game = tap - board_game_crux.getNode()->getPosition();
  if(selected_crux) {
  
    if(selected_crux->boundingBox().containsPoint(tap)) {
      return true;
    }
  }

  //esta arriba
  if(tap.y > visibleSize.height/2) {
    for(int y = 0; y < board_crux.getGrid(); y++) {
      for(int x = 0; x < board_crux.getGrid(); x++) {
	const Crux *crux = board_crux.getCrux(Point(x,y));
	if(crux) {
	  if(crux->boundingBox().containsPoint(base)){
	    CCLog("Take from board placeholder\n");

	    selected_crux = board_crux.translateCrux(Point(x,y));
	    selected_crux_begin_grid = Point(x,y);
	    selected_crux_board = &board_crux;
	    tap.x -= selected_crux->getContentSize().width/2;
	    tap.y -= selected_crux->getContentSize().height/2;
	    selected_crux->setPosition(tap);
	    this->addChild(selected_crux);
	    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(_effect_grab.getCString());
	    return true;
	  }
	}
      }
    }
  }else{ //abajo
    for(int y = 0; y < board_game_crux.getGrid(); y++) {
      for(int x = 0; x < board_game_crux.getGrid(); x++) {
	const Crux *crux = board_game_crux.getCrux(Point(x,y));
	if(crux) {
	  if(crux->boundingBox().containsPoint(base_game)){
	    CCLog("Take from board placeholder\n");

	    selected_crux = board_game_crux.translateCrux(Point(x,y));
	    selected_crux_begin_grid = Point(x,y);
	    selected_crux_board = &board_game_crux;
	    tap.x -= selected_crux->getContentSize().width/2;
	    tap.y -= selected_crux->getContentSize().height/2;
	    selected_crux->setPosition(tap);
	    this->addChild(selected_crux);
	    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(_effect_grab.getCString());
	    return true;
	  }
	}
      }
    }
  }
  CCLog("Board realtive placeholder %f %f\n", base.x, base.y);
  return true;
}

void MoreNeuron::onTouchMoved(Touch* touch, Event* event) {
  //CCLog("TouchMoved\n");
  Point tap = touch->getLocation();
  if(selected_crux) {
    tap.x -= selected_crux->getContentSize().width/2;
    tap.y -= selected_crux->getContentSize().height/2;
    selected_crux->setPosition(tap);
  }
}
void MoreNeuron::onTouchEnded(Touch* touch, Event* event) {
  CCLog("TouchEnded\n");
  Point tap = touch->getLocation();
  Point base_board_game_crux =  board_game_crux.getNode()->getPosition();
  Point base_board_crux =  board_crux.getNode()->getPosition();
  if(selected_crux) {
    //arriba tablero
    if(tap.y > visibleSize.height/2) {
      Point tap_selected_crux = selected_crux->getPosition() - base_board_crux;
      Point near = board_crux.getNearPlaceXY(tap_selected_crux, cellWidth);
      Point neargrid = board_crux.XYToGrid(near);
      selected_crux->retain();
      selected_crux->removeFromParent();
      if(near == Point(-1, -1) || !board_crux.addCrux(board_crux.XYToGrid(near), selected_crux)){
	selected_crux_board->addCrux(selected_crux_begin_grid, selected_crux);
      }
    }else{ //abajo tablero
      CCLog("game");
      Point tap_selected_crux = selected_crux->getPosition() - base_board_game_crux;
      Point near = board_game_crux.getNearPlaceXY(tap_selected_crux, cellWidth);
      Point neargrid = board_game_crux.XYToGrid(near);
      selected_crux->retain();
      selected_crux->removeFromParent();	
      if(near == Point(-1, -1) || !board_game_crux.addCrux(board_crux.XYToGrid(near), selected_crux)) {
	selected_crux_board->addCrux(selected_crux_begin_grid, selected_crux);
      }
    }
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(_effect_drop.getCString());
  }

  if(board_game_crux.cruxAllConected() && !_finished) {
    _finished = true;
    this->onGameComplete();
  }

  selected_crux = NULL; //en algun lado debia haber quedado pero no flotando
}

void MoreNeuron::showMenu() {
  stopAllActions();
  unscheduleAllSelectors();
  removeFromParentAndCleanup(true);
  CocosDenshion::SimpleAudioEngine::sharedEngine()->stopBackgroundMusic(true);
  Scene* gameOver = TransitionFade::create(0.7, NeuronMenu::createScene());
  Director::sharedDirector()->replaceScene(gameOver);
}

void MoreNeuron::onGameComplete() {
  CCLog("Muy bien lo has conseguido");
  Layer* bg_credits = Layer::create();

  if(Layer::init()) {
    Sprite* bg_img = Sprite::create("mapa/fondo_fin.png");
    int last_y_item = 0;
    bg_img->setPosition(Point(visibleSize.width/2, visibleSize.height/2));
    bg_credits->addChild(bg_img);
    LabelTTF* thanks = LabelTTF::create("Thanks for playing", "ThonburiBold", 50);
    bg_credits->addChild(thanks);
    last_y_item = visibleSize.height/2 + thanks->getContentSize().height*5 + origin.y;
    thanks->setPosition(Point(visibleSize.width/2 + origin.x, last_y_item));

    LabelTTF* the_last_score = LabelTTF::create("Last Time", "Serif", 45);
    bg_credits->addChild(the_last_score);
    last_y_item -= the_last_score->getContentSize().height + 5;
    the_last_score->setPosition(Point(visibleSize.width/2 + origin.x, last_y_item));

    int last_score = UserDefault::sharedUserDefault()->getIntegerForKey("last_time", 999);
    char buff[80]; sprintf(buff, "%d", last_score);
    LabelTTF* lb_last_score = LabelTTF::create(buff, "Serif", 40);
    bg_credits->addChild(lb_last_score);
    last_y_item -= lb_last_score->getContentSize().height+ 5;
    lb_last_score->setPosition(Point(visibleSize.width/2 + origin.x, last_y_item));
    if(_time_elapsed < last_score) {
      UserDefault::sharedUserDefault()->setIntegerForKey("last_time", _time_elapsed);
      LabelTTF* the_new_score = LabelTTF::create("!!New Time!!", "ThonburiBold", 50);
      the_new_score->setFontFillColor(Color3B(255, 255, 0));
      bg_credits->addChild(the_new_score);
      last_y_item -= the_new_score->getContentSize().height + 10;
      the_new_score->setPosition(Point(visibleSize.width/2 + origin.x, last_y_item));


      char buff[80];
      sprintf(buff, "%d", _time_elapsed);
      LabelTTF* new_score = LabelTTF::create(buff, "ThonburiBold", 40);
      bg_credits->addChild(new_score);
      last_y_item -= new_score->getContentSize().height + 5;
      new_score->setPosition(Point(visibleSize.width/2 + origin.x, last_y_item));
    }
    //bg_credits->runAction(Sequence::create(FadeIn::create(1.0f), DelayTime::create(3.0f), FadeOut::create(1.0f), CallFunc::create(CC_CALLBACK_0(MoreNeuron::showMenu, this)), NULL));
    this->addChild(bg_credits, 99);
    ADMOB_NEURON_SHOW_TOP;
    //Ads
    //_admob->showAds(adInfo, _posAdmob);
  }
}

void MoreNeuron::menuCloseCallback(Object* pSender) {
}

bool MoreNeuron::isTouchEnabled() {
  return true;
}

void MoreNeuron::onExit() {
  CCLog("MORENEURON EXIT");
}

void MoreNeuron::updateClock(float dt) {
  if(_finished) return;
  _time_elapsed++;
  auto value = String::createWithFormat("%02i", _time_elapsed);
  _timer->setString(value->getCString());
}

void MoreNeuron::closeGame(Object* pSender) {
 //VOLVIENDO AL MENU
  ADMOB_NEURON_SHOW;
  //ADMOB_NEURON_LOAD;
 /* if (_admob != NULL)
    {
      _admob->hideAds(adInfo);
      PluginManager::getInstance()->unloadPlugin("AdsAdmob");
      _admob = NULL;
      }*/
  stopAllActions();
  unscheduleAllSelectors();
  removeFromParentAndCleanup(true);
  CocosDenshion::SimpleAudioEngine::sharedEngine()->stopBackgroundMusic(true);
  Scene* newScene = TransitionFade::create(0.7, NeuronMenu::createScene()); 
  Director::sharedDirector()->replaceScene(newScene);
}



MoreNeuronBG* MoreNeuron::createRandomTheme() {
  int pos = RANDOM_INT(4);
  return MoreNeuronBG::create(pos);
}

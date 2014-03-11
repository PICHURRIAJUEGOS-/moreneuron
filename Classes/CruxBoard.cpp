#include "CruxBoard.h"
#include "MoreNeuronScene.h"
#include <cassert>

USING_NS_CC;

CruxBoard::CruxBoard(): _grid(20)  {
  _cellWidth = _cellHeight = 0;
  _grid.clear();
  _node = Node::create();
}

CruxBoard::~CruxBoard(){
  _node->autorelease();
}

Node* CruxBoard::getNode() {
  return _node;
}

void CruxBoard::setGrid(int size) {
  _grid.clear();
  for(int y = 0; y < size; y++) {
    for(int x = 0; x < size; x++) {
      _grid[y].push_back(NULL);
    }
  }
  _size = size;
}

bool CruxBoard::addCrux(Point pos, Crux* crux) {
  if(_grid[pos.y][pos.x]) {
    return false;
  }

  //modelo
  _grid[pos.y][pos.x] = crux;
  //visual
  crux->setPosition(GridToXY(pos));
  _node->addChild(crux);
  return true;
}

const Crux* CruxBoard::getCrux(Point pos) {
  return _grid[pos.y][pos.x];
}

Crux* CruxBoard::translateCrux(Point pos) {
  Crux* tmp = _grid[pos.y][pos.x];

  _grid[pos.y][pos.x] = NULL;
  tmp->retain(); //si esto se genera assercion
  tmp->removeFromParent();

  return tmp;
}

Point CruxBoard::getPositionXYOfCrux(Point pos) {
  auto visibleSize = Director::getInstance()->getVisibleSize();
  float rx = DESIGN_WIDTH/visibleSize.width;
  pos.x *= rx;
  return Point(pos.x * _cellWidth * SIZE_OF_CRUX + pos.x * SIZE_OF_CRUX * SIZE_OF_CRUX, pos.y * _cellHeight * SIZE_OF_CRUX + _cellHeight);
}

Point CruxBoard::getNearPlaceXY(Point pos, float max_dist) {
  Point near(-1,-1);
  auto visibleSize = Director::getInstance()->getVisibleSize();
  float rx = DESIGN_WIDTH/visibleSize.width;
  pos.x /= rx;
  for(int y = 0; y < _size; y++) {
    for(int x = 0; x < _size; x++) {
      Point pos_crux = GridToXY(Point(x,y));
      pos_crux.x /= rx;

      //if(near == Point(-1,-1)) near = pos_crux;
      CCLog("Distance %f", pos.getDistance(pos_crux));
      if(pos.getDistance(pos_crux) < pos.getDistance(near) && pos.getDistance(pos_crux) < max_dist) {
	near = pos_crux;
      }
    }
  }
  return near;
}

Point CruxBoard::getNearPlace(Point pos, float max_dist) {

  float dist = max_dist * _cellWidth;
  Point pxy = GridToXY(pos);
  Point near = getNearPlaceXY(pxy, dist);
  return XYToGrid(near);
}

Point CruxBoard::XYToGrid(Point pos) {
  float x = (int)(pos.x / (_cellWidth*SIZE_OF_CRUX));
  float y = (int)(pos.y / (_cellHeight*SIZE_OF_CRUX)) ;
  assert(x < _size);
  assert(y < _size);

  return Point(x,y);
}

Point CruxBoard::GridToXY(Point pos) {
  Point origin = Director::getInstance()->getVisibleOrigin();
  auto cwidth = _cellWidth * SIZE_OF_CRUX;
  auto cheight = _cellHeight * SIZE_OF_CRUX;
  auto visibleSize = Director::getInstance()->getVisibleSize();
  float rx = DESIGN_WIDTH/visibleSize.width;
  return Point(rx * pos.x * cwidth + origin.x, pos.y * cheight + origin.y + _cellHeight);
}

int CruxBoard::connectCruxToCrux(Point c1, Point c2) {
  Crux* crux1 = _grid[c1.y][c1.x];
  Crux* crux2 = _grid[c2.y][c2.x];
  if(!crux1 || !crux2) return 0;
  int v = c2.y - c1.y;
  int h = c2.x - c1.x;
  if(v < 0) {
    crux1->setPerlaBottom(crux2->getPerlaTop());
  }else if(v > 0) {
    crux1->setPerlaTop(crux2->getPerlaBottom());
  }

  if(h < 0) {
    crux1->setPerlaLeft(crux2->getPerlaRight());
  }else if(h > 0) {
    crux1->setPerlaRight(crux2->getPerlaLeft());
  }
}

void CruxBoard::transposeCrux(Point c1, Point c2) {
  Crux* tmp = _grid[c1.y][c1.x];
  _grid[c1.y][c1.x] = _grid[c2.y][c2.x];
  _grid[c2.y][c2.x] = tmp;
  _grid[c1.y][c1.x]->setPosition(GridToXY(c1));
  _grid[c2.y][c2.x]->setPosition(GridToXY(c2));
}

bool CruxBoard::cruxAllConected() {
  for(int cy = 0; cy < _size - 1; cy++) {
    for(int cx = 0; cx < _size; cx++) {
      if(!_grid[cy][cx]) return false;
    }
  }

  for(int cx = 0; cx < _size - 1; cx++) {
    for(int cy = 0; cy < _size; cy++) {
      Crux* left = _grid[cy][cx];
      Crux* right = _grid[cy][cx+1];
      if(!left || !right) return false;
      if(left->getPerlaRight() != right->getPerlaLeft()) return false;
    }
  }
  for(int cy = 0; cy < _size - 1; cy++) {
    for(int cx = 0; cx < _size; cx++) {
      Crux* bottom = _grid[cy][cx];
      Crux* top = _grid[cy + 1][cx];
      if(!bottom || !top) return false;
      if(bottom->getPerlaTop() != top->getPerlaBottom()) return false;
    }
  }
  return true;
}










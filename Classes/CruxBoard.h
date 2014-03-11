#ifndef CRUXBOARD_H
#define CRUXBOARD_H

#include "cocos2d.h"
#include <vector>
#include "Crux.h"

/**
 *Clase base para tablero que almacene y mueve Crux
 *Aqui el *Point* es en base a la grilla del tablero y no en pixel.
 */
class CruxBoard 
{
 public:
  CruxBoard();
  ~CruxBoard();
  cocos2d::Node* getNode();

  cocos2d::Point XYToGrid(cocos2d::Point);
  cocos2d::Point GridToXY(cocos2d::Point);

  CC_SYNTHESIZE(int, _cellWidth, CellWidth);
  CC_SYNTHESIZE(int, _cellHeight, CellHeight);
  /**
   *Tamano de la cuadriculo
   *un solo lado
   */
  void setGrid(int);
  int getGrid() { return _size; }

  /**
   *Obtiene la posicion en pixel
   *de la crux indicada
   */
  cocos2d::Point getPositionXYOfCrux(cocos2d::Point);

  /**
   *Retorna el punto mas cerca de la crux mas cercana en pixeles
   *desde el punto indicado y con la maxima distancia indicada
   *Retorna -1,-1 si no hay cercano
   */
  cocos2d::Point getNearPlaceXY(cocos2d::Point, float);

  /**
   *Retorna la crux (vacia) mas cercana en medidas malla (grid)
   */
  cocos2d::Point getNearPlace(cocos2d::Point, float);

  /**
   *Conecta las perlas de las cruxes indicadas.
   *!!Esto reemplaza la perla actual de cada crux.
   *Retorna la cantidad de conexiones que se realizo 0 en ninguna
   */
  int connectCruxToCrux(cocos2d::Point c1, cocos2d::Point c2);

  /**
   *Indica si estan conectada todas las cruxes o no
   */
  bool cruxAllConected();

  /**
   *Intercambia de posicion cruxes
   */
  void transposeCrux(cocos2d::Point c1, cocos2d::Point c2);
  /**
   *Agrega nueva crux al tablero en la posicion indicada
   *Retorna verdadero si ya existe crux en la posicion indicado de lo contrario falso
   */
  virtual bool addCrux(cocos2d::Point, Crux*);

  /**
   *Retorna *Crux* en la posicion indicada
   */
  virtual const Crux* getCrux(cocos2d::Point);

  /**
   *Retorna *Crux* en la posicion indicada y elimina del tablero
   */
  virtual Crux* translateCrux(cocos2d::Point);

 private:
  std::vector< std::vector< Crux* > > _grid;
  int _size;
  cocos2d::Node* _node;

};

#endif

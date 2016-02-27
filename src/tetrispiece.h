#ifndef TETRISPIECE_H
#define TETRISPIECE_H

enum TetrisShape { NoShape, ZShape, SShape, LineShape, TShape, SquareShape, LShape, MirroredLShape };
#include "random.h"
#include <algorithm>

class TetrisPiece
{
public:
   TetrisPiece();

   void setRandomShape(int nbr);
   void setShape(TetrisShape shape);

   TetrisShape shape() const { return pieceShape; }
   int x(int index) const { return coords[index][0]; }
   int y(int index) const { return coords[index][1]; }
   int minX() const;
   int maxX() const;
   int minY() const;
   int maxY() const;
   TetrisPiece rotatedLeft() const;
   TetrisPiece rotatedRight() const;

private:
   void setX(int index, int x) { coords[index][0] = x; }
   void setY(int index, int y) { coords[index][1] = y; }

   TetrisShape pieceShape;
   int coords[4][2];
};

#endif // TETRISPIECE_H

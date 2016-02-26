#ifndef TETRISKI_H
#define TETRISKI_H

#include <vector>
#include "tetrisfeatures.h"

class TetrisBoard;

class TetrisKi
{
private:
   std::vector<double> mWeights;

   double evaluateBoard(TetrisBoard* board, int lines, int landingHeight);
public:
   TetrisKi();
   TetrisKi(std::vector<double> weights);

   void setWeights(std::vector<double> weights);
   std::vector<double> getWeights();

   std::pair<int, int> onePiece(TetrisBoard* board);
   std::pair<int, int> twoPiece(TetrisBoard* board);
   std::pair<int, int> threePiece(TetrisBoard* board);
};

#endif // TETRISKI_H

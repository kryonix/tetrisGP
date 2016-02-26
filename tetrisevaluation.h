#ifndef TETRISEVALUATION_H
#define TETRISEVALUATION_H

#include <vector>

#include "tetriski.h"
#include "individuum.h"
#include "tetrisfeatures.h"

#include <QDebug>

class TetrisBoard;

class TetrisEvaluation
{
private:
   Individuum* mIndividuum;
   TetrisKi mKi;
   TetrisBoard* board;
public:
   TetrisEvaluation();
   TetrisEvaluation(Individuum* individuum);
   ~TetrisEvaluation();

   int evaluate();

   //void eval();
   void eval(Individuum& individuum);

   void operator()(Individuum& individuum);
};

#endif // TETRISEVALUATION_H

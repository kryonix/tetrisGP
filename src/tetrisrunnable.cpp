#include "tetrisrunnable.h"

TetrisRunnable::TetrisRunnable(Individuum& individuum)
   : mIndividuum(individuum)
{
}

TetrisRunnable::~TetrisRunnable()
{
}

void TetrisRunnable::run()
{
   mEvaluation(mIndividuum);
   //mEvaluation.evaluate();
}

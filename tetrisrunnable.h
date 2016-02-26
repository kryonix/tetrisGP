#ifndef TETRISRUNNABLE_H
#define TETRISRUNNABLE_H

#include <QRunnable>

#include "individuum.h"
#include "tetrisevaluation.h"

class TetrisRunnable : public QRunnable
{
private:
   TetrisEvaluation mEvaluation;
   Individuum& mIndividuum;
public:
   explicit TetrisRunnable(Individuum& individuum);
   ~TetrisRunnable();
   void run();
};

#endif // TETRISRUNNABLE_H

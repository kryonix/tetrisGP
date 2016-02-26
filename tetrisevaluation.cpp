#include "tetrisevaluation.h"
#include "tetrisboard.h"

TetrisEvaluation::TetrisEvaluation()
{
   board = new TetrisBoard;
}

TetrisEvaluation::TetrisEvaluation(Individuum *individuum)
{
   mIndividuum = individuum;
   mKi.setWeights(mIndividuum->weights);

   board = new TetrisBoard;
}

TetrisEvaluation::~TetrisEvaluation()
{
   delete board; //?
}

int TetrisEvaluation::evaluate()
{
   int linesRemoved = 0;

   for(int attempts = 0; attempts < 3; attempts++)
   {
      board->start();

      while(!board->lost())
      {
   #if 1
         for(int i = 0; i < board->BoardHeight-maxHeight(board)-5; i++)
         {
            board->oneLineDown();
         }
   #endif

         std::pair<int, int> zug = mKi.onePiece(board); //mKi.twoPiece(board);
         play(board, zug.first, zug.second);

         board->oneLineDown();
         //board->dropDown();
         board->timerEvent();
      }

      linesRemoved += board->getLinesRemoved();
   }

   return linesRemoved/3;
}
/*
void TetrisEvaluation::eval()
{
   mIndividuum->fitness = evaluate();
}
*/
void TetrisEvaluation::eval(Individuum& individuum)
{
   mKi.setWeights(individuum.weights);
   individuum.fitness = evaluate();
}

void TetrisEvaluation::operator ()(Individuum &individuum)
{
   mKi.setWeights(individuum.weights);
   individuum.fitness = evaluate();
}

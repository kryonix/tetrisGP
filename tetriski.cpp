#include "tetriski.h"
#include "tetrisboard.h"

TetrisKi::TetrisKi(std::vector<double> weights)
   : mWeights(weights)
{
}

TetrisKi::TetrisKi()
{
}

void TetrisKi::setWeights(std::vector<double> weights)
{
   mWeights = weights;
}

std::vector<double> TetrisKi::getWeights()
{
   return mWeights;
}

double TetrisKi::evaluateBoard(TetrisBoard *board, int lines, int landingHeight)
{
   double score = 0;

   score -= weightedHeight(board, mWeights.at(0));
   score -= holes(board) * mWeights.at(1);
   score -= blockades(board) * mWeights.at(2);
   score -= columnSpace(board) * mWeights.at(3);
   score += touchFloor(board) * mWeights.at(4);
   score += touchWall(board) * mWeights.at(5);
   score += touchPiece(board) * mWeights.at(6);
   score -= rowSpace(board) * mWeights.at(7);
   score -= blocks(board) * mWeights.at(8);
   score += lines * mWeights.at(9);
   score -= well(board) * mWeights.at(10);
   score -= landingHeight+board->getCurPiece().maxX()/2 * mWeights.at(11);

   return score;
}

std::pair<int, int> TetrisKi::onePiece(TetrisBoard *board)
{
   int rot = 4;
   if(board->getCurPiece().shape() == SquareShape) rot = 0;
   if(board->getCurPiece().shape() == ZShape) rot = 1;
   if(board->getCurPiece().shape() == SShape) rot = 1;
   if(board->getCurPiece().shape() == LineShape) rot = 1;

   int landingHeight = 0;
   int lines = board->getLinesRemoved();

   int move = 0;
   int rotate = 0;
   double bestScore = -100000;

   board->savePimpl();

   for(int i = -(board->BoardWidth/2)-1; i <= (board->BoardWidth/2)+1; i++)
   {
      for(int j = 0; j <= rot; j++)
      {
         lines = board->getLinesRemoved();
         play(board, i, j);
         landingHeight = board->dropDownNoNewPiece();

         lines = board->getLinesRemoved() - lines;

         double score = evaluateBoard(board, lines, landingHeight);

         if(score > bestScore)
         {
            //qDebug() << score;
            bestScore = score;
            move = i;
            rotate = j;
         }
         board->restorePimplNoPop();
      }
   }

   board->restorePimpl();

   return std::pair<int, int>(move, rotate);
}

std::pair<int, int> TetrisKi::twoPiece(TetrisBoard *board)
{
   int lines = board->getLinesRemoved();
   int landingHeight = 0;
   int rot = 3;
   if(board->getCurPiece().shape() == SquareShape) rot = 0;
   if(board->getCurPiece().shape() == ZShape) rot = 1;
   if(board->getCurPiece().shape() == SShape) rot = 1;
   if(board->getCurPiece().shape() == LineShape) rot = 1;

   int rot2 = 3;
   if(board->getNextPiece().shape() == SquareShape) rot2 = 0;
   if(board->getNextPiece().shape() == ZShape) rot2 = 1;
   if(board->getNextPiece().shape() == SShape) rot2 = 1;
   if(board->getNextPiece().shape() == LineShape) rot2 = 1;

   int move = 0;
   int rotate = 0;
   double bestScore = -100000;

   board->savePimpl();

   for(int i = -(board->BoardWidth/2)-1; i <= (board->BoardWidth/2)+1; i++)
   {
      for(int j = 0; j <= rot; j++)
      {
         play(board, i, j);
         landingHeight = board->dropDownNoNewPiece();
         board->savePimpl();

         for(int k = -(board->BoardWidth/2)-1; k <= (board->BoardWidth/2)+1; k++)
         {
            for(int l = 0; l <= rot2; l++)
            {
               play(board, k, l);
               board->dropDownNoNewPiece();

               double score = evaluateBoard(board, board->getLinesRemoved() - lines, landingHeight);

               if(score > bestScore)
               {
                  //qDebug() << score;
                  bestScore = score;
                  move = i;
                  rotate = j;
               }

               board->restorePimplNoPop();
            }
         }
         board->popPimpl();
         board->restorePimplNoPop();
      }
   }

   board->restorePimpl();

   return std::pair<int, int>(move, rotate);
}

std::pair<int, int> TetrisKi::threePiece(TetrisBoard *board)
{
   int lines = board->getLinesRemoved();

   int rot = 3;
   if(board->getCurPiece().shape() == SquareShape) rot = 0;
   if(board->getCurPiece().shape() == ZShape) rot = 1;
   if(board->getCurPiece().shape() == SShape) rot = 1;
   if(board->getCurPiece().shape() == LineShape) rot = 1;
   int rot2 = 3;
   if(board->getCurPiece().shape() == SquareShape) rot2 = 0;
   if(board->getCurPiece().shape() == ZShape) rot2 = 1;
   if(board->getCurPiece().shape() == SShape) rot2 = 1;
   if(board->getCurPiece().shape() == LineShape) rot2 = 1;
   int rot3 = 3;
   if(board->getCurPiece().shape() == SquareShape) rot3 = 0;
   if(board->getCurPiece().shape() == ZShape) rot3 = 1;
   if(board->getCurPiece().shape() == SShape) rot3 = 1;
   if(board->getCurPiece().shape() == LineShape) rot3 = 1;

   int move = 0;
   int rotate = 0;
   double bestScore = -100000;

   int landingHeight = 0;

   board->savePimpl();

   for(int i = -6; i <= 6; i++)
   {
      for(int j = 0; j <= rot; j++)
      {
         play(board, i, j);
         landingHeight = board->dropDownNoNewPiece();
         board->savePimpl();

         for(int k = -6; k <= 6; k++)
         {
            for(int l = 0; l <= rot2; l++)
            {
               play(board, k, l);
               board->dropDownNoNewPiece();
               board->savePimpl();
               for(int m = -6; m <= 6; m++)
               {
                  for(int n = 0; n <= rot3;n++)
                  {
                     play(board, m, n);
                     board->dropDownNoNewPiece();

                     double score = evaluateBoard(board, board->getLinesRemoved() - lines, landingHeight);

                     if(score > bestScore)
                     {
                        //qDebug() << score;
                        bestScore = score;
                        move = i;
                        rotate = j;
                     }
                     board->restorePimplNoPop();
                  }
               }
               board->popPimpl();
               board->restorePimplNoPop();
            }
         }
         board->popPimpl();
         board->restorePimplNoPop();
      }
   }

   board->restorePimpl();
   return std::pair<int, int>(move, rotate);
}

#include "tetriski.h"

#include "tetrisboard.h"
#include <bitset>

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

   std::bitset<TetrisBoard::BoardHeight*TetrisBoard::BoardWidth> boardSet;

   for(int x = 0; x < board->BoardWidth; ++x)
   {
       for(int y = 0; y < board->BoardHeight; ++y)
       {
           boardSet[(y*TetrisBoard::BoardWidth)+x] = board->shapeAt(x,y) == NoShape ? true:false;
       }
   }

   std::vector<int> heights;
   heights.resize(TetrisBoard::BoardWidth);

   for(int i = 0; i < TetrisBoard::BoardWidth; i++)
   {
      int max = 0;
      heights[i] = 0;
      for(int j = 0; j < TetrisBoard::BoardHeight; j++)
      {
         if(!boardSet.test((TetrisBoard::BoardHeight-j-1)*TetrisBoard::BoardWidth+i))
         {
            if(max < TetrisBoard::BoardHeight-j)
            {
               max = TetrisBoard::BoardHeight-j;
               heights[i] = max;
            }
         }
      }
   }
   int aggregateHeight = heights[heights.size()-1];
   int bumpiness = 0;

   for(int i = 0; i < heights.size()-2; ++i)
   {
       bumpiness += std::abs(heights[i]-heights[i+1]);
       aggregateHeight += heights[i];
   }

   score = (-aggregateHeight * mWeights.at(10)) - (bumpiness * mWeights.at(11));

#if 1
   score -= weightedHeight(&boardSet, mWeights.at(0));

   //score -= holesBlockades(&boardSet, mWeights.at(1), mWeights.at(2));

   score -= holesBlockadesColumn(&boardSet, mWeights.at(1), mWeights.at(2), mWeights.at(3));

   //score -= holes(&boardSet) * mWeights.at(1);
   //score -= blockades(&boardSet) * mWeights.at(2);
   //score -= columnSpace(&boardSet) * mWeights.at(3);

   score += touchFloor(&boardSet) * mWeights.at(4);
   score += touchWall(&boardSet) * mWeights.at(5);
   score += touchPiece(&boardSet) * mWeights.at(6);
   score -= rowSpace(&boardSet) * mWeights.at(7);
   score -= blocks(&boardSet) * mWeights.at(8);
   score += lines * mWeights.at(9);
   score -= well(&boardSet) * mWeights.at(10);
   score -= landingHeight+board->getCurPiece().maxX()/2 * mWeights.at(11);
#endif
   return score;
}

std::pair<int, int> TetrisKi::onePiece(TetrisBoard *board)
{
   int rot = 3;
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

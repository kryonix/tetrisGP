#include "tetrisfeatures.h"
#include "tetrisboard.h"

void play(TetrisBoard *board, int move, int rotate)
{
   for(int i = 0; i < rotate; ++i)
   {
      if(!board->rotateLeft())
      {
         break;
      }
   }

   if(move == 0) return;

   if(move < 0)
   {
      for(int i = move; i < 0; ++i)
      {
         if(!board->moveLeft())
         {
            break;
         }
      }
   }
   else
   {
      for(int i = 0; i < move; ++i)
      {
         if(!board->moveRight())
         {
            break;
         }
      }
   }
}

int blocks(TetrisBoard* board)
{
   int cnt = 0;
   for(int x = 0; x < board->BoardWidth; ++x)
   {
      for(int y = 0; y < board->BoardHeight; ++y)
      {
         if(board->shapeAt(x, y) != NoShape)
         {
            ++cnt;
         }
      }
   }

   return cnt;
}

double weightedHeight(TetrisBoard* board, double weight)
{
   double score = 0;
   for(int x = 0; x < board->BoardWidth; ++x)
   {
      for(int y = 0; y < board->BoardHeight; ++y)
      {
         if(board->shapeAt(x, y) != NoShape)
         {
            score += (y+1) * weight;
         }
      }
   }

   return score;
}

int holes(TetrisBoard* board)
{
   int cnt = 0;
   for(int x = 0; x < board->BoardWidth; ++x)
   {
      for(int y = 0; y < board->BoardHeight-1 ; ++y)
      {
         if(board->shapeAt(x, y) == NoShape && board->shapeAt(x, y+1) != NoShape)
         {
            ++cnt;
         }
      }
   }

   return cnt;
}

int blockades(TetrisBoard* board)
{
   int cnt = 0;
   for(int x = 0; x < board->BoardWidth; x++) //blockade
   {
      for(int y = 0; y < board->BoardHeight-1 ; y++)
      {
         if(board->shapeAt(x, y) == NoShape)
         {
            if(board->shapeAt(x, y+1) != NoShape)
            {
               ++cnt;

               for(int i = y+2; i < board->BoardHeight; i++)
               {
                  if(board->shapeAt(x, i) != NoShape)
                  {
                     ++cnt;
                  }
                  else
                  {
                     break;
                  }
               }
            }
         }
      }
   }

   return cnt;
}

int columnSpace(TetrisBoard* board)
{
   int cnt = 0;
   for(int y = 0; y < board->BoardHeight-1; ++y)
   {
      for(int x = 0; x < board->BoardWidth; ++x)
      {
         if(board->shapeAt(x, y) != NoShape && board->shapeAt(x, y+1) == NoShape)
         {
            ++cnt;
         }
      }
   }

   return cnt;
}

int touchFloor(TetrisBoard* board)
{
   int cnt = 0;
   for(int k = 0; k < board->BoardWidth; k++)
   {
      if(board->shapeAt(k, 0) != NoShape)
      {
         ++cnt;
      }
   }

   return cnt;
}

int touchWall(TetrisBoard* board)
{
   int cnt = 0;
   for(int y = 0; y < board->BoardHeight; ++y)
   {
      if(board->shapeAt(0, y) != NoShape)
      {
         ++cnt;
      }
      if(board->shapeAt(board->BoardWidth-1, y) != NoShape)
      {
         ++cnt;
      }
   }

   return cnt;
}

int touchPiece(TetrisBoard* board)
{
   int cnt = 0;
   for(int x = 0; x < board->BoardWidth; x++)   //touching another block
   {
      for(int y = 0; y < board->BoardHeight; y++)
      {
         if(board->shapeAt(x, y) != NoShape)
         {
            if(board->shapeAt(x, y-1) != NoShape)
            {
               ++cnt;
            }
            if(board->shapeAt(x, y+1) != NoShape)
            {
               ++cnt;
            }
            if(board->shapeAt(x-1, y) != NoShape)
            {
               ++cnt;
            }
            if(board->shapeAt(x+1, y) != NoShape)
            {
               ++cnt;
            }
         }
      }
   }
   return cnt;
}

int rowSpace(TetrisBoard* board)
{
   int cnt = 0;
   for(int y = 0; y < board->BoardHeight; y++)
   {
      for(int x = 0; x < board->BoardWidth-1; x++)
      {
         if(board->shapeAt(x, y) != NoShape && board->shapeAt(x+1, y) == NoShape)
         {
            ++cnt;
         }
      }
   }

   return cnt;
}

int well(TetrisBoard* board)
{
   int cnt = 0;
/*
   for(int x = 0; x < board->BoardWidth-2; x++)
   {
      for(int y = 0; y < board->BoardHeight; y++)
      {
         if(board->shapeAt(x, y) != NoShape && board->shapeAt(x+1, y) == NoShape)
         {
            for(int i = x+1; i < board->BoardWidth-2; i++)
            {
               if(board->shapeAt(i, y) == NoShape && board->shapeAt(i+1, y) != NoShape)
               {
                  cnt += i-x;

                  for(int j = x; j < i; j++)
                  {
                     for(int k = y; k >= 0; k--)
                     {
                        if(board->shapeAt(j, k) == NoShape)
                        {
                           ++cnt;
                        }
                        else
                        {
                           break;
                        }
                     }
                  }

                  //break;
               }
            }
         }
      }
   }
*/

   for(int x = 1; x < board->BoardWidth-1; ++x)
   {
      for(int y = board->BoardHeight-1; y >= 0; --y)
      {
         if(board->shapeAt(x, y) == NoShape && board->shapeAt(x-1, y) != NoShape && board->shapeAt(x+1, y) != NoShape)
         {
            ++cnt;

            for(int k = y-1; k >= 0; --k)
            {
               if(board->shapeAt(x, k) == NoShape)
               {
                  ++cnt;
               }
               else
               {
                  break;
               }
            }
         }
      }
   }

   for(int y = board->BoardHeight-1; y >= 0; --y)
   {
      if(board->shapeAt(0, y) == NoShape && board->shapeAt(1, y) != NoShape)
      {
         ++cnt;

         for(int k = y-1; k >= 0; --k)
         {
            if(board->shapeAt(0, k) == NoShape)
            {
               ++cnt;
            }
            else
            {
               break;
            }
         }
      }
   }

   for(int y = board->BoardHeight-1; y >= 0; --y)
   {
      if(board->shapeAt(board->BoardWidth-1, y) == NoShape && board->shapeAt(board->BoardWidth-2, y) != NoShape)
      {
         ++cnt;

         for(int k = y-1; k >= 0; --k)
         {
            if(board->shapeAt(board->BoardWidth-1, k) == NoShape)
            {
               ++cnt;
            }
            else
            {
               break;
            }
         }
      }
   }

   return cnt;
}

int maxHeight(TetrisBoard* board)
{
   int max = 0;
   for(int i = 0; i < board->BoardWidth; i++)
   {
      for(int j = 0; j < board->BoardHeight; j++)
      {
         if(board->shapeAt(i, board->BoardHeight-j-1) != NoShape)
         {
            if(max < board->BoardHeight-j)
            {
               max = board->BoardHeight-j;
            }
         }
      }
   }

   return max;
}

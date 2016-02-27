#include "tetrisfeatures.h"

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

int blocks(const std::bitset<TetrisBoard::Fields>* board)
{
   return board->count();
}

double weightedHeight(const std::bitset<TetrisBoard::Fields>* board, double weight)
{
   double score = 0;
   for(int x = 0; x < TetrisBoard::BoardWidth; ++x)
   {
      for(int y = 0; y < TetrisBoard::BoardHeight; ++y)
      {
         if(board->test((y*TetrisBoard::BoardWidth)+x))
         {
            score += (y+1);// * weight;
         }
      }
   }

   return score*weight*0.005;
}

int holes(const std::bitset<TetrisBoard::Fields>* board)
{
   int cnt = 0;
   for(int x = 0; x < TetrisBoard::BoardWidth; ++x)
   {
      for(int y = 0; y < TetrisBoard::BoardHeight-1 ; ++y)
      {
         if(!board->test((y*TetrisBoard::BoardWidth)+x) && board->test(((y+1)*TetrisBoard::BoardWidth)+x))
         {
            ++cnt;
         }
      }
   }

   return cnt;
}

int blockades(const std::bitset<TetrisBoard::Fields>* board)
{
   int cnt = 0;
   for(int x = 0; x < TetrisBoard::BoardWidth; x++) //blockade
   {
      for(int y = 0; y < TetrisBoard::BoardHeight-1 ; y++)
      {
         if(!board->test((y*TetrisBoard::BoardWidth)+x) && board->test(((y+1)*TetrisBoard::BoardWidth)+x))
         {
            ++cnt;

            for(int i = y+2; i < TetrisBoard::BoardHeight; i++)
            {
               if(board->test((i*TetrisBoard::BoardWidth)+x))
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

   return cnt;
}

double holesBlockades(const std::bitset<TetrisBoard::Fields>* board, double holeW, double blockadeW)
{
    int holes = 0;
    int blockades = 0;
    for(int x = 0; x < TetrisBoard::BoardWidth; ++x)
    {
       for(int y = 0; y < TetrisBoard::BoardHeight-1 ; ++y)
       {
          if(!board->test((y*TetrisBoard::BoardWidth)+x) && board->test(((y+1)*TetrisBoard::BoardWidth)+x))
          {
              ++holes;
              ++blockades;
              for(int i = y+2; i < TetrisBoard::BoardHeight; i++)
              {
                 if(board->test((i*TetrisBoard::BoardWidth)+x))
                 {
                    ++blockades;
                 }
                 else
                 {
                    break;
                 }
              }
          }
       }
    }

    return holes*holeW + blockades*blockadeW;
}

double holesBlockadesColumn(const std::bitset<TetrisBoard::Fields>* board, double holeW, double blockadeW, double columnW)
{
    int holes = 0;
    int blockades = 0;
    int column = 0;

    bool bNext = board->test(0);

    for(int x = 0; x < TetrisBoard::BoardWidth; ++x)
    {
        bNext = board->test(x);
       for(int y = 0; y < TetrisBoard::BoardHeight-1 ; ++y)
       {
           bool a = bNext;//board->test((y*TetrisBoard::BoardWidth)+x);
           bool b = board->test(((y+1)*TetrisBoard::BoardWidth)+x);
           bNext = b;
          if(!a && b)
          {
              ++holes;
              ++blockades;
              for(int i = y+2; i < TetrisBoard::BoardHeight; i++)
              {
                 if(board->test((i*TetrisBoard::BoardWidth)+x))
                 {
                    ++blockades;
                 }
                 else
                 {
                    break;
                 }
              }
          }
          else if(a && !b)
          {
              ++column;
          }
       }
    }

    return holes*holeW + blockades*blockadeW + column*columnW;
}

int columnSpace(const std::bitset<TetrisBoard::Fields>*  board)
{
   int cnt = 0;
   for(int y = 0; y < TetrisBoard::BoardHeight-1; ++y)
   {
      for(int x = 0; x < TetrisBoard::BoardWidth; ++x)
      {
         if(board->test((y*TetrisBoard::BoardWidth)+x) && !board->test(((y+1)*TetrisBoard::BoardWidth)+x))
         {
            ++cnt;
         }
      }
   }

   return cnt;
}

int touchFloor(const std::bitset<TetrisBoard::Fields>* board)
{
   int cnt = 0;
   for(int k = 0; k < TetrisBoard::BoardWidth; k++)
   {
      if(board->test((0*TetrisBoard::BoardWidth)+k))
      {
         ++cnt;
      }
   }

   return cnt;
}

int touchWall(const std::bitset<TetrisBoard::Fields>* board)
{
   int cnt = 0;
   for(int y = 0; y < TetrisBoard::BoardHeight; ++y)
   {
      if(board->test((0*TetrisBoard::BoardWidth)+y))
      {
         ++cnt;
      }
      if(board->test((y*TetrisBoard::BoardWidth)+TetrisBoard::BoardWidth-1))
      {
         ++cnt;
      }
   }

   return cnt;
}

int touchPiece(const std::bitset<TetrisBoard::Fields>* board)
{
   int cnt = 0;
   for(int x = 0; x < TetrisBoard::BoardWidth-1; x+=2)   //touching another block
   {
      for(int y = 0; y < TetrisBoard::BoardHeight-1; y+=2)
      {
         if(board->test((y*TetrisBoard::BoardWidth)+x))
         {
            if(y > 1 && board->test(((y-1)*TetrisBoard::BoardWidth)+x))
            {
               ++cnt;
            }
            if(board->test(((y+1)*TetrisBoard::BoardWidth)+x))
            {
               ++cnt;
            }
            if(x > 1 && board->test((y*TetrisBoard::BoardWidth)+x-1))
            {
               ++cnt;
            }
            if(board->test((y*TetrisBoard::BoardWidth)+x+1))
            {
               ++cnt;
            }
         }
      }
   }
   return cnt;
}

int rowSpace(const std::bitset<TetrisBoard::Fields>* board)
{
   int cnt = 0;
   for(int y = 0; y < TetrisBoard::BoardHeight; y++)
   {
      for(int x = 0; x < TetrisBoard::BoardWidth-1; x++)
      {
         if(board->test((y*TetrisBoard::BoardWidth)+x) && !board->test((y*TetrisBoard::BoardWidth)+x+1))
         {
            ++cnt;
         }
      }
   }

   return cnt;
}

int well(const std::bitset<TetrisBoard::Fields>* board)
{
/*    std::vector<int> heights;
    heights.resize(TetrisBoard::BoardWidth);

    for(int i = 0; i < TetrisBoard::BoardWidth; i++)
    {
       int max = 0;
       heights[i] = 0;
       for(int j = 0; j < TetrisBoard::BoardHeight; j++)
       {
          if(!board->test((TetrisBoard::BoardHeight-j-1)*TetrisBoard::BoardWidth+i))
          {
             if(max < TetrisBoard::BoardHeight-j)
             {
                max = TetrisBoard::BoardHeight-j;
                heights[i] = max;
             }
          }
       }
    }

    int bumpiness = 0;

    for(int i = 0; i < heights.size()-2; ++i)
    {
        bumpiness += std::abs(heights[i]-heights[i+1]);
    }

    return bumpiness;*/

   int cnt = 0;

   for(int x = 1; x < TetrisBoard::BoardWidth-1; ++x)
   {
      for(int y = TetrisBoard::BoardHeight-1; y >= 0; --y)
      {
         if(!board->test((y*TetrisBoard::BoardWidth)+x) && board->test((y*TetrisBoard::BoardWidth)+x-1) && board->test((y*TetrisBoard::BoardWidth)+x+1))
         {
            ++cnt;

            for(int k = y-1; k >= 0; --k)
            {
               if(!board->test((k*TetrisBoard::BoardWidth)+x))
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

   for(int y = TetrisBoard::BoardHeight-1; y >= 0; --y)
   {
      if(!board->test((y*TetrisBoard::BoardWidth)+0) && board->test((y*TetrisBoard::BoardWidth)+1))
      {
         ++cnt;

         for(int k = y-1; k >= 0; --k)
         {
            if(!board->test((k*TetrisBoard::BoardWidth)+0))
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

   for(int y = TetrisBoard::BoardHeight-1; y >= 0; --y)
   {

      if(!board->test((y*TetrisBoard::BoardWidth)+TetrisBoard::BoardWidth-1) && !board->test((y*TetrisBoard::BoardWidth)+TetrisBoard::BoardWidth-2))
      {
         ++cnt;

         for(int k = y-1; k >= 0; --k)
         {
            if(!board->test((k*TetrisBoard::BoardWidth)+TetrisBoard::BoardWidth-1))
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

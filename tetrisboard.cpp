#include "tetrisboard.h"

TetrisBoard::TetrisBoard()
   : mem(new Pimpl)
{
   mem->isStarted = false;
   mem->isPaused = false;
   mem->genNewPiece = true;
   clearBoard();

   mRandom.setSeed(0);

   mShapes.assign(7, 0);

   mem->nextPiece.setRandomShape(mRandom.rnd(1, 7));
   mem->nextNextPiece.setRandomShape(mRandom.rnd(1, 7));

   showNextPieces = true;
}

void TetrisBoard::start()
{
   mShapes.clear();
   mShapes.assign(7, 0);

   if (mem->isPaused)
      return;

   mem->genNewPiece = true;
   mem->isStarted = true;
   mem->isWaitingAfterLine = false;
   mem->numLinesRemoved = 0;
   mem->numPiecesDropped = 0;
   mem->score = 0;
   mem->level = 1;
   clearBoard();

   newPiece();
}

void TetrisBoard::pause()
{
   if (!mem->isStarted)
      return;

   mem->isPaused = !mem->isPaused;
}

bool TetrisBoard::moveLeft()
{
   return tryMove(mem->curPiece, mem->curX - 1, mem->curY);
}

bool TetrisBoard::moveRight()
{
   return tryMove(mem->curPiece, mem->curX + 1, mem->curY);
}

bool TetrisBoard::rotateLeft()
{
   return tryMove(mem->curPiece.rotatedLeft(), mem->curX, mem->curY);
}

bool TetrisBoard::rotateRight()
{
   return tryMove(mem->curPiece.rotatedRight(), mem->curX, mem->curY);
}

TetrisPiece TetrisBoard::getCurPiece()
{
   return mem->curPiece;
}

TetrisPiece TetrisBoard::getNextPiece()
{
   return mem->nextNextPiece;
}

bool TetrisBoard::getStarted() const
{
   return mem->isStarted;
}

int TetrisBoard::getLinesRemoved() const
{
   return mem->numLinesRemoved;
}

int TetrisBoard::getScore() const
{
   return mem->score;
}

int TetrisBoard::getLevel() const
{
   return mem->level;
}

void TetrisBoard::timerEvent(QTimerEvent *event)
{
   if (mem->isWaitingAfterLine) {
      mem->isWaitingAfterLine = false;
      newPiece();
   } else {
      oneLineDown();
   }
}

bool TetrisBoard::lost() const
{
   return !mem->isStarted;
}

void TetrisBoard::clearBoard()
{
   for (int i = 0; i < BoardHeight * BoardWidth; ++i)
      mem->board[i] = NoShape;
}

int TetrisBoard::dropDown(bool genNewPiece)
{
   mem->genNewPiece = genNewPiece;
   int dropHeight = 0;
   int newY = mem->curY;
   while (newY > 0) {
      if (!tryMove(mem->curPiece, mem->curX, newY - 1))
         break;
      --newY;
      ++dropHeight;
   }
   pieceDropped(dropHeight);

   return newY;
}

int TetrisBoard::dropDownNoNewPiece()
{
   return dropDown(false);
}

void TetrisBoard::savePimpl()
{
   memStack.push( new Pimpl(*mem) );
}

void TetrisBoard::restorePimpl()
{
   if(memStack.empty()) return;
   delete mem;
   mem = new Pimpl(*memStack.top());
   popPimpl();
}

void TetrisBoard::restorePimplNoPop()
{
   if(memStack.empty()) return;
   delete mem;
   mem = new Pimpl(*memStack.top());
}

void TetrisBoard::popPimpl()
{
   if(memStack.empty()) return;
   delete memStack.top();
   memStack.pop();
}

void TetrisBoard::oneLineDown()
{
   if (!tryMove(mem->curPiece, mem->curX, mem->curY - 1))
      pieceDropped(0);
}

void TetrisBoard::pieceDropped(int dropHeight)
{
   for (int i = 0; i < 4; ++i) {
      int x = mem->curX + mem->curPiece.x(i);
      int y = mem->curY - mem->curPiece.y(i);
      shapeAt(x, y) = mem->curPiece.shape();
   }

   ++mem->numPiecesDropped;
   if (mem->numPiecesDropped % 25 == 0) {
      ++mem->level;
   }

   mem->score += dropHeight + 7;
   removeFullLines();

   if (!mem->isWaitingAfterLine)
      newPiece();
}

void TetrisBoard::removeFullLines()
{
   int numFullLines = 0;

   for (int i = BoardHeight - 1; i >= 0; --i)
   {
      bool lineIsFull = true;

      for (int j = 0; j < BoardWidth; ++j)
      {
         if (shapeAt(j, i) == NoShape)
         {
            lineIsFull = false;
            break;
         }
      }

      if (lineIsFull)
      {
         ++numFullLines;
         for (int k = i; k < BoardHeight - 1; ++k)
         {
            for (int j = 0; j < BoardWidth; ++j)
               shapeAt(j, k) = shapeAt(j, k + 1);
         }
         for (int j = 0; j < BoardWidth; ++j)
            shapeAt(j, BoardHeight - 1) = NoShape;
      }
   }

   if (numFullLines > 0)
   {
      mem->numLinesRemoved += numFullLines;
      mem->score += 10 * numFullLines;
      mem->isWaitingAfterLine = true;
      mem->curPiece.setShape(NoShape);
   }
}

void TetrisBoard::newPiece()
{
   mem->curPiece = mem->nextPiece;
   mem->nextPiece = mem->nextNextPiece;

   if(mem->genNewPiece)
   {
      int shape = mRandom.rnd(1, 7);
      mem->nextNextPiece.setRandomShape(shape);
      ++mShapes.at(shape-1);
   }

   mem->curX = BoardWidth / 2 + 1;
   mem->curY = BoardHeight - 1 + mem->curPiece.minY();

   if (!tryMove(mem->curPiece, mem->curX, mem->curY))
   {
      mem->curPiece.setShape(NoShape);
      mem->isStarted = false;
   }
}

bool TetrisBoard::tryMove(const TetrisPiece &newPiece, int newX, int newY)
{
   for (int i = 0; i < 4; ++i)
   {
      int x = newX + newPiece.x(i);
      int y = newY - newPiece.y(i);
      if (x < 0 || x >= BoardWidth || y < 0 || y >= BoardHeight)
         return false;
      if (shapeAt(x, y) != NoShape)
         return false;
   }

   mem->curPiece = newPiece;
   mem->curX = newX;
   mem->curY = newY;
   return true;
}

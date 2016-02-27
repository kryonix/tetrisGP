#ifndef TETRISBOARD_H
#define TETRISBOARD_H

#include <QObject>

#include <vector>
#include <stack>

#include "random.h"

#include "tetrispiece.h"

class TetrisBoard
{
   friend class TetrisRenderer;
public:
   TetrisBoard();

   TetrisShape &shapeAt(int x, int y) { return mem->board[(y * BoardWidth) + x]; }

   enum { BoardWidth = 10, BoardHeight = 22, Fields = BoardWidth*BoardHeight };

   bool moveLeft();
   bool moveRight();
   bool rotateLeft();
   bool rotateRight();
   void oneLineDown();

   TetrisPiece getCurPiece();
   TetrisPiece getNextPiece();

   bool getStarted() const;

   int getLinesRemoved() const;

   int getScore() const;
   int getLevel() const;

   void timerEvent(QTimerEvent *event = 0);

   bool lost() const;

   int dropDown(bool genNewPiece = true);
   int dropDownNoNewPiece();

   void savePimpl();
   void restorePimpl();
   void restorePimplNoPop();
   void popPimpl();
   std::size_t pimplStackSize() { return memStack.size(); }

   bool showNextPieces;

   void start();
   void pause();

   std::vector<int> mShapes;

private:
   Random mRandom;


   void clearBoard();
   void pieceDropped(int dropHeight);
   void removeFullLines();
   void newPiece();
   bool tryMove(const TetrisPiece &newPiece, int newX, int newY);

   class Pimpl
   {
   public:
      bool isStarted;
      bool isPaused;
      bool isWaitingAfterLine;
      TetrisPiece nextNextPiece;
      TetrisPiece nextPiece;
      TetrisPiece curPiece;
      int curX;
      int curY;
      int numLinesRemoved;
      int numPiecesDropped;
      int score;
      int level;
      TetrisShape board[BoardWidth * BoardHeight];
      bool genNewPiece;
   };

   Pimpl* mem;
   std::stack< Pimpl* > memStack;
};

#endif // TETRISBOARD_H

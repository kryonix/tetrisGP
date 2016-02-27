#include "tetrisrenderer.h"
#include "tetrisboard.h"

TetrisRenderer::TetrisRenderer(QWidget *parent)
   : QFrame(parent)
{
   setFrameStyle(QFrame::Panel | QFrame::Sunken);
   setFocusPolicy(Qt::StrongFocus);
}

QSize TetrisRenderer::sizeHint() const
{
      return QSize(board.BoardWidth * 15 + frameWidth() * 2,
                   board.BoardHeight * 15 + frameWidth() * 2);
}

QSize TetrisRenderer::minimumSizeHint() const
{
      return QSize(board.BoardWidth * 5 + frameWidth() * 2,
                   board.BoardHeight * 5 + frameWidth() * 2);
}

int TetrisRenderer::squareWidth()
{
      return contentsRect().width() / board.BoardWidth;
}

int TetrisRenderer::squareHeight()
{
      return contentsRect().height() / board.BoardHeight;
}

void TetrisRenderer::setBoard(TetrisBoard tetrisBoard)
{
   board = tetrisBoard;
}

void TetrisRenderer::paintEvent(QPaintEvent *event)
{
   QFrame::paintEvent(event);

   QPainter painter(this);

   painter.setRenderHint(QPainter::Antialiasing);

   QRect rect = contentsRect();

   QLinearGradient gradient;
   gradient.setStart(0, 0);
   gradient.setFinalStop(rect.width(), rect.height());

   gradient.setColorAt(0.0, Qt::white);
   gradient.setColorAt(1.0, QColor(Qt::white).darker());

   painter.setBrush(gradient);

   painter.drawRect(rect);

   if (board.mem->isPaused) {
      painter.drawText(rect, Qt::AlignCenter, tr("Pause"));
      return;
   }

   int boardTop = rect.bottom() - board.BoardHeight*squareHeight();

   for (int i = 0; i < board.BoardHeight; ++i) {
      for (int j = 0; j < board.BoardWidth; ++j) {
         TetrisShape shape = board.shapeAt(j, board.BoardHeight - i - 1);
         if (shape != NoShape)
         {
            drawSquare(painter, rect.left() + j * squareWidth(),
                       boardTop + i * squareHeight(), shape);
         }
      }
   }

   if (board.mem->curPiece.shape() != NoShape) {
      for (int i = 0; i < 4; ++i) {
         int x = board.mem->curX + board.mem->curPiece.x(i);
         int y = board.mem->curY - board.mem->curPiece.y(i);
         drawSquare(painter, rect.left() + x * squareWidth(),
                    boardTop + (board.BoardHeight - y - 1) * squareHeight(),
                    board.mem->curPiece.shape());
      }
   }
}

void TetrisRenderer::keyPressEvent(QKeyEvent *event)
{
   if (!board.mem->isStarted || board.mem->isPaused || board.mem->curPiece.shape() == NoShape) {
      QFrame::keyPressEvent(event);
      return;
   }

   switch (event->key()) {
   case Qt::Key_Left:
      board.tryMove(board.mem->curPiece, board.mem->curX - 1, board.mem->curY);
      break;
   case Qt::Key_Right:
      board.tryMove(board.mem->curPiece, board.mem->curX + 1, board.mem->curY);
      break;
   case Qt::Key_Down:
      board.tryMove(board.mem->curPiece.rotatedRight(), board.mem->curX, board.mem->curY);
      break;
   case Qt::Key_Up:
      board.tryMove(board.mem->curPiece.rotatedLeft(), board.mem->curX, board.mem->curY);
      break;
   case Qt::Key_Space:
      board.dropDown();
      break;
   case Qt::Key_D:
      board.oneLineDown();
      break;
   default:
      QFrame::keyPressEvent(event);
   }
}

void TetrisRenderer::drawSquare(QPainter &painter, int x, int y, TetrisShape shape)
{
   QLinearGradient gradient;
   gradient.setStart(x, y);
   gradient.setFinalStop(x+squareWidth(), y+squareHeight());

   gradient.setColorAt(0.0, 0x2a86dc);
   gradient.setColorAt(1.0, QColor(0x2a86dc).darker());

   painter.setBrush(gradient);

   /*painter.fillRect(x, y, squareWidth(), squareHeight(),
                    0x2a86dc);*/
   painter.drawRect(x+4, y, squareWidth(), squareHeight());
   //painter.drawRoundedRect(x+4, y, squareWidth(), squareHeight(), 3, 3);
}

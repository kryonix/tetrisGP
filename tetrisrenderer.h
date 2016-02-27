#ifndef TETRISRENDERER_H
#define TETRISRENDERER_H

#include <QFrame>
#include <QKeyEvent>

#include <QPainter>
#include <QLinearGradient>

#include "tetrispiece.h"

#include "tetrisboard.h"

class TetrisRenderer : public QFrame
{
   Q_OBJECT
private:
   void drawSquare(QPainter &painter, int x, int y, TetrisShape shape);
   int squareWidth();
   int squareHeight();

public:
   explicit TetrisRenderer(QWidget *parent = 0);

   TetrisBoard board;

   QSize sizeHint() const;
   QSize minimumSizeHint() const;

   void setBoard(TetrisBoard tetrisBoard);
signals:
   
public slots:
   void paintEvent(QPaintEvent * event);
   void keyPressEvent(QKeyEvent * event);
};

#endif // TETRISRENDERER_H

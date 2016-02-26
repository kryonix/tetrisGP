#ifndef TETRISWINDOW_H
#define TETRISWINDOW_H

#include <QFrame>
#include <QMainWindow>

#include <QTimer>

#include <QDebug>

#include <QSharedPointer>

#include "control.h"

#include "tetrisfeatures.h"

#include "QPCTimer.h"

#include <windows.h>

#include "tetriski.h"

#define BENCH 0

class QLCDNumber;
class QLabel;
class QPushButton;

#include "tetrisboard.h"

namespace Ui
{
class MainWindow;
}

class TetrisWindow : public QMainWindow
{
   Q_OBJECT
public:
   explicit TetrisWindow(QWidget *parent = 0);
   ~TetrisWindow();

private:
   Ui::MainWindow *ui;

   Control mControl;

   QTimer timer;

   TetrisKi mKi;
private slots:
   void TimerEvent();

   void setValues();

   void setScore(int arg);
   void setLinesRemoved(int arg);
   void setLevel(int arg);

   void setShapes();
   void setWeights();
   void on_horizontalSlider_valueChanged(int value);
   void on_checkBoxNoOutput_clicked();

   void setWeights(std::vector<double> weights);

   void individuenFinished(int arg);
};

#endif // TETRISWINDOW_H

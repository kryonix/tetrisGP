#include <QtGui>

#include "tetrisboard.h"
#include "tetriswindow.h"

#include "ui_TetrisWindow.h"

TetrisWindow::TetrisWindow(QWidget *parent) :
   QMainWindow(parent),
   ui(new Ui::MainWindow)
{
   ui->setupUi(this);

   ui->board->setFrameStyle(QFrame::Box | QFrame::Raised);

   //connect(mBoard.data(), SIGNAL(valuesUpdate(int,int,int)), this, SLOT(setValues(int,int,int)));

   setWindowTitle(tr("Tetris"));

   qRegisterMetaType< std::vector<double> >("std::vector<double>");

   connect(&mControl, SIGNAL(bestWeights(std::vector<double>)), this, SLOT(setWeights(std::vector<double>)));

   connect(&mControl, SIGNAL(individuenFinished(int)), this, SLOT(individuenFinished(int)));

   mControl.setGenomSize(12);
   mControl.begin();

   //mKi.setWeights(mControl.getWeights());


//   std::vector<double> weights;
//   weights.assign(12, 0);

   //3.24597 8.78897 8.36048 4.30189 3.13448 7.89289 5.31735 9.48082 7.96379 4.45269

//Generation:  39  Individuen:  103 	 Beste Fitness:  111212 	 Schlechteste Fitness:  28
//"1.77889 8.99933 1.45133 8.4268 6.0539 7.70892 3.19771 5.89378 9.38269 5.28904 0.866322 "
//Generation:  12  Individuen:  302 	 Beste Fitness:  94184 	 Schlechteste Fitness:  48
//"2.6661 9.50805 2.05836 7.32433 4.29216 6.93806 3.09512 6.27277 8.73402 9.56727 0.706733 1.49069 "

//Generation:  35  Individuen:  103 	 Beste Fitness:  174272 	 Schlechteste Fitness:  0
//"1.39979 5.90664 0.835818 12.6618 3.46759 3.43536 0.927099 2.17969 1.90578 3.88131 2.36073 0.757554 "

//Generation:  65  Individuen:  13 	 Beste Fitness:  468699 	 Schlechteste Fitness:  0
//"2.11016 44.7341 1.03651 2.74468 2.57603 8.11868 2.13356 9.46372 2.20143 2.81046 6.16237 0.939097 "

   /*
   weights.at(0) = 0;
   weights.at(1) = 7.899265427351652;
   weights.at(2) = 0;
   weights.at(3) = 9.348695305445199;
   weights.at(4) = 0;
   weights.at(5) = 0;
   weights.at(6) = 0;
   weights.at(7) = 3.2178882868487753;
   weights.at(8) = 0;
   weights.at(9) = 3.4181268101392694;
   weights.at(10) = 3.3855972247263626;
   weights.at(11) = 4.500158825082766;
*/
/*
   mValues[0] = holes(board);
   mValues[1] = blockades(board);
   mValues[2] = columnSpace(board);
   mValues[3] = touchFloor(board);
   mValues[4] = touchWall(board);
   mValues[5] = touchPiece(board);
   mValues[6] = rowSpace(board);
   mValues[7] = blocks(board);
   mValues[8] = lines;
   mValues[9] = well(board);
   mValues[10] = landingHeight;
   */

//   setWeights(weights);

   connect(&timer, SIGNAL(timeout()), this, SLOT(TimerEvent()));

   //timer.start(50);
   //ui->board->start();
   ui->board->board.start();
}

TetrisWindow::~TetrisWindow()
{
   delete ui;
}

void TetrisWindow::TimerEvent()
{
   for(int a=0; a < ui->horizontalSlider->value(); a++)
   {
      if(ui->board->board.lost())
      {
         ui->board->repaint();
         setShapes();

//         mControl.next(/*board->getScore() +*/ ui->board->getLinesRemoved());

//         mKi.setWeights(mControl.getWeights());
         //qDebug() << "Individuum " << mControl.getIndividuumId() << " / " << mControl.getPopulationSize() << " " << board->getLinesRemoved();

         setWeights();

//         ui->labelIndividuum->setText(QString::number(mControl.getIndividuumId()+1));

         ui->board->board.start();
      }

#if 0
      if(!ui->checkBoxDrop->isChecked())
      {
         for(int i = 0; i < ui->board->board.BoardHeight-maxHeight(&ui->board->board)-5; i++)
         {
            ui->board->board.oneLineDown();
         }
      }
#endif

#if BENCH == 1
      QPCTimer perfTimer;
      perfTimer.start();
#endif

      for(int i = 0; i < ui->board->board.BoardHeight-maxHeight(&ui->board->board)-5; i++)
      {
         ui->board->board.oneLineDown();
      }

      std::pair<int, int> zug = mKi.twoPiece(&ui->board->board);//mKi.onePiece(&ui->board->board);

#if BENCH == 1
      qDebug() << perfTimer.elapsedMs() << "ms";
#endif

      play(&ui->board->board, zug.first, zug.second);

      ui->board->board.oneLineDown();
      if(ui->checkBoxDrop->isChecked()) ui->board->board.dropDown();

      ui->board->board.timerEvent();

      if((!ui->checkBoxFPS->isChecked() || a%100 == 0) && !ui->checkBoxNoOutput->isChecked())
      {
         //ui->board->showNextPieces = false;
         ui->board->repaint();
         setShapes();

         setValues();
      }
   }
}

void TetrisWindow::setValues()
{
   setScore(ui->board->board.getScore());
   setLevel(ui->board->board.getLevel());
   setLinesRemoved(ui->board->board.getLinesRemoved());

#if 1
   //Output the Tetrisboard feature values on GUI
   std::vector<double> mValues;
   mValues.resize(12);

   std::bitset<TetrisBoard::BoardHeight*TetrisBoard::BoardWidth> boardSet;

   for(int x = 0; x < TetrisBoard::BoardWidth; ++x)
   {
       for(int y = 0; y < TetrisBoard::BoardHeight; ++y)
       {
           boardSet[(y*TetrisBoard::BoardWidth)+x] = ui->board->board.shapeAt(x,y) == NoShape ? true:false;
       }
   }

   std::vector<double> weights = mKi.getWeights();

   mValues[0] = weightedHeight(&boardSet, weights.at(0));
   mValues[1] = holes(&boardSet);// * weights.at(1);
   mValues[2] = blockades(&boardSet) * weights.at(2);
   mValues[3] = columnSpace(&boardSet) * weights.at(3);
   mValues[4] = touchFloor(&boardSet) * weights.at(4);
   mValues[5] = touchWall(&boardSet) * weights.at(5);
   mValues[6] = touchPiece(&boardSet) * weights.at(6);
   mValues[7] = rowSpace(&boardSet) * weights.at(7);
   mValues[8] = blocks(&boardSet) * weights.at(8);
   mValues[9] = 0;
   mValues[10] = well(&boardSet);// * weights.at(10);
   mValues[11] = 0;

   for(int i = 0; i < mValues.size(); i++)
   {
      ui->tableWidget->item(i, 1)->setText(QString::number(mValues.at(i)));
   }
#endif
}

void TetrisWindow::setScore(int arg)
{
   ui->labelScore->setText(QString::number(arg));
}

void TetrisWindow::setLevel(int arg)
{
   ui->labelLevel->setText(QString::number(arg));
}

void TetrisWindow::setShapes()
{
   int summe = 0;

   for(int i = 0; i < 7; i++)
   {
      summe += ui->board->board.mShapes[i];
   }

   ui->labelZShape->setText(QString::number(ui->board->board.mShapes[0]));
   ui->labelSShape->setText(QString::number(ui->board->board.mShapes[1]));
   ui->labelLineShape->setText(QString::number(ui->board->board.mShapes[2]));
   ui->labelTShape->setText(QString::number(ui->board->board.mShapes[3]));
   ui->labelSquareShape->setText(QString::number(ui->board->board.mShapes[4]));
   ui->labelLShape->setText(QString::number(ui->board->board.mShapes[5]));
   ui->labelMLShape->setText(QString::number(ui->board->board.mShapes[6]));

   ui->labelSummeShapes->setText(QString::number(summe));
}

void TetrisWindow::setWeights()
{
   std::vector<double> weights = mKi.getWeights();
   for(int i = 0; i < weights.size(); i++)
   {
      ui->tableWidget->item(i, 0)->setText(QString::number(weights.at(i)));
   }
}

void TetrisWindow::setLinesRemoved(int arg)
{
   ui->LabelLinesRemoved->setText(QString::number(arg));
}

void TetrisWindow::on_horizontalSlider_valueChanged(int value)
{
   ui->labelSpeed->setText(QString::number(value));
}

void TetrisWindow::on_checkBoxNoOutput_clicked()
{
}

void TetrisWindow::setWeights(std::vector<double> weights)
{
   mKi.setWeights(weights);
   setWeights();
   timer.start(50);
}

void TetrisWindow::individuenFinished(int arg)
{
   ui->labelIndividuum->setText(QString::number(arg));
}

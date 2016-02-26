#ifndef CONTROL_H
#define CONTROL_H

#include <QObject>
#include <QThread>
#include <QThreadPool>

#include <QWaitCondition>
#include <QMutex>
#include <QMutexLocker>

#include <QVector>
#include <QDebug>

#include <QTime>

#include <algorithm>
#include <tr1/memory>

#include <vector>

#include "random.h"

#include "individuum.h"

#include "tetrisrunnable.h"

#include "tetrisevaluation.h"

#include <QtConcurrent/QtConcurrentRun>
#include <QtConcurrent/QtConcurrentMap>
#include <QFutureWatcher>
#include <QFuture>

#include <boost/bind.hpp>

class TetrisBoard;

class Control : public QThread
{
   Q_OBJECT
private:
   std::vector< Individuum > mPopulation;
   std::size_t mPopulationSize;
   int mBestFitness;
   int mGeneration;
   Random mRandom;

   std::size_t mPopulationInitSize;
   double mMutationsrate;
   double mCrossoverrate;

   void generateNewPopulation();
   void recombine();

   std::size_t mInd;

   int genomSize;
   bool abort;
   bool newGeneration;
   QMutex mutex;
   QWaitCondition waitCondition;

   QThreadPool mThreadPool;

   int mIndividuenFinished;

   void evaluateInd(Individuum& individuum);
public:
   Control(QObject* parent = 0);
   ~Control();

   /*!
     Generiert eine neue Population und Startet den Thread
   */
   void begin();

   void evolve();

   void next(int fitness);

   std::vector<double> getWeights() { return mPopulation.at(mInd).weights; }

   std::size_t getIndividuumId() { return mInd; }
   std::size_t getPopulationSize() { return mPopulation.size(); }
protected:
   void run();

signals:
   void bestWeights(std::vector<double> weights);
   void individuenFinished(int individuen);
public slots:
   /*!
     \param populationSize Groesse der Population
   */
   void setPopulationSize(std::size_t populationSize);
   /*!
     \param populationInitSize Groesse der Initialisierungspopulation
   */
   void setPopulationInitSize(std::size_t populationInitSize);
   /*!
     \param mutationrate Prozentualer anteil an Mutation pro Generation [0;100]
   */
   void setMutationRate(double mutationrate);
   /*!
     \param crossoverRate Erfolgsrate der Rekombination [0;100]
     \warning Bei einer sehr kleinen Erfolgsrate wird die Effektivitaet des Algorithmus stark sinken
   */
   void setCrossoverRate(double crossoverRate);

   void setGenomSize(int size);

private slots:
   void increaseIndividuenFinished();
};

#endif // CONTROL_H

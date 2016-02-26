#include "tetrisboard.h"

#include "control.h"

Control::Control(QObject *parent)
   : QThread(parent),
     mPopulationSize(500),
     mBestFitness(-1),
     mGeneration(0),
     mPopulationInitSize(10000),
     mMutationsrate(1),
     mCrossoverrate(90),
     mInd(0),
     abort(false),
     newGeneration(false),
     mIndividuenFinished(0)
{
}

Control::~Control()
{
   mutex.lock();
   abort = true;
   waitCondition.wakeOne();
   mutex.unlock();

   wait();
}

void Control::run()
{
   forever
   {
      mutex.lock();
      if(abort)
      {
         return;
      }
      mutex.unlock();

      mutex.lock();
      if(newGeneration)
      {
         generateNewPopulation();
         newGeneration = false;
      }
      mutex.unlock();

      mutex.lock();
      mIndividuenFinished = 0;
      emit individuenFinished(0);

/*
      for(std::size_t i = 0; i < mPopulation.size(); ++i)
      {
         TetrisRunnable* runnable = new TetrisRunnable(mPopulation.at(i));
         mThreadPool.start(runnable);
      }

      mutex.unlock();

      mThreadPool.waitForDone();
*/

      /*
      TetrisEvaluation evalu(&mPopulation.at(0));

      QFuture<void> future = QtConcurrent::run(&evalu, &TetrisEvaluation::eval);

      future.waitForFinished();
*/

      QFuture<void> nix = QtConcurrent::map(mPopulation, boost::bind(&Control::evaluateInd, this, _1));

      nix.waitForFinished();

      mutex.unlock();

      recombine();
   }
}

void Control::evaluateInd(Individuum& individuum)
{
   if(individuum.fitness != 0)
   {
      increaseIndividuenFinished();
      return;
   }

   //qDebug() << "Start evaluation";
   TetrisEvaluation evaluate(&individuum);
   individuum.fitness = evaluate.evaluate();
   //qDebug() << "End evaluation " << individuum.fitness;
   increaseIndividuenFinished();
   return;
}

void Control::begin()
{
   qDebug() << "<font color=blue>Start des genetischen Algorithmus</font>";

   QMutexLocker locker(&mutex);

   newGeneration = true;

   if(!isRunning())
   {
      start(LowPriority);
   }
   else
   {
      waitCondition.wakeOne();
   }
}

void Control::evolve()
{
   recombine();
}

void Control::next(int fitness)
{
   mPopulation[mInd].fitness = fitness;
   mInd++;

   if(mInd >= mPopulation.size())
   {
      mInd = 0;
      evolve();
   }
}

void Control::recombine()
{
   QMutexLocker locker(&mutex);
   if(mPopulation.size() < 1) return;

   std::sort(mPopulation.begin(), mPopulation.end());

   emit bestWeights(mPopulation.at(0).weights);

   QString bestgenom;

   for(int i = 0; i < genomSize; i++)
   {
      bestgenom += QString::number(mPopulation.front().weights.at(i)) + " ";
   }

   qDebug()  << QTime::currentTime().toString() << ": "
             << "Generation: " << ++mGeneration
             << " Individuen: " << mPopulation.size()
             << "\t Beste Fitness: " << mPopulation.front().fitness
             << "\t Schlechteste Fitness: " << mPopulation.back().fitness
             << "";
   qDebug() << bestgenom;

   std::vector< Individuum > newPop;

   //if( mGeneration != 1 )
   {
      /*
      while( mPopulation.size() > mPopulationSize/2)
      {
         mPopulation.pop_back();
      }*/
   }
/*
   for(int i = 0, added = 0; i < mPopulation.size(); i++) //Elitismus
   {
      if( mPopulation.at(i).fitness <= mPopulation.front().fitness*0.8 )
      {
         newPop.push_back( Individuum(mPopulation.at(i)) );

         if(++added == mPopulation.size()*0.2)
         {
            break;
         }
      }
   }
*/

   for(int i = 0; i < 1; i++)
   {
      newPop.push_back( Individuum(mPopulation[i]) );
      newPop.back().fitness = mPopulation[i].fitness;
      mPopulation[i].mutate();
      newPop.push_back( Individuum(mPopulation[i]) );
   }

   for(int i = 0; i < mPopulation.size(); i++)
   {
      if(mRandom.rnd(0, 100) < mMutationsrate)
      {
         mPopulation[i].mutate();
      }
   }

   std::size_t popBegin = newPop.size();

   while(newPop.size()-popBegin < mPopulationSize)
   {
      int intervall = 0;
      //if( mRandom.rnd(0, 10) == 0 )
      {
         intervall = mPopulation.size()-1;
      }
      /*else
      {
         intervall = (mPopulation.size()-1)/2;
      }*/

      int a = 0;
      int b = 0;

      a = mRandom.rnd(0, intervall);

      if( mRandom.rnd(0, 100) <= mCrossoverrate)
      {
         if(intervall != 0)
         {
            do
            {
               b = mRandom.rnd(0, intervall);
            } while( a == b );
         }

         newPop.push_back(Individuum(mPopulation.at(a), mPopulation.at(b)));
         //newPop.push_back(Individuum(mPopulation.at(b), mPopulation.at(a)));
      }
      else
      {
         newPop.push_back( Individuum(mPopulation.at(a)) );
      }
   }

   mPopulation.clear();
   mPopulation = newPop;

   mPopulation.push_back( Individuum(genomSize) );
   mPopulation.push_back( Individuum(genomSize) );
/*
   if( mGeneration%25 == 0 )
   {
      for(unsigned int i = 0; i < mPopulationSize; i++)
      {
         mPopulation.push_back( Individuum(genomSize) );
      }
   }*/
}

void Control::setPopulationSize(std::size_t populationSize)
{
   mPopulationSize = populationSize;
}

void Control::setPopulationInitSize(std::size_t populationInitSize)
{
   mPopulationInitSize = populationInitSize;
}

void Control::setMutationRate(double mutationrate)
{
   mMutationsrate = mutationrate;
}

void Control::setCrossoverRate(double crossoverRate)
{
   mCrossoverrate = crossoverRate;
}

void Control::setGenomSize(int size)
{
   genomSize = size;
}

void Control::increaseIndividuenFinished()
{
   ++mIndividuenFinished;
   emit individuenFinished(mIndividuenFinished);
}

void Control::generateNewPopulation()
{
   mBestFitness = 0;
   mGeneration = 0;

   mPopulation.clear();

   for(unsigned int i=0; i < mPopulationInitSize; i++)
   {
      mPopulation.push_back( Individuum(genomSize) );
   }
}

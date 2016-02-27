#ifndef RANDOM_H
#define RANDOM_H

#include <QtGlobal>
#include <cstdlib>

class Random
{
private:

public:
    Random()
    {
       //qsrand(0);
    }

    Random(unsigned int seed)
    {
        qsrand(seed);
    }

    void setSeed(int seed)
    {
        qsrand(seed);
    }

    int rnd(int lowerbounds, int upperbounds)
    {
        return lowerbounds + qrand()%(upperbounds-lowerbounds+1);
    }

    double rnd(double lowerbounds, double upperbounds)
    {
        double rn = qrand()/((double)RAND_MAX+1);

        return rn*(upperbounds-lowerbounds)+lowerbounds;
    }
};

#if 0
#include "MersenneTwister.h"
#include <ctime>

class Random
{
private:
   MTRand random;

public:
   Random()
   {
      random.seed( time(0) + random.randInt() );
   }

   Random(int seed)
   {
      random.seed(seed);
   }

   void setSeed(int seed)
   {
      random.seed(seed);
   }

   int rnd(int lowerbounds, int upperbounds)
   {
      return lowerbounds + random.randInt(upperbounds-lowerbounds);
   }

   double rnd(double lowerbounds, double upperbounds)
   {
      return lowerbounds + random.rand(upperbounds-lowerbounds);
   }
};
#endif

#endif // RANDOM_H

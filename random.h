#ifndef RANDOM_H
#define RANDOM_H

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

#endif // RANDOM_H

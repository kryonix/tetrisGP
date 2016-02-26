#ifndef INDIVIDUUM_H
#define INDIVIDUUM_H

#include <vector>
#include "random.h"

class Individuum
{
private:
   Random mRandom;
public:
   Individuum(int weightsSize);

   Individuum(const Individuum& a, const Individuum& b);

   void mutate();

   std::vector<double> weights;
   int fitness;

   bool operator<(const Individuum& other) const
   {
      return fitness > other.fitness;
   }
};
#endif // INDIVIDUUM_H

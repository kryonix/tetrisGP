#ifndef INDIVIDUUM_H
#define INDIVIDUUM_H

#include <vector>
#include "random.h"
#include <cmath>

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

   bool operator==(const Individuum& other) const
   {
      for(int i = 0; i < weights.size(); i++)
      {
         if(std::fabs(weights.at(i)-other.weights.at(i)) > 0.0000001)
         {
            return false;
         }
      }

      return true;
   }
};
#endif // INDIVIDUUM_H

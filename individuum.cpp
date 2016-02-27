#include "individuum.h"

Individuum::Individuum(int weightsSize)
{
   for(int i = 0; i < weightsSize; i++)
   {
      weights.push_back(mRandom.rnd(-1.0, 1.0));
   }
   fitness = 0;
}

Individuum::Individuum(const Individuum &a, const Individuum &b)
{
   if(a.weights.size() == b.weights.size())
   {
      int pos = mRandom.rnd(1, a.weights.size()-2);

      weights = a.weights;

      for(int i = pos; i < a.weights.size(); i++)
      {
         weights.at(i) = b.weights.at(i);
      }
   }

   if(a == *this)
   {
      fitness = a.fitness;
   }
   else if(b == *this)
   {
      fitness = b.fitness;
   }
   else
   {
      fitness = 0;
   }
}

void Individuum::mutate()
{
   double mul = 0;
   int op = mRandom.rnd(0, 4);

   if(op == 0)
   {
      mul = 1.5;
      weights.at(mRandom.rnd(0, weights.size()-1)) *= mul;
      return;
   }
   else if(op == 1)
   {
      mul = 0.5;
      weights.at(mRandom.rnd(0, weights.size()-1)) *= mul;
      return;
   }
   else if(op == 2)
   {
      weights.at(mRandom.rnd(0, weights.size()-1)) = mRandom.rnd(-10.0, 10.0);
      return;
   }
   else if(op == 3)
   {
      std::swap(weights.at(mRandom.rnd(0, weights.size()-1)), weights.at(mRandom.rnd(0, weights.size()-1)));
      return;
   }
   else if(op == 4)
   {
       weights.at(mRandom.rnd(0, weights.size()-1)) *= -2;
   }
}

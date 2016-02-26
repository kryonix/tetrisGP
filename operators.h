#ifndef OPERATORS_H
#define OPERATORS_H

namespace Operators
{
enum Genetic_Programming
{
   ifthen4 = 0,      //4 Childs
   add,              //2
   sub,              //2
   mul,              //2

   Board,            //2 (x, y)

   MoveLeft,         //1
   MoveRight,        //1
   RotateLeft,       //1
   RotateRight,      //1
   DoNothing,        //1

   Piece,            //0
   constant          //0
};
}

#endif // OPERATORS_H

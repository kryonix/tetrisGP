#include "tree.h"

#include "tetrisboard.h"

bool operator<(Tree const& lhs, Tree const& rhs)
{
   return lhs.fitness < rhs.fitness;
}

Tree::Tree(unsigned int depth, bool halframping)
   : fitness(0)
{
   root = new Node;
   root->setOperator( mRandom.rnd(0, Operators::DoNothing) );
   if( depth != 0)
   {
      insert_rnd(depth, halframping);
   }
}

Tree::Tree(const Tree &otherA, const Tree &otherB, double mutationrate)
   : fitness(0)
{
   if(otherA.is_empty())
   {
      root = 0;
   }
   else
   {
      root = otherA.root->clone(otherA.root);

      if(!otherB.is_empty())
      {
         bool leave = false;

         if( mRandom.rnd(0, 10) == 0 ) leave = true;
         Node* rand_A = select_random_node(root, leave);

         leave = false;
         if( mRandom.rnd(0, 10)  == 0 ) leave = true;

         Node* rand_B = select_random_node(otherB.root, leave);

         Node* parent = rand_A->mNodeParent;

         for(std::size_t i = 0; i < parent->mChilds.size(); i++)
         {
            if( parent->mChilds.at(i) == rand_A )
            {
               delete parent->mChilds.at(i);
               parent->mChilds.at(i) = 0;
               parent->mChilds.at(i) = Node::clone(rand_B);
               parent->mChilds.at(i)->mNodeParent = parent;
            }
         }
      }

      if(mRandom.rnd(0, 100) <= mutationrate)
      {
         mutate();
      }

      if(mRandom.rnd(0, 100) <= mutationrate)
      {
         grow();
      }
   }
}

Tree::Tree(const std::string algorithm)
   : fitness(0)
{
   root = new Node(algorithm);
}

Tree::Tree(const Tree& otherA)
   : root(0),
     fitness(0)
{
   if(otherA.is_empty())
   {
      root = 0;
   }
   else
   {
      root = otherA.root->clone(otherA.root);
   }
}

void Tree::mutate()
{
   Node* rand_A = select_random_node(root);

   if(mRandom.rnd(0, 100) <= 50)
   {
      if(rand_A->mChilds.size() >= 2)
      {
         int first = mRandom.rnd(0, rand_A->mChilds.size()-1);
         int second = 0;

         if(rand_A->mChilds.size() == 2)
         {
            first = 0;
            second = 1;
         }
         else
         {
            do
            {
               second = mRandom.rnd(0, rand_A->mChilds.size()-1);
            } while(first == second);
         }

         Node* one = rand_A->mChilds.at(first);

         rand_A->mChilds.at(first) = rand_A->mChilds.at(second);
         rand_A->mChilds.at(second) = one;
      }
   }
   else
   {

      if(rand_A->getOperator() != Operators::ifthen4)
      {
         if(   rand_A->getOperator() == Operators::add ||
               rand_A->getOperator() == Operators::sub ||
               rand_A->getOperator() == Operators::mul )
         {
            rand_A->setOperator(mRandom.rnd(Operators::add, Operators::mul));
         }
         else
         {
            rand_A->setOperator(mRandom.rnd(Operators::MoveLeft, Operators::constant), mRandom.rnd(0, 100));
         }
      }

   }
}

void Tree::grow()
{
   Node* rand_A = select_random_node(root);

   if(rand_A->mChilds.size() >= 1)
   {
      int child = mRandom.rnd(0, rand_A->mChilds.size()-1);

      delete rand_A->mChilds.at(child);

      rand_A->mChilds.at(child) = insert_random(rand_A, mRandom.rnd(0, 2));
   }
}

Tree::~Tree()
{
   delete root;
   root = 0;
}

int Tree::height()
{
   return get_height(root);
}

int Tree::get_height(const Node* node)
{
   if( !node ) return 0;

   int heightMax = 0;

   for(unsigned int i = 0; i < node->mChilds.size(); i++)
   {
      int height = get_height(node->mChilds.at(i));
      if(height > heightMax)
      {
         heightMax = height;
      }
   }

   return heightMax + 1;
}

void Tree::destroy_node(Node* node)
{
   if(node)
   {
      delete node;
      node = 0;
   }
}

Node* Tree::get_random_node(bool number, double value)
{
   int count = 0;
   if(!number)
   {
      count = 10;
      value = 0;
   }
   else
   {
      count = 13;
   }

   Node* new_node = new Node;

   new_node->setOperator( mRandom.rnd(0, count), value );

   return new_node;
}

Node* Tree::insert_random(Node* node, unsigned int max_depth, bool halframping)
{
   if( max_depth == 0 )
   {
      return new Node(node, mRandom.rnd(Operators::Piece, Operators::constant), mRandom.rnd(0, 10));
   }
   else
   {
      Node* new_node;
      new_node = get_random_node(halframping, mRandom.rnd(0,100));
      new_node->mNodeParent = node;

      int childs = 0;

      if(new_node->getOperator() == Operators::ifthen4)
      {
         childs = 4;
      }

      if(new_node->getOperator() >= Operators::add && new_node->getOperator() <= Operators::mul)
      {
         childs = 2;
      }

      if(new_node->getOperator() >= Operators::MoveLeft && new_node->getOperator() <= Operators::DoNothing)
      {
         childs = 1;
      }

      for(int i = 0; i < childs; i++)
      {
         new_node->insertChild(insert_random(new_node, max_depth-1, halframping));
      }

      return new_node;
   }
}

Node* Tree::random_node()
{
   return select_random_node(root);
}

Node* Tree::select_random_node(Node* node, bool leave)
{
   if(node)
   {
      for(int i = 0; i < height()+1; i++)
      {
         if(node)
         {
            if(node->isLeave() && leave)
            {
               return node;
            }

            if(node->mChilds.size() != 0)
            {
               node = node->mChilds.at(mRandom.rnd(0, node->mChilds.size()-1));
            }
            else
            {
               if(node->mNodeParent != 0)
               {
                  node = node->mNodeParent;
               }
            }
         }
      }

      if(node == root)
      {
         return select_random_node(node, leave);
      }

      return node;
   }
   return 0;
}

void Tree::insert_rnd(unsigned int max_depth, bool halframping)
{
   if(root)
   {
      if(root->mChilds.size() == 0)
      {
         int childs = 2;

         if( root->getOperator() == Operators::ifthen4)
         {
            childs = 4;
         }

         if(root->getOperator() >= Operators::add && root->getOperator() <= Operators::mul)
         {
            childs = 2;
         }

         if(root->getOperator() >= Operators::MoveLeft && root->getOperator() <= Operators::DoNothing)
         {
            childs = 1;
         }

         for(int i = 0; i < childs; i++)
         {
            root->insertChild(insert_random(root, max_depth-1, halframping));
         }
      }
   }
}

bool Tree::eval(TetrisBoard* board)
{
   if(root)
   {
      return root->eval(board);
   }
   else
   {
      return 0;
   }
}

std::string Tree::dump()
{
   if(root)
   {
      return root->dump();
   }
   else
   {
      return "";
   }
}

std::string Tree::dumpXml()
{
   if(root)
   {
      return root->dumpXml();
   }
   else
   {
      return "";
   }
}

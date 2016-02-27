#ifndef TREE_H
#define TREE_H

#include <tr1/memory>

#include <string>
#include <sstream>

#include "random.h"

#include "node.h"

class TetrisBoard;

class Tree
{
private:
   Random mRandom;
   Node* root;

   static void destroy_node(Node* node);
   Node* insert_random(Node* node, unsigned int max_depth, bool halframping = false);

   Node* get_random_node(bool number = false, double value = 0);

   static int get_height(const Node* Node);

   Node* select_random_node(Node* node, bool leave = false);

//   static Node* get_next_node(Node* now);
//   static Node* get_prev_node(Node* now);

public:
   /*!
     Initialisierung des Baumes mit direktem einfuegen von Elementen
     \param depth maximale Tiefe des Baumes
     \param halframping Halframpingmethode
   */
   Tree(unsigned int depth = 0, bool halframping = false);
   /*!
     copy-constructor
   */
   Tree(const Tree& otherA);
   /*!
     copy-constructor und Rekombination mit Mutation
   */
   Tree(const Tree &otherA, const Tree &otherB, double mutationrate);
   /*!
     Baum aus XML-Export aufbauen
   */
   Tree(const std::string algorithm);
   ~Tree();

   /*!
     \return Gibt die aktuelle Tiefe des Baumes zurueck
   */
   int height();

   /*!
     \return Gibt ein Zufaelliges Element des Baumes zurueck
   */
   Node* random_node();

   /*!
     \param max_depth Maximale tiefe des Baumes
     \param halframping Halframpingmethode
   */
   void insert_rnd(unsigned int max_depth, bool halframping = false);

   /*!
     \param regler Pointer auf einen Regler
     \todo Ersetzen mit Smartpointer
     \return Ergebnis der Auswertung des Baumes
   */
   bool eval(TetrisBoard* board);

   /*!
     \return C++-Code des Baumes
   */
   std::string dump();

   /*!
     \return XML-String des Baumes
   */
   std::string dumpXml();

   /*!
     Fitnesswert des Baumes
   */
   double fitness;

   /*!
     Mutation des Operators oder ersetzen eines Astes
   */
   void mutate();

   /*!
     Auswahl eines Blattes und ersetzten durch einen Zufaelligen
   */
   void grow();

   /*!
     \return true wenn wenigstens eine Wurzel vorhanden ist
   */
   bool is_empty() const
   {
      return !root;
   }
/*
   bool operator<(const Tree& b) const
   {
      if( fitness < b.fitness )
      {
         return true;
      }
      return false;
   }
*/
   static bool compareFitness(std::tr1::shared_ptr<Tree> const& lhs, std::tr1::shared_ptr<Tree> const& rhs)
   { return lhs->fitness > rhs->fitness; }
   //static bool compareFitness(const Tree* a, const Tree* b) { return a->fitness < b->fitness; }
};

bool operator<(Tree const& lhs, Tree const& rhs);

#endif // TREE_H

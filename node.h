#ifndef NODE_H
#define NODE_H

#include <string>
#include <sstream>

#include <vector>

#include <QDomElement>
#include <QDomDocument>

#include <QXmlParseException>

#include <QDebug>

#include "operators.h"

class TetrisBoard;

class Node
{
private:
   unsigned int mOperator;
   double mValue;

   void getXml(QDomElement& element, QDomDocument& document);

public:
   std::vector<Node*> mChilds;
   Node* mNodeParent;

   /*!
     \param parent Moeglicher Pointer auf ein Eltern-Node
     \param pOperator Element aus Operators
     \param value Wert des Operators::constant
   */
   Node(Node* parent = 0, unsigned int pOperator = 0, double value = 0);

   /*!
     Erstellt einen Baum aus einem XML-Baum Export
     \param xmlTree einen XML-Baum importieren
   */
   Node(std::string xmlTree);
   /*!
     Copy-Konstruktor
     \param other ein Node dass Kopiert werden soll.
   */
   Node(const Node& other);
   ~Node();

   /*!
     Setzt das Parent von other auf this
     und fuegt es dem Childs-Vector hinzu
   */
   void insertChild(Node* child);

   /*!
     Dupliziert das uebergebene Node inklusive aller childs
     \param other ein Node dass Kopiert werden soll.
     \return das duplikat von other
   */
   static Node* clone(const Node* other);

   /*!
     Berechnet ein Tastverhaeltnis mithilfe des Reglers
     \param regler ein Pointer auf einen Regler
     \return ein Tastverhaeltnis
   */
   int eval(TetrisBoard* board);

   /*!
     Export des Nodes als c++ Code
     \return c++ Code des Nodes inklusive der Childs
   */
   std::string dump();

   /*!
     Export des Nodes als XML-String
     \return XML-String des Nodes inklusive der Childs
   */
   std::string dumpXml();

   /*!
     Definieren eines Operators
     \param pOperator ein Element aus Operators
     \param value Wert bei Operators::constant
   */
   void setOperator(unsigned int pOperator, double value = 0);
   /*!
     \return den aktuellen Operator
   */
   unsigned int getOperator() const;

   /*!
     \return den Wert bei Operators::constant, sonst 0
   */
   double getValue() const;

   /*!
     \return true wenn Operators::constant sonst false
   */
   bool isNumber() const;

   /*!
     \return true wenn keine Childs vorhanden sind, aber ein Parent gesetzt
   */
   bool isLeave();
};

#endif // NODE_H

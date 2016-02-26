#include "node.h"
#include "tetrisboard.h"

Node::Node(Node *parent, unsigned int pOperator, double value)
   : mNodeParent(parent)
{
   mOperator = pOperator;
   if(isNumber())
   {
      mValue = value;
   }
   else
   {
      mValue = 0;
   }
}

Node::Node(std::string xmlTree)
   : mOperator(0),
     mValue(0),
     mNodeParent(0)
{
   QDomDocument document;

   bool content = document.setContent(QString::fromStdString(xmlTree));

   if(content)
   {
      QString tag = document.childNodes().at(0).toElement().tagName();

      if( tag == "constant" )
      {
         double value = document.childNodes().at(0).toElement().attribute("value").toDouble();
         setOperator(Operators::constant, value);
         return;
      }
      /*else if(tag == "ifthen4") setOperator(Operators::ifthen4);
      else if( tag == "add") setOperator(Operators::add);
      else if( tag == "sub") setOperator(Operators::sub);
      else if( tag == "mul") setOperator(Operators::mul);
      else if( tag == "div") setOperator(Operators::div);
      else if( tag == "getEnergie") setOperator(Operators::getTemprature);
      else if( tag == "getEnergieDifferenz") setOperator(Operators::getTempratureDiff);
      else if( tag == "getEnergieWunsch") setOperator(Operators::getTempWunsch);
      else if( tag == "getLastTastverhaeltnis") setOperator(Operators::getTastverhaeltnis);*/

      for(std::size_t i = 0; i < document.childNodes().at(0).childNodes().size(); i++)
      {
         QDomDocument import;
         import.appendChild(import.importNode(document.childNodes().at(0).childNodes().at(i), true));
         insertChild(new Node(import.toString().toStdString()));
      }
   }
}

Node::Node(const Node &other)
   : mNodeParent(0)
{
   mOperator = other.getOperator();
   mValue = other.getValue();
   for(unsigned int i = 0; i < other.mChilds.size(); i++)
   {
      insertChild(clone(other.mChilds.at(i)));
   }
}

Node* Node::clone(const Node* other)
{
   if(other)
   {
      Node* node = new Node(0, other->getOperator(), other->getValue());

      for(std::size_t i = 0; i < other->mChilds.size(); i++)
      {
         node->insertChild(clone(other->mChilds.at(i)));
      }

      return node;
   }
   return 0;
}

void Node::insertChild(Node *child)
{
   if(child)
   {
      child->mNodeParent = this;
   }
   mChilds.push_back(child);
}

Node::~Node()
{
   for(std::size_t i = 0; i < mChilds.size(); i++)
   {
      delete mChilds.at(i);
      mChilds.at(i) = 0;
   }

   mChilds.clear();
}

bool Node::isNumber() const
{
   // Überprüfung ob node eine Konstante ist
   return mOperator == Operators::constant;
}

bool Node::isLeave()
{
   if( mChilds.size() == 0 && mNodeParent)
   {
      return true;
   }

   return false;
}

unsigned int Node::getOperator() const
{
   return mOperator;
}

double Node::getValue() const
{
   if(isNumber())
   {
      return mValue;
   }
   else
   {
      return 0;
   }
}

int Node::eval(TetrisBoard* board)
{
   // Auswertung des nodes
   // Wenn node Konstante ist, wert zurückgeben
   if(isNumber()) return mValue;

   // Implementierung der Operatoren

   if(mChilds.size() != 0)
   {
      std::vector<int> ergebnisse;
      ergebnisse.assign(mChilds.size(), 0);

      for(std::size_t i = 0; i < mChilds.size(); i++)
      {
         ergebnisse.at(i) = mChilds.at(i)->eval(board);
      }

      if(mOperator == Operators::ifthen4)
      {
         if(ergebnisse.at(0) > ergebnisse.at(1))
         {
            return ergebnisse.at(2);
         }
         else
         {
            return ergebnisse.at(3);
         }
      }

      if(mOperator == Operators::add)
      {
         return ergebnisse.at(0) + ergebnisse.at(1);
      }

      if(mOperator == Operators::sub)
      {
         return ergebnisse.at(0) - ergebnisse.at(1);
      }

      if(mOperator == Operators::mul)
      {
         return ergebnisse.at(0) * ergebnisse.at(1);
      }

      if(mOperator == Operators::Board)
      {
         int width = board->BoardWidth;
         int height = board->BoardHeight;

         int res = 0;

         for(int i = 0; i < width; i++)
         {
            res += board->shapeAt(i, ergebnisse.at(0)%height) == NoShape ? 2 : 0;
         }

         //return board->shapeAt(ergebnisse.at(0)%width, ergebnisse.at(1)%height);
      }

      if(mOperator == Operators::MoveLeft)
      {
         if(ergebnisse.at(0) > 0)
            board->moveLeft();
         return ergebnisse.at(0);
      }

      if(mOperator == Operators::MoveRight)
      {
         if(ergebnisse.at(0) < 0)
            board->moveRight();
         return ergebnisse.at(0);
      }

      if(mOperator == Operators::RotateLeft)
      {
         if(ergebnisse.at(0) > 0)
            board->rotateLeft();
         return ergebnisse.at(0);
      }

      if(mOperator == Operators::RotateRight)
      {
         if(ergebnisse.at(0) < 0)
            board->rotateRight();
         return ergebnisse.at(0);
      }

      if(mOperator == Operators::DoNothing)
      {
         return ergebnisse.at(0);
      }

      if(mOperator == Operators::Piece)
      {
         return board->getCurPiece().shape();
      }
   }

   return 0;
}

std::string Node::dump()
{
   std::ostringstream out;

   if(isNumber())
   {
      out << "(" << mValue << ")";
      return out.str();
   }

   std::string op;
   bool isOp = false;
   bool isGet = false;
   switch(mOperator)
   {
   /*case Operators::add:
      op = "+";
      isOp = true;
      break;
   case Operators::sub:
      op = "-";
      isOp = true;
      break;
   case Operators::mul:
      op = "*";
      isOp = true;
      break;
   case Operators::div:
      op = "/";
      break;
   case Operators::getTemprature:
      op = "getEnergie()";
      isGet = true;
      break;
   case Operators::getTempratureDiff:
      op = "getEnergieDifferenz()";
      isGet = true;
      break;
   case Operators::getTempWunsch:
      op = "getEnergieWunsch()";
      isGet = true;
      break;
   case Operators::getTastverhaeltnis:
      op = "getLastTastverhaeltnis()";
      isGet = true;
      break;*/
   }

   if(isGet)
   {
      out << op;
   }

   if(isOp)
   {
      out << "(";

      if(mChilds.size() >= 1)
      {
         out << mChilds.at(0)->dump();
      }
      else
      {
         out << "0";
      }

      out << op;

      if(mChilds.size() >= 2)
      {
         out << mChilds.at(1)->dump();
      }
      else
      {
         out << "0";
      }

      out << ")";
   }

   /*if(mOperator == Operators::div)
   {
      out << "(protdiv(";

      if(mChilds.size() >= 1)
      {
         out << mChilds.at(0)->dump();
      }
      else
      {
         out << "0";
      }

      out << ",";

      if(mChilds.size() >= 2)
      {
         out << mChilds.at(1)->dump();
      }
      else
      {
         out << "0";
      }

      out << "))";
   }*/

   /*if(mOperator == Operators::ifthen4)
   {
      out << "((";

      if(mChilds.size() >= 1)
      {
         out << "(" << mChilds.at(0)->dump() << ")";
      }
      else
      {
         out << "0";
      }

      out << ">";

      if(mChilds.size() >= 2)
      {
         out << "(" << mChilds.at(1)->dump() << ")";
      }
      else
      {
         out << "0";
      }

      out << ") ? ";

      if(mChilds.size() >= 3)
      {
         out << "(" << mChilds.at(2)->dump() << ")";
      }
      else
      {
         out << "0";
      }

      out << ":";

      if(mChilds.size() >= 4)
      {
         out << "(" << mChilds.at(3)->dump() << ")";
      }
      else
      {
         out << "0";
      }
      out << ")";
   }*/

   return out.str();
}

void Node::getXml(QDomElement &element, QDomDocument& document)
{
   if(isNumber())
   {
      element.setTagName("constant");
      element.setAttribute("value", mValue);
      return;
   }

   QString op;
   switch(mOperator)
   {
   /*case Operators::ifthen4:
      op = "ifthen4";
      break;
   case Operators::add:
      op = "add";
      break;
   case Operators::sub:
      op = "sub";
      break;
   case Operators::mul:
      op = "mul";
      break;
   case Operators::div:
      op = "div";
      break;
   case Operators::getTemprature:
      op = "getEnergie";
      break;
   case Operators::getTempratureDiff:
      op = "getEnergieDifferenz";
      break;
   case Operators::getTempWunsch:
      op = "getEnergieWunsch";
      break;
   case Operators::getTastverhaeltnis:
      op = "getLastTastverhaeltnis";
      break;*/
   }

   element.setTagName(op);

   for(std::size_t i = 0; i < mChilds.size(); i++)
   {
      QDomElement child = document.createElement("a");
      mChilds.at(i)->getXml(child, document);
      element.appendChild(child);
   }

   return;
}

std::string Node::dumpXml()
{
   QDomDocument document;

   QDomElement element = document.createElement("root");

   getXml(element, document);

   document.appendChild(element);

   return document.toString().toStdString();
}

void Node::setOperator(unsigned int pOperator, double value)
{
   mOperator = pOperator;

   if( isNumber() )
   {
      mValue = value;
   }
}

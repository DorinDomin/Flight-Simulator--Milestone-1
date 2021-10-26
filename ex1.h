#ifndef EXPRESSION_H
#define EXPRESSION_H

#include "Expression.h"

#endif

#ifndef EX1_H
#define EX1_H
#include <string>
#include <stack>
#include <map>

class UnaryOperator: public Expression{
 protected:
  Expression *expression;
};
class BinaryOperator: public Expression{
 protected:
  Expression *right;
  Expression *left;
};
class Variable: public Expression{
  string name;
  double value;
 public:
  Variable(string, double);
  Variable &operator++ ();
  Variable &operator-- ();
  Variable& operator++ (int);
  Variable& operator-- (int);
  Variable& operator+= (double);
  Variable& operator-= (double);
  double calculate();
};
class Value:public Expression{
  const double value;
 public:
  Value(const double);
  double calculate();
};
class UMinus: public UnaryOperator{
 public:
  UMinus(Expression*);
  double calculate();
  ~UMinus();
};
class UPlus: public UnaryOperator{
 public:
  UPlus(Expression*);
  double calculate();
  ~UPlus();
};
class Div: public BinaryOperator{
 public:
  Div(Expression*, Expression*);
  double calculate();
  ~Div();
};
class Plus: public BinaryOperator{
 public:
  Plus(Expression*, Expression*);
  double calculate();
  ~Plus();
};
class Minus: public BinaryOperator{
 public:
  Minus(Expression*, Expression*);
  double calculate();
  ~Minus();
};
class Mul: public BinaryOperator{
 public:
  Mul(Expression*, Expression*);
  double calculate();
  ~Mul();
};
class Interpreter{
 private:
  map<string ,double > variableTable;
 public:
  Expression* interpret(string);
  void setVariables(string);
  void getVariables();
  bool isRightPriorityGreater(char,char);
};


#endif //EX1_H

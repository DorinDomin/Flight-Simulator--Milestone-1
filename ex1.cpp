#include <iostream>
#include <queue>
#include "ex1.h"
#include <list>

//Expression
double Expression::calculate() {
  return 0;
}

//Plus
Plus::Plus(Expression *firstExpression, Expression *secondExpression) {
  left = firstExpression;
  right = secondExpression;
}
double Plus::calculate() {
  return left->calculate() + right->calculate();
}

//Minus
Minus::Minus(Expression *firstExpression, Expression *secondExpression) {
  left = firstExpression;
  right = secondExpression;
}
double Minus::calculate() {
  return left->calculate() - right->calculate();
}

//Mul
Mul::Mul(Expression *firstExpression, Expression *secondExpression) {
  left = firstExpression;
  right = secondExpression;
}
double Mul::calculate() {
  return left->calculate() * right->calculate();
}

//Div
Div::Div(Expression *firstExpression, Expression *secondExpression) {
  left = firstExpression;
  right = secondExpression;
}
double Div::calculate() {
  if (right->calculate() == 0) {
    throw "bad input";
  } else {
    return left->calculate() / right->calculate();
  }
  return 0;
}

//Plus
UPlus::UPlus(Expression * exp) {
  this->expression = exp;
}
double UPlus::calculate() {
  return expression->calculate();
}

//UMinus
UMinus::UMinus(Expression *exp) {
  this->expression = exp;
}
double UMinus::calculate() {
  return -1 * (expression->calculate());
}

//Variable
Variable::Variable(string n, double val) {
  this->name = n;
  this->value = val;
}
double Variable::calculate() {
  return value;
}
Variable &Variable::operator--() {
  double newValue = --value;
  this->value = newValue;
  return *this;
}
Variable &Variable::operator++() {
  double newValue = ++value;
  this->value = newValue;
  return *this;
}
Variable &Variable::operator++(int) {
  double newValue = value + 1;
  this->value = newValue;
  return *this;
}
Variable &Variable::operator--(int) {
  double newValue = value - 1;
  this->value = newValue;
  return *this;
}
Variable &Variable::operator+=(double secondValue) {
  double newValue = this->value + secondValue;
  this->value = newValue;
  return *this;
}
Variable &Variable::operator-=(double secondValue) {
  double newValue = this->value - secondValue;
  this->value = newValue;
  return *this;
}

//VALUE
Value::Value(const double val) : value(val) {
}
double Value::calculate() {
  return value;
}

Expression *Interpreter::interpret(string input) {
  string tempinput;
  for(unsigned int k=0;k<input.length();k++){
    if(input[k]!=' '){
      tempinput+=input[k];
    }
  }
  input=tempinput;
  queue<string> outputQueue;
  stack<char> opStack;
  int binaryOperators = 0;
  char first = input[0];
  int inputLength = input.length();
  if (inputLength == 1 && (first == '+' || first == '-' || first == '*' || first == '/')) {
    throw "bad input1";
  }
  //Checks validity of operators in input
  for (unsigned long i = 0; i < input.length() - 1; i++) {
    char currChar = input[i];
    char nextChar = input[i + 1];
    if (currChar == '+' || currChar == '-' || currChar == '*' || currChar == '/') {
      binaryOperators++;
      if (nextChar == '*' || nextChar == '/' || nextChar == '+' || nextChar == '-') {
        throw "bad input";
      }
    }
  }
  for (unsigned long i = 0; i < input.length() - 1; i++) {
    char currChar = input[i];
    char nextChar = input[i + 1];
    if (currChar == ')') {
      if (nextChar == '(') {
        throw "bad input2";
      }
    }
  }
  //Checks amount of parenthesis and order
  unsigned long openP = 0;
  unsigned long closeP = 0;
  for (unsigned long i = 0; i < input.length(); i++) {
    if (input[i] == '(') {
      openP++;
    }
    if (input[i] == ')') {
      closeP++;
    }
    if (closeP > openP) {
      throw "bad input3";
    }
  }
  if ((openP != closeP) || (openP + closeP == input.length())) {
    throw "bad input4";
  }

  string currString = "";
  string nextString = "";
  for (unsigned long i = 0; i < input.length() - 1; i++) {
    char currChar = input[i];
    char nextChar = input[i + 1];
    currString.push_back(currChar);
    nextString.push_back(nextChar);
    if (!(variableTable.find(currString) == variableTable.end())) {
      if (!(variableTable.find(nextString) == variableTable.end())) {
        cout<<"badinput5"<<endl;
        throw "bad input5";
      }
    }
    currString = "";
    nextString = "";
  }
  //Variable replacement with the equvilent values
  if (!variableTable.empty()) {
    int len=input.length();
    for (int i = 0; i < len; i++) {
      for (int j = len - 1; j>=i; j--) {
        if (variableTable.find(input.substr(i, j-i+1)) == variableTable.end()) {
        } else {
          string numberInString = to_string(variableTable.at(input.substr(i, j-i+1)));
          string firstPartOfString = input.substr(0, i);
          string secondPartOfString = input.substr(j+1, input.length() - (j));
          input = firstPartOfString;
          input += numberInString;
          input += secondPartOfString;
        }
        len=input.length();
      }
      len=input.length();
    }
  }
  //Checks if all variables are gone
  /*for (unsigned long i = 0; i < input.length(); i++) {
    char currChar = input[i];
    if ((currChar <= '9' && currChar >= '0') || currChar == '+' || currChar == '-' || currChar == '*' || currChar == '/'
        || currChar == '(' || currChar == ')' || currChar == '.') {
    } else {
      cout<<"badinput6"<<endl;
      cout<<input.length()<<endl;
      cout<<input<<endl;
      this->getVariables();
      throw "bad input666";
    }
  }*/
  if(binaryOperators==0){
    return new Value(stod(input));
  }

  char currChar;
  char nextChar;
  for (unsigned long i = 0; i < input.length() - 1; i++) {
    currChar = input[i];
    nextChar = input[i + 1];
    if ((currChar == '-') && (nextChar == '-')) {
      input[i] ='?';
      input[i + 1] = '?';
    }
  }
  //Operator Replacement, we change negative to '?' and positive to '^' to avoid confusion later on
  if (input[0] == '-') {
    input[0] = '?';
  }
  if (input[0] == '+') {
    input[0] = '^';
  }
  for (unsigned long i = 0; i < input.length() - 1; i++) {
    currChar = input[i];
    nextChar = input[i + 1];
    if ((currChar == '(' || currChar == '+' || currChar == '-' || currChar == '*' || currChar == '/')
        && (nextChar == '-')) {
      input[i + 1] = '?';
    }
    if ((currChar == '(' || currChar == '+' || currChar == '-' || currChar == '*' || currChar == '/')
        && (nextChar == '+')) {
      input[i + 1] = '^';
    }
  }

  //Shunting yard algorithm infix to postfix
  string currOperator;
  unsigned long j = 0;
  while (input.length() > 0) {
    currOperator = "";

    //If current operator is one of the following: +,-,*,/,(,^,?
    if (input[0] == '+' || input[0] == '-' || input[0] == '*' || input[0] == '/' || input[0] == '(' || input[0] == '^'
        || input[0] == '?') {
      //If operator stack is empty
      if (opStack.empty()) {
        opStack.push(input[0]);
      }

        //Checks if we can push the next operator to the stack
      else {
        if ((input[0] == '(') || (opStack.top() == '(') || ((isRightPriorityGreater(opStack.top(), input[0])))) {
          opStack.push(input[0]);
        }

          //If we cant then we need to move operators from stack to queue
        else {
          while ((!opStack.empty()) && !isRightPriorityGreater(opStack.top(), input[0])) {
            currOperator = "";
            currOperator.push_back(opStack.top());
            outputQueue.push(currOperator);
            opStack.pop();
          }
          opStack.push(input[0]);
        }
      }
      input = input.substr(1, input.length() - 1);
    }

      //If current operator is ')'
    else if (input[0] == ')') {
      while (opStack.top() != '(') {
        currOperator = "";
        currOperator.push_back(opStack.top());
        outputQueue.push(currOperator);
        opStack.pop();
      }
      opStack.pop();
      input = input.substr(1, input.length() - 1);
    }

      //We are dealing with a number
    else {
      j = 0;
      while (j < input.length() && input[j] != '+' && input[j] != '-' && input[j] != '*' && input[j] != '/'
          && input[j] != '?' && input[j] != '^' && input[j] != '(' && input[j] != ')') {
        j++;
      }
      outputQueue.push((input.substr(0, j)));
      input = input.substr(j, input.length());
    }
  }
  //Empties opStack if any operators are left there
  while (!opStack.empty()) {
    //Enque pop
    currOperator = "";
    currOperator.push_back(opStack.top());
    outputQueue.push(currOperator);
    opStack.pop();
  }


  //Convert to expressions
  list<Expression *> expressionList;
  string *outputArray = new string[outputQueue.size()];
  int outputOueueSize = outputQueue.size();
  stack<Expression *> expressionStack;
  int k = 0;
  Expression *currEx1, *currEx2;

  if (outputOueueSize == 1) {
    return new Value(stod(outputQueue.front()));
  }

  //Transfer Queue to Array
  while (!outputQueue.empty()) {
    outputArray[k] = outputQueue.front();
    outputQueue.pop();
    k++;
  }
  int deletedCells = 0;

  //Runs until we have one 'Q' left and it is the only element in the array. 'Q' will be our symbol for an expression
  while (deletedCells != (outputOueueSize - 1) || outputArray[0] != "Q") {
    /*if (binaryOperators == 0) {
      throw "bad input7";
    }*/
    int index = 0;
    int expressionsFound = 0;

    //Counts how many expressions we have and moves them to expressionStack
    while (outputArray[index] != "+" && outputArray[index] != "-" && outputArray[index] != "*"
        && outputArray[index] != "/" && outputArray[index] != "^" && outputArray[index] != "?") {
      if (outputArray[index] == "Q") {
        expressionsFound++;
        expressionStack.push(expressionList.front());
        expressionList.pop_front();
      }
      index++;
    }

    //In case operator is '?'
    if (outputArray[index] == "?") {
      outputArray[index] = "Q";

      //If we are dealing with an expression
      if (outputArray[index - 1] == "Q") {
        Expression *currEx = expressionStack.top();
        expressionStack.pop();
        currEx = new UMinus(currEx);
        expressionStack.push(currEx);
        for (int f = index; f < outputOueueSize - deletedCells; f++) {
          outputArray[f - 1] = outputArray[f];
        }
      }

        //If we are dealing with a non expression
      else {
        expressionList.push_back(new UMinus(new Value(stod(outputArray[index - 1]))));
        for (int f = index; f < outputOueueSize - deletedCells; f++) {
          outputArray[f - 1] = outputArray[f];
        }
      }
      deletedCells++;
    }

      //In case operator is '^'
    else if (outputArray[index] == "^") {
      outputArray[index] = "Q";

      //If we are dealing with an expression
      if (outputArray[index - 1] == "Q") {
        Expression *currEx = expressionStack.top();
        expressionStack.pop();
        currEx = new UPlus(currEx);
        expressionStack.push(currEx);
        for (int f = index; f < outputOueueSize - deletedCells; f++) {
          outputArray[f - 1] = outputArray[f];
        }
      }

        //If we are dealing with a non expression
      else {
        expressionList.push_back(new UPlus(new Value(stod(outputArray[index - 1]))));
        for (int f = index; f < outputOueueSize - deletedCells; f++) {
          outputArray[f - 1] = outputArray[f];
        }
      }
      deletedCells++;
    }

      //In case operator is '+'
    else if (outputArray[index] == "+") {
      binaryOperators--;
      //If we are dealing with two expressions
      if (outputArray[index - 1] == "Q" && outputArray[index - 2] == "Q") {
        currEx1 = expressionStack.top();
        expressionStack.pop();
        currEx2 = expressionStack.top();
        expressionStack.pop();
        currEx1 = new Plus(currEx2, currEx1);
        expressionStack.push(currEx1);
        outputArray[index] = "Q";
        for (int f = index; f < outputOueueSize - deletedCells; f++) {
          outputArray[f - 2] = outputArray[f];
        }
      }

        //If we are dealing with a non expression and then a expression
      else if (outputArray[index - 1] == "Q" && outputArray[index - 2] != "Q") {
        currEx1 = expressionStack.top();
        expressionStack.pop();
        currEx2 = new Value(stod(outputArray[index - 2]));
        currEx1 = new Plus(currEx2, currEx1);
        expressionStack.push(currEx1);
        outputArray[index] = "Q";
        for (int f = index; f < outputOueueSize - deletedCells; f++) {
          outputArray[f - 2] = outputArray[f];
        }

      }

        //If we are dealing with a expression and then a non expression
      else if (outputArray[index - 1] != "Q" && outputArray[index - 2] == "Q") {
        currEx1 = new Value(stod(outputArray[index - 1]));
        currEx2 = expressionStack.top();
        expressionStack.pop();
        currEx2 = new Plus(currEx2, currEx1);
        expressionStack.push(currEx2);
        for (int f = index + 1; f < outputOueueSize - deletedCells; f++) {
          outputArray[f - 2] = outputArray[f];
        }

      }

        //If we are dealing with two non expressions
      else {
        outputArray[index] = "Q";
        expressionList.push_back(new Plus(new Value(stod(outputArray[index - 2])),
                                          new Value(stod(outputArray[index - 1]))));
        for (int f = index; f < outputOueueSize - deletedCells; f++) {
          outputArray[f - 2] = outputArray[f];
        }
      }
      deletedCells += 2;

    }
      //In case operator is '-'
    else if (outputArray[index] == "-") {
      binaryOperators--;
      //If we are dealing with two expressions
      if (outputArray[index - 1] == "Q" && outputArray[index - 2] == "Q") {
        currEx1 = expressionStack.top();
        expressionStack.pop();
        currEx2 = expressionStack.top();
        expressionStack.pop();
        currEx1 = new Minus(currEx2, currEx1);
        expressionStack.push(currEx1);
        outputArray[index] = "Q";
        for (int f = index; f < outputOueueSize - deletedCells; f++) {
          outputArray[f - 2] = outputArray[f];
        }
      }
        //If we are dealing with a non expression and then a expression
      else if (outputArray[index - 1] == "Q" && outputArray[index - 2] != "Q") {
        currEx1 = expressionStack.top();
        expressionStack.pop();
        currEx2 = new Value(stod(outputArray[index - 2]));
        currEx1 = new Minus(currEx2, currEx1);
        expressionStack.push(currEx1);
        outputArray[index] = "Q";
        for (int f = index; f < outputOueueSize - deletedCells; f++) {
          outputArray[f - 2] = outputArray[f];
        }

      }
        //If we are dealing with a non expression and then a expression
      else if (outputArray[index - 1] != "Q" && outputArray[index - 2] == "Q") {
        currEx1 = new Value(stod(outputArray[index - 1]));
        currEx2 = expressionStack.top();
        expressionStack.pop();
        currEx2 = new Minus(currEx2, currEx1);
        expressionStack.push(currEx2);
        for (int f = index + 1; f < outputOueueSize - deletedCells; f++) {
          outputArray[f - 2] = outputArray[f];
        }

      }
        //If we are dealing with two non expressions
      else {
        outputArray[index] = "Q";
        expressionList.push_back(new Minus(new Value(stod(outputArray[index - 2])),
                                           new Value(stod(outputArray[index - 1]))));
        for (int f = index; f < outputOueueSize - deletedCells; f++) {
          outputArray[f - 2] = outputArray[f];
        }
      }
      deletedCells += 2;
    }

      //In case operator is '*'
    else if (outputArray[index] == "*") {
      binaryOperators--;
      //If we are dealing with two expressions
      if (outputArray[index - 1] == "Q" && outputArray[index - 2] == "Q") {
        currEx1 = expressionStack.top();
        expressionStack.pop();
        currEx2 = expressionStack.top();
        expressionStack.pop();
        currEx1 = new Mul(currEx2, currEx1);
        expressionStack.push(currEx1);
        outputArray[index] = "Q";
        for (int f = index; f < outputOueueSize - deletedCells; f++) {
          outputArray[f - 2] = outputArray[f];
        }
      }
        //If we are dealing with a non expression and then a expression
      else if (outputArray[index - 1] == "Q" && outputArray[index - 2] != "Q") {
        currEx1 = expressionStack.top();
        expressionStack.pop();
        currEx2 = new Value(stod(outputArray[index - 2]));
        currEx1 = new Mul(currEx2, currEx1);
        expressionStack.push(currEx1);
        outputArray[index] = "Q";
        for (int f = index; f < outputOueueSize - deletedCells; f++) {
          outputArray[f - 2] = outputArray[f];
        }

      }
        //If we are dealing with a non expression and then a expression
      else if (outputArray[index - 1] != "Q" && outputArray[index - 2] == "Q") {
        currEx1 = new Value(stod(outputArray[index - 1]));
        currEx2 = expressionStack.top();
        expressionStack.pop();
        currEx2 = new Mul(currEx2, currEx1);
        expressionStack.push(currEx2);
        for (int f = index + 1; f < outputOueueSize - deletedCells; f++) {
          outputArray[f - 2] = outputArray[f];
        }

      }
        //If we are dealing with two non expressions
      else {
        outputArray[index] = "Q";
        expressionList.push_back(new Mul(new Value(stod(outputArray[index - 2])),
                                         new Value(stod(outputArray[index - 1]))));
        for (int f = index; f < outputOueueSize - deletedCells; f++) {
          outputArray[f - 2] = outputArray[f];
        }
      }
      deletedCells += 2;
    }

      //In case operator is '/'
    else if (outputArray[index] == "/") {
      binaryOperators--;
      //If we are dealing with two expressions
      if (outputArray[index - 1] == "Q" && outputArray[index - 2] == "Q") {
        currEx1 = expressionStack.top();
        expressionStack.pop();
        currEx2 = expressionStack.top();
        expressionStack.pop();
        currEx1 = new Div(currEx2, currEx1);
        expressionStack.push(currEx1);
        outputArray[index] = "Q";
        for (int f = index; f < outputOueueSize - deletedCells; f++) {
          outputArray[f - 2] = outputArray[f];
        }
      }
        //If we are dealing with a non expression and then a expression
      else if (outputArray[index - 1] == "Q" && outputArray[index - 2] != "Q") {
        currEx1 = expressionStack.top();
        expressionStack.pop();
        currEx2 = new Value(stod(outputArray[index - 2]));
        currEx1 = new Div(currEx2, currEx1);
        expressionStack.push(currEx1);
        outputArray[index] = "Q";
        for (int f = index; f < outputOueueSize - deletedCells; f++) {
          outputArray[f - 2] = outputArray[f];
        }

      }
        //If we are dealing with a non expression and then a expression
      else if (outputArray[index - 1] != "Q" && outputArray[index - 2] == "Q") {
        currEx1 = new Value(stod(outputArray[index - 1]));
        currEx2 = expressionStack.top();
        expressionStack.pop();
        currEx2 = new Div(currEx2, currEx1);
        expressionStack.push(currEx2);
        for (int f = index + 1; f < outputOueueSize - deletedCells; f++) {
          outputArray[f - 2] = outputArray[f];
        }

      }
        //If we are dealing with two non expressions
      else {
        outputArray[index] = "Q";
        expressionList.push_back(new Div(new Value(stod(outputArray[index - 2])),
                                         new Value(stod(outputArray[index - 1]))));
        for (int f = index; f < outputOueueSize - deletedCells; f++) {
          outputArray[f - 2] = outputArray[f];
        }
      }
      deletedCells += 2;
    }

    while (!expressionStack.empty()) {
      expressionList.push_front(expressionStack.top());
      expressionStack.pop();
    }

  }
  delete[] outputArray;
  return expressionList.front();

}

//Defines variable map
void Interpreter::setVariables(string variableList) {
  int k = 0;
  char currChar;
  int equalsFound = 0;
  int variableListSize = (int) (variableList.size());
  int lastStringRead = 0;
  while (k != -1) {

    //Find the next '='
    k = variableList.find("=", k);
    if (k == -1) {
      if (equalsFound == 0 && variableList.length() > 0) {
        throw "bad input8";
      }
      break;
    }
    equalsFound = equalsFound + 1;
    int j = k + 1;
    int m = k - 1;

    //Checks for legality of what is right of the '=' sign
    while (variableList[j] != ';' && j < variableListSize) {
      currChar = variableList[j];
      if (j == k + 1) {
        if (!((currChar <= '9' && currChar >= '0') || currChar == '-')) {
          throw "bad input9";
        }
      } else {
        if (!((currChar <= '9' && currChar >= '0') || currChar == '.')) {
          throw "bad input10";
        }
      }
      j++;
    }
    if (j == k + 1) {
      throw "bad input11";
    }
    if (j == k + 2 && variableList[j - 1] == '-') {
      throw "bad input12";
    } else {
      if (variableList[j - 1] == '.') {
        throw "bad input13";
      }
    }

    //Checks for legality of what is left of the '=' sign
    while (m >= 0 && variableList[m] != ';' && m >= lastStringRead) {
      currChar = variableList[m];
      if (!((currChar <= '9' && currChar >= '0') || (currChar >= 'a' && currChar <= 'z')
          || (currChar >= 'A' && currChar <= 'Z') || (currChar == '_'))) {
        throw "bad input14";
      }
      m--;
    }

    //Checks that variable starts with a letter
    if (!((currChar >= 'a' && currChar <= 'z') || (currChar >= 'A' && currChar <= 'Z'))) {
      throw "bad input15";
    }

    lastStringRead = j;

    //Adds pair to the variable map
    if (variableTable.count(variableList.substr(m + 1, k - (m + 1))) > 0) {
      variableTable.erase(variableList.substr(m + 1, k - (m + 1)));
      variableTable.insert(
          pair<string, double>(variableList.substr(m + 1, k - (m + 1)), stod(variableList.substr(k + 1, j - (k + 1)))));
    } else {
      variableTable.insert(
          pair<string, double>(variableList.substr(m + 1, k - (m + 1)), stod(variableList.substr(k + 1, j - (k + 1)))));
    }

    //Advances k so we move on to the next '='
    k++;
  }
  if (variableList[variableListSize - 1] != ';' && lastStringRead != variableListSize) {
    throw "bad input16";
  }
  if (variableList[variableListSize - 1] == ';' && lastStringRead != variableListSize - 1) {
    throw "bad input17";
  }
}

//Defines and checks algebraic priority between two operators
bool Interpreter::isRightPriorityGreater(char c1, char c2) {
  map<char, int> priorityRank;
  priorityRank.insert(pair<char, int>('+', 1));
  priorityRank.insert(pair<char, int>('-', 1));
  priorityRank.insert(pair<char, int>('*', 2));
  priorityRank.insert(pair<char, int>('/', 2));
  priorityRank.insert(pair<char, int>('?', 3));
  priorityRank.insert(pair<char, int>('^', 3));
  priorityRank.insert(pair<char, int>('(', 0));
  if (priorityRank.at(c1) <= priorityRank.at(c2))
    return true;
  return false;
}

Div::~Div() {
  delete (left);
  delete (right);
}
Mul::~Mul() {
  delete (left);
  delete (right);
}
Plus::~Plus() {
  delete (left);
  delete (right);
}
Minus::~Minus() {
  delete (left);
  delete (right);
}
UPlus::~UPlus() {
  delete (expression);
}
UMinus::~UMinus() {
  delete (expression);
}
void Interpreter::getVariables() {
  try {
    for (pair<string, int> p:this->variableTable) {
      cout << p.first << "=" << p.second << endl;
    }
  }
  catch (...){
  }
}

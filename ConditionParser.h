//
// Created by netanel on 20/12/2019.
//

#ifndef EX3__CONDITIONPARSER_H_
#define EX3__CONDITIONPARSER_H_
#include "Command.h"
//ConditionParser Class in charge of handling conditions
class ConditionParser: public Command{
 public:
  int firstpos;
  int secondpos;
  string op;
  int execute(string *,int);
  ConditionParser(map<string, VarData *> *m1){
    this->var2val=m1;
  }
  ConditionParser(){

  }
};

//IfCommand Class, in charge of handling "if"
class IfCommand:public ConditionParser{
 public:
  IfCommand(map<string, VarData *> *m1,int firstpos2,int secondpos2,string op2){
    this->firstpos=firstpos2;
    this->secondpos=secondpos2;
    this->op=op2;
    this->var2val=m1;
  }
  int execute(string* str,int pos) {
    map<string, Command *> mapCommands;
    OpenServerCommand open_server_command(var2val);
    ConnectCommand connect_command(var2val);
    DefineVarCommand define_var_command(var2val);
    PrintCommand print_command;
    SleepCommand sleep_command;

    mapCommands.insert(pair<string, Command *>("openDataServer", &open_server_command));
    mapCommands.insert(pair<string, Command *>("connectControlClient", &connect_command));
    mapCommands.insert(pair<string, Command *>("var", &define_var_command));
    mapCommands.insert(pair<string, Command *>("Print", &print_command));
    mapCommands.insert(pair<string, Command *>("Sleep", &sleep_command));

    //Calculates the value of the arguments of the if
    Expression *ex1=i1->interpret(str[firstpos]);
    Expression *ex2=i1->interpret(str[secondpos]);
    double first=ex1->calculate();
    double second=ex2->calculate();
    delete(ex1);
    delete(ex2);
    int start = pos;
    int end;
    while (str[pos] != "}") {
      pos++;
    }
    pos--;
    end = pos;
    bool condition;
    //Checks if the condition is true
    if (op == ">" && first > second) {
      condition = true;
    } else if (op == ">=" && first >= second) {
      condition = true;
    } else if (op == "==" && first == second) {
      condition = true;
    } else if (op == "<" && first < second) {
      condition = true;
    } else if (op == "<=" && first <= second) {
      condition = true;
    } else if (op == "!=" && first != second) {
      condition = true;
    } else {
      condition = false;
    }
    mutex_lock1.unlock();
    
    //Executes the lines inside the condition if the condition is true
    if (condition) {
      mutex_lock1.lock();
      pos = start;
      while (pos < end) {
        if (mapCommands.find(str[pos]) != mapCommands.end()) {
          Command *c = (mapCommands.find(str[pos])->second);
          pos = c->execute(str, pos);
        } else {
          VarData *v = var2val->at(str[pos]);
          double y = simplify(str, pos + 2);
          v->setValue(y);
          var2val->at(str[pos])->setChanged();
          var2val->at(str[pos])->setSentFalse();
          i1->setVariables(
              str[pos] + "=" + to_string(simplify(str, pos + 2)) + ";");/////////////////////////////////Fix
          pos = progress(str, pos);
        }
      }
    }
    return 1;
  }
};
//LoopCommand Class, in charge of handling "while"
class LoopCommand:public ConditionParser{
 public:
  LoopCommand(map<string, VarData *> *m1,int firstpos2,int secondpos2,string op2){
    this->firstpos=firstpos2;
    this->secondpos=secondpos2;
    this->op=op2;
    this->var2val=m1;
  }
  int execute(string* str,int pos) {

    map<string, Command*> mapCommands;
    OpenServerCommand open_server_command(var2val);
    ConnectCommand connect_command(var2val);
    DefineVarCommand define_var_command(var2val);
    PrintCommand print_command;
    SleepCommand sleep_command;

    mapCommands.insert(pair<string, Command*>("openDataServer",&open_server_command));
    mapCommands.insert(pair<string, Command*>("connectControlClient", &connect_command));
    mapCommands.insert(pair<string, Command*>("var", &define_var_command));
    mapCommands.insert(pair<string, Command*>("Print", &print_command));
    mapCommands.insert(pair<string, Command*>("Sleep", &sleep_command));
    //Calculates the value of the arguments of the while
    Expression *ex1=i1->interpret(str[firstpos]);
    Expression *ex2=i1->interpret(str[secondpos]);
    double first=ex1->calculate();
    double second=ex2->calculate();
    delete(ex1);
    delete(ex2);
    int start=pos;
    int end;
    while (str[pos]!="}"){
      pos++;
    }
    pos--;
    end=pos;
    bool condition;
    //Checks if the condition is true
    if(op==">" && first>second){
      condition=true;
    }
    else if(op==">=" && first>=second){
      condition=true;
    }
    else if(op=="==" && first==second){
      condition=true;
    }
    else if(op=="<" && first<second){
      condition=true;
    }
    else if(op=="<=" && first<=second){
      condition=true;
    }
    else if(op=="!=" && first!=second){
      condition=true;
    } else{
      condition=false;
    }
    mutex_lock1.unlock();
    //Executes the lines inside the while
    while (condition){
      mutex_lock1.lock();
      pos=start;
      while (pos < end) {
        if(mapCommands.find(str[pos])!=mapCommands.end()){
          Command * c = (mapCommands.find(str[pos])->second);
          pos = c->execute(str,pos);
        }
        else{
          VarData* v=var2val->at(str[pos]);
          double y=simplify(str,pos+2);
          v->setValue(y);
          //cerr << "out" << endl;
          var2val->at(str[pos])->setChanged();
          var2val->at(str[pos])->setSentFalse();
          i1->setVariables(str[pos]+"="+to_string(simplify(str,pos+2))+";");/////////////////////////////////Fix
          pos=progress(str,pos);
        }
      }
      //Updates the arguments of the condition
      Expression *ex3=i1->interpret(str[firstpos]);
      Expression *ex4=i1->interpret(str[secondpos]);
      first=ex3->calculate();
      second=ex4->calculate();
      delete(ex3);
      delete(ex4);
      mutex_lock1.unlock();
      //Checks if the condition is still true or not
      if(op==">" && first>second){
        condition=true;
      }
      else if(op==">=" && first>=second){
        condition=true;
      }
      else if(op=="==" && first==second){
        condition=true;
      }
      else if(op=="<" && first<second){
        condition=true;
      }
      else if(op=="<=" && first<=second){
        condition=true;
      }
      else if(op=="!=" && first!=second){
        condition=true;
      } else{
        condition=false;
      }
      sleep(0.1);
    }
    return 1;
  }
};

//Execute method of ConditionParser, in charge of handling the logic between the 3 classes
int ConditionParser::execute(string * str, int pos) {
  int start = pos;

  //If the line is a loop command then we create a loopcommand object and execute it
  if (str[start] == "while") {
    LoopCommand loop_command(this->var2val, pos + 1, pos + 3, str[pos + 2]);
    while (str[pos] != "{") {
      pos++;
    }
    loop_command.execute(str, pos + 2);
  }
  //If the line is an if command then we create a ifcommand object and execute it
  if (str[start] == "if") {
    IfCommand if_command(this->var2val, pos + 1, pos + 3, str[pos + 2]);
    while (str[pos] != "{") {
      pos++;
    }
    if_command.execute(str, pos + 2);
  }
  //Advances pos to the next command
  while (str[pos] != "}") {
    pos++;
  }
  pos++;
  return pos + 1;
}


#endif //EX3__CONDITIONPARSER_H_

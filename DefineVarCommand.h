#ifndef EX3__DEFINEVARCOMMAND_H_
#define EX3__DEFINEVARCOMMAND_H_
#include <iostream>
#include <thread>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <map>
#include "MapObj.h"
#include <regex>
#include "ex1.h"
#include "Command.h"
#include "VarData.h"

using namespace std;


/**
 * DefineVarCommand‬‬ class.
 */

class DefineVarCommand : public Command {
 public:
  DefineVarCommand(map<string, VarData *> *m1);
  int execute(string *, int);
  ~DefineVarCommand();
};
DefineVarCommand::DefineVarCommand(map<string, VarData *> *m1) {
  this->var2val = m1;
}
int DefineVarCommand::execute(string *str, int pos) {
  string name=str[pos+1];
  int index = pos + 1;
  (*this->var2val)[name] = new VarData();
  index += 1;
  //If this var command has a <-
  if (str[index] == "<-") {
    (*this->var2val)[name]->setDirect('0');
    (*this->var2val)[name]->setLocation(str,index+2);
  }
  //If this var command has a ->
  else if (str[index] == "->") {
    (*this->var2val)[name]->setDirect('1');
    (*this->var2val)[name]->setLocation(str,index+2);
    //Otherwise it has a '='
  } else{
    i1->setVariables(name+"="+to_string(simplify(str,index+1))+";");
  }
  return progress(str,pos);
}
DefineVarCommand::~DefineVarCommand() = default;
#endif //EX3__DEFINEVARCOMMAND_H_


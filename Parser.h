#include <map>
#include <list>
#include "Command.h"
#include "ex1.h"
#include "MapObj.h"
#include "OpenServerCommand.h"
#include "VarData.h"
#include "ConnectCommand.h"
#include "DefineVarCommand.h"
#include "ConditionParser.h"

using namespace std;
#ifndef EX3__PARSER_H_
#define EX3__PARSER_H_

void Parser(string * tokens,map<string, VarData *>* var2val) {

  //Creates instances of all the types of commands
  map<string, Command*> mapCommands;
  OpenServerCommand open_server_command(var2val);
  ConnectCommand connect_command(var2val);
  DefineVarCommand define_var_command(var2val);
  PrintCommand print_command;
  SleepCommand sleep_command;
  ConditionParser condition_parser(var2val);

  mapCommands.insert(pair<string, Command*>("openDataServer",&open_server_command));
  mapCommands.insert(pair<string, Command*>("connectControlClient", &connect_command));
  mapCommands.insert(pair<string, Command*>("var", &define_var_command));
  mapCommands.insert(pair<string, Command*>("Print", &print_command));
  mapCommands.insert(pair<string, Command*>("Sleep", &sleep_command));
  mapCommands.insert(pair<string, Command*>("while", &condition_parser));
  mapCommands.insert(pair<string, Command*>("if", &condition_parser));

  int index = 1;
  int previndex;
  while (index < stoi(tokens[0])) {
    if(mapCommands.find(tokens[index])!=mapCommands.end()){
      mutex_lock1.lock();
      Command * c = (mapCommands.find(tokens[index])->second);
      previndex=index;
      index = c->execute(tokens,index);
      mutex_lock1.unlock();

      //If this is an open server command
      if(mapCommands.find(tokens[previndex])->first=="openDataServer") {
        mutex_lock2.lock();
        while (!connectionAchieved) {
          mutex_lock2.unlock();
          this_thread::sleep_for(std::chrono::milliseconds(200));
        }
      }
      //If this is a connect client command
      else if(mapCommands.find(tokens[previndex])->first=="connectControlClient"){
        mutex_lock3.lock();
        while (!connectionAchieved2) {
          mutex_lock3.unlock();
          this_thread::sleep_for(std::chrono::milliseconds(200));
        }
      }
    }
    else{
      //Otherwise
      mutex_lock1.lock();
      i1->setVariables(tokens[index]+"="+to_string(simplify(tokens+2,index))+";");/////////////////////////////////Fix
      var2val->at(tokens[index])->setValue(simplify(tokens,index+2));
      var2val->at(tokens[index])->setChanged();
      var2val->at(tokens[index])->setSentFalse();
      mutex_lock1.unlock();
      index=progress(tokens,index);
    }
  }

  //Sets endOfProgram to true
  mutex_lock4.lock();
  endOfProgram= true;
  mutex_lock4.unlock();
}
#endif //EX3__PARSER_H_

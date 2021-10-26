#include <string>
#include <thread>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <map>
#include <list>
#include <mutex>
#include <arpa/inet.h>
#include "ex1.h"
#include "MapObj.h"
#include "VarData.h"
#include <chrono>
using namespace std;
#ifndef EX3__COMMAND_H_
#define EX3__COMMAND_H_
mutex mutex_lock1;
mutex mutex_lock2;
mutex mutex_lock3;
mutex mutex_lock4;
mutex mutex_lock5;
mutex mutex_lock6;

//Defining global variables
bool endOfProgram=false;
bool ack1=false;
bool ack2=false;
bool connectionAchieved= false;
bool connectionAchieved2=false;
Interpreter* i1=new Interpreter;


//Cheecks if we need to simplify an expression
bool needsimplify(string* tokens,int index){
  string temp="";
  while(tokens[index]!="$"){
    temp+=tokens[index];
    index++;
  }
  if(temp[0]=='\"') {
    return false;
  }
  return true;
}

//Simplifies the expression
double simplify(string * tokens,int index){
  string temp="";
  while(tokens[index]!="$"){
    for(unsigned int i=0;i<tokens[index].length();i++){
      if(tokens[index][i]!=' '){
        temp+=tokens[index][i];
      }
    }
    index++;
  }
  double x=0;
  Expression* expression=i1->interpret(temp);
  x=expression->calculate();
  delete(expression);
  return x;
}

//Progresses array index to next line
int progress(string * tokens,int index){
  while(index<stoi(tokens[0])&&tokens[index]!="$"){
    index++;
  }
  return index+1;
}

//Command Class
class Command{
 protected:
  map<string, VarData *> *var2val;
 public:
  virtual int execute(string *, int) = 0;
  virtual ~Command();
};
Command::~Command() {

}

//PrintCommand -In charge of handling "Print"
class PrintCommand:public Command{
  int execute(string* str,int index){
    if(needsimplify(str,index+1)) {
      cout << simplify(str, index + 1) << endl;
    }
    else{
      cout<< str[index+1].substr(1,str[index+1].length()-2)<<endl;
    }
    return progress(str, index);
  }
};
//SleepCommand -In charge of handling "Sleep"
class SleepCommand:public Command{
  int execute(string* str,int index){
    int x;
    if(needsimplify(str,index+1)){
      x=simplify(str,index+1)/1000;
    }
    else{
      x=stoi(str[index+1].substr(1,str[index+1].length()-2))/1000;
    }
    mutex_lock1.unlock();
    sleep(x);
    mutex_lock1.lock();
    return progress(str,index);
  }
};
#endif //EX3__COMMAND_H_

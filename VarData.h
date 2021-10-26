#ifndef EX3__VARDATA_H_
#define EX3__VARDATA_H_
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

using namespace std;
/**
 * VarData‬‬ class.
 */
class VarData {
  MapObj *value;
  string sim="";
  char direct='2';
  bool changed= false;
  bool sent=false;
 public:
  VarData(string,char);
  VarData();
  ~VarData();
  void setValue(double val);
  void setLocation(string *str, int index);
  void setDirect(char direction);
  double getValue();
  bool getChanged();
  void setChanged();
  bool getSent();
  void setSentTrue();
  void setSentFalse();
  string getLocation();
  char getDirect();
};
VarData::VarData(string s,char direction) {
  this->value = new MapObj(-1); // ask for sim
  this->sim = s;
  this->direct = direction;
}
VarData::VarData(){
  this->value=new MapObj(-1);
}
void VarData::setValue(double val) {
  this->value->set(val);
}
double VarData::getValue() {
  return this->value->get();
}
void VarData::setLocation(string *str, int index) {
  this->sim = str[index].substr(1,str[index].length()-2); // srcopy ?
}
string VarData::getLocation() {
  return this->sim;
}
void VarData::setDirect(char direction) {
  this->direct = direction;
}
char VarData::getDirect() {
  return this->direct;
}
bool VarData::getChanged() {
  return this->changed;
}
void VarData::setChanged() {
  this->changed= true;
}
bool VarData::getSent() {
  return this->sent;
}
void VarData::setSentTrue() {
  this->sent= true;
}
void VarData::setSentFalse() {
  this->sent= false;
}
VarData::~VarData() {
  delete this->value;
}
#endif //EX3__VARDATA_H_

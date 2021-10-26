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
 * ‫‪ConnectCommand‬‬ class.
 */
class ConnectCommand : public Command {
  int client_socket;
  int is_connect;
 public:
  void ConnectControlClient(string *, int,int);
  ConnectCommand(map<string, VarData *> *m1);
  int execute(string *, int);
  int send(string, double);
  ~ConnectCommand();
};


// Constructor
ConnectCommand::ConnectCommand(map<string, VarData *> *m1) {
  this->client_socket = socket(AF_INET, SOCK_STREAM, 0);
  this->is_connect = -1;
  this->var2val = m1;
}
void ConnectCommand::ConnectControlClient(string *str, int pos,int port) {
  int position = pos + 1;
  string local_host_ddr = "";
  /*regex e("\\b((25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)(\\.|$)){4}\\b"); // checks ip
  if (regex_match(str[position], e)) {
    local_host_ddr = str[position];
  }*/
  local_host_ddr = str[position];
  // Localhost address
  // create sockadd object to hold the simulator's address
  sockaddr_in address;
  address.sin_family = AF_INET;
  const char * temp=str[position].substr(1,str[position].length()-2).c_str();
  address.sin_addr.s_addr=inet_addr(temp);
  address.sin_port = htons(port);

  // Requesting to connect with the simulator on a local host with a given port
  this->is_connect = connect(this->client_socket, (struct sockaddr *) &address, sizeof(address));
  while(this->is_connect==-1){
    //cout<<"could not connect client"<<endl;
    this->is_connect=connect(this->client_socket, (struct sockaddr *) &address, sizeof(address));
    sleep(1);
  }
  //cout<<"connectedclient"<<endl;
  mutex_lock3.lock();
  connectionAchieved2= true;
  mutex_lock3.unlock();
  while (true){
    mutex_lock1.lock();
    for(pair<string,VarData*> p:*(this->var2val)){
      if(p.second->getDirect()=='1'){
        if(p.second->getChanged()){
          if(!p.second->getSent()){
            //cout<<"location:" + p.second->getLocation() + "vl to send:" + to_string(p.second->getValue())<<endl;
            this->send(p.second->getLocation(),p.second->getValue());
            p.second->setSentTrue();
            //cout<<"send "<<p.first<<" "<<p.second->getValue()<<endl;
          }
        }
      }
    }

    mutex_lock1.unlock();
    this_thread::sleep_for(std::chrono::milliseconds(100));
    mutex_lock4.lock();
    if(endOfProgram){
      mutex_lock4.unlock();
      break;
    }
    mutex_lock4.unlock();
  }
  //close(this->client_socket);
  mutex_lock6.lock();
  ack2=true;
  mutex_lock6.unlock();
}
int ConnectCommand::execute(string *str, int pos) {
  // No socket for our client
  if (client_socket == -1) {
    throw ("Could not create a socket for control client");
  }
  int port;
  if(needsimplify(str,pos+2)){
    port=simplify(str,pos+2);
  }
  else{
    port=stoi(str[pos+2].substr(1,str[pos+2].length()-2));
  }
  //port=simplify(str,pos+2);
  std::thread connection(&ConnectCommand::ConnectControlClient,this,str, pos,port);
  connection.detach();
  return progress(str,pos);

}
int ConnectCommand::send(string str, double val) {
  string msg = "set " + str +" "+to_string(val)+"\r\n";
  int is_send =::send(client_socket, msg.c_str(), msg.length(), 0);//$$$ remember to check if ==-1 from call-code $$$
  return is_send;
}
// Destructor
ConnectCommand::~ConnectCommand() {
  close(this->client_socket);
}

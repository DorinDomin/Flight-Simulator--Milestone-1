#ifndef EX3__OPENSERVERCOMMAND_H_
#define EX3__OPENSERVERCOMMAND_H_
#include <string>
#include <thread>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <map>
#include "MapObj.h"
#include "Command.h"

using namespace std;
class OpenServerCommand:public Command{
 public:
  static void serverListen(map<string,VarData*>* var2val,int);
  OpenServerCommand(map<string,VarData*>*m1){
    this->var2val=m1;
  }
  int execute(string* s,int i) override {
    //Gets the port
    int port;
    if(needsimplify(s,i+1)){
      port=simplify(s,i+1);
    }
    else{
      port=stoi(s[i+1].substr(1,s[i+1].length()-2));
    }
    //Creates a thread for the server and detaches it
    thread serverThread(OpenServerCommand::serverListen,var2val,port);
    serverThread.detach();
    return progress(s,i);
  }
};

void OpenServerCommand::serverListen(map<string,VarData*>* var2val,int port) {
  //cout<<"beginning server"<<endl;
  //Creates Socket
  int sockfd=0;
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd == -1) {
    cout << "1"<<endl;
    throw ("Error creating socket");
  }
  sockaddr_in address;
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(port);
  //Binds socket to ip and port
  while(true){
    if (bind(sockfd, (struct sockaddr *) &address, sizeof(address)) == -1) {
      cout << "Could not bind socket to ip" << endl;
      cout << port << endl;
    }
    else{
      break;
    }
  }
  //Starts Listening
  if (listen(sockfd, SOMAXCONN) == -1) {
    cout << "3"<<endl;
    throw ("Error during listening command");
  }
  //cout<<"opening server"<<endl;
  int client_socket=0;
  //memset(&address, 0, sizeof(struct sockaddr_in));
  //Socket Accepts Client
  client_socket = accept(sockfd, (struct sockaddr *) &address, (socklen_t *) &address);
  if (client_socket == -1) {
    cout << "4"<<endl;
    throw ("Error accepting client");
  }
  mutex_lock2.lock();
  connectionAchieved=true;
  mutex_lock2.unlock();
  //cout<<"opened server"<<endl;
  //Creates index map with all of the paths
  map<int,string>indexmap;
  indexmap.insert(pair<int,string>(0,"/instrumentation/airspeed-indicator/indicated-speed-kt"));
  indexmap.insert(pair<int,string>(1,"/sim/time/warp"));
  indexmap.insert(pair<int,string>(2,"/controls/switches/magnetos"));
  indexmap.insert(pair<int,string>(3,"/instrumentation/heading-indicator/offset-deg"));
  indexmap.insert(pair<int,string>(4,"/instrumentation/altimeter/indicated-altitude-ft"));
  indexmap.insert(pair<int,string>(5,"/instrumentation/altimeter/pressure-alt-ft"));
  indexmap.insert(pair<int,string>(6,"/instrumentation/attitude-indicator/indicated-pitch-deg"));
  indexmap.insert(pair<int,string>(7,"/instrumentation/attitude-indicator/indicated-roll-deg"));
  indexmap.insert(pair<int,string>(8,"/instrumentation/attitude-indicator/internal-pitch-deg"));
  indexmap.insert(pair<int,string>(9,"/instrumentation/attitude-indicator/internal-roll-deg"));
  indexmap.insert(pair<int,string>(10,"/instrumentation/encoder/indicated-altitude-ft"));
  indexmap.insert(pair<int,string>(11,"/instrumentation/encoder/pressure-alt-ft"));
  indexmap.insert(pair<int,string>(12,"/instrumentation/gps/indicated-altitude-ft"));
  indexmap.insert(pair<int,string>(13,"/instrumentation/gps/indicated-ground-speed-kt"));
  indexmap.insert(pair<int,string>(14,"/instrumentation/gps/indicated-vertical-speed"));
  indexmap.insert(pair<int,string>(15,"/instrumentation/heading-indicator/indicated-heading-deg"));
  indexmap.insert(pair<int,string>(16,"/instrumentation/magnetic-compass/indicated-heading-deg"));
  indexmap.insert(pair<int,string>(17,"/instrumentation/slip-skid-ball/indicated-slip-skid"));
  indexmap.insert(pair<int,string>(18,"/instrumentation/turn-indicator/indicated-turn-rate"));
  indexmap.insert(pair<int,string>(19,"/instrumentation/vertical-speed-indicator/indicated-speed-fpm"));
  indexmap.insert(pair<int,string>(20,"/controls/flight/aileron"));
  indexmap.insert(pair<int,string>(21,"/controls/flight/elevator"));
  indexmap.insert(pair<int,string>(22,"/controls/flight/rudder"));
  indexmap.insert(pair<int,string>(23,"/controls/flight/flaps"));
  indexmap.insert(pair<int,string>(24,"/controls/engines/engine/throttle"));
  indexmap.insert(pair<int,string>(25,"/controls/engines/current-engine/throttle"));
  indexmap.insert(pair<int,string>(26,"/controls/switches/master-avionics"));
  indexmap.insert(pair<int,string>(27,"/controls/switches/starter"));
  indexmap.insert(pair<int,string>(28,"/engines/active-engine/auto-start"));
  indexmap.insert(pair<int,string>(29,"/controls/flight/speedbrake"));
  indexmap.insert(pair<int,string>(30,"/sim/model/c172p/brake-parking"));
  indexmap.insert(pair<int,string>(31,"/controls/engines/engine/primer"));
  indexmap.insert(pair<int,string>(32,"/controls/engines/current-engine/mixture"));
  indexmap.insert(pair<int,string>(33,"/controls/switches/master-bat"));
  indexmap.insert(pair<int,string>(34,"/controls/switches/master-alt"));
  indexmap.insert(pair<int,string>(35,"/engines/engine/rpm"));

  string buffer = "";
  while (true) {
    mutex_lock1.lock();
    char c=0;
    //Reads until '/n'
    while(c!='\n'){
      read(client_socket,&c,1);
      buffer+=c;
    }
    buffer+='\n';
    //cout<<buffer<<endl;
    int k = 0;
    int counter = 0;
    string tempString = "";
    while (buffer[k] != '\n') {
      while (buffer[k] != '\n' && buffer[k] != ',') {
        tempString += buffer[k];
        k++;
      }
      float tempFloat = stof(tempString);
      for(pair<string, VarData*> p:*var2val){
        if(p.second->getDirect()=='0'){
          if(p.second->getLocation()==indexmap.at(counter)) {
            //Adds value to *var2val and updates interpreter map
            p.second->setValue(tempFloat);
            i1->setVariables(p.first + "=" + to_string(p.second->getValue()) + ";");
            //cout<<"recieved:"<<p.first<<"="<<p.second->getValue()<<endl;
          }
        }
      }
      k++;
      counter++;
      tempString = "";
    }
    mutex_lock1.unlock();
    buffer="";
    this_thread::sleep_for(std::chrono::milliseconds(100));
    //Checks if we reaches the end of the text file
    if(endOfProgram){
      mutex_lock4.unlock();
      break;
    }
    mutex_lock4.unlock();
  }
  //Closes sockets
  close(client_socket);
  close(sockfd);

  //Makes ack1 true;
  mutex_lock5.lock();
  ack1= true;
  mutex_lock5.unlock();
}
#endif

#include <fstream>
#include <iostream>
#include <list>
#include "Command.h"
#include "ex1.h"
#include "Parser.h"
#include "VarData.h"
using namespace std;

//Lexer Function
list<string> lexer(list<string> linesread) {
  list<string> afterlexer;
  unsigned int pos = 0;
  list<string>::iterator it = linesread.begin();
  while (pos < linesread.size()) {
    if ((*it)[0] == '\t') {
      *it = (*it).substr(1, (*it).length());
    }
    while ((*it)[0] == ' ') {
      *it = (*it).substr(1, (*it).length());
    }
    it++;
    pos++;
  }
  pos = 0;
  it = linesread.begin();
  while (pos < linesread.size()) {
    string curr = (*it);
    //If OpenServerCommmand
    if (curr.find("openDataServer") != curr.npos) {
      int i = curr.find('(');
      afterlexer.push_back((curr.substr(0, i)));
      afterlexer.push_back(curr.substr(i + 1, curr.length() - (i + 2)));
      //If ConnectClientCommand
    } else if (curr.find("connectControlClient") != curr.npos) {
      int i = curr.find('(');
      int j = curr.find(',');
      afterlexer.push_back((curr.substr(0, i)));
      afterlexer.push_back(curr.substr(i + 1, j - (i + 1)));
      afterlexer.push_back(curr.substr(j + 1, curr.length() - (j + 2)));
      //If VarCommand
    } else if ((*it).find("var") != curr.npos) {
      //sim
      if ((*it).find("sim") != curr.npos) {
        //<-
        if ((*it).find("<") != curr.npos) {
          for (unsigned int k = 0; k < curr.length(); k++) {
            if (curr[k] == '(' || curr[k] == ')') {
              curr[k] = ' ';
            }
          }
          afterlexer.push_back("var");
          int j = curr.find('<');
          int i = j - 1;
          unsigned int l;
          while (curr[i] == ' ') {
            i--;
          }
          l = i - 1;
          while (curr[l] != ' ') {
            l--;
          }
          l++;
          afterlexer.push_back((curr.substr(l, i - l + 1)));
          afterlexer.push_back("<-");

          i = j + 2;
          while (curr[i] == ' ') {
            i++;
          }
          l = i + 1;
          while (curr[l] != ' ') {
            l++;
          }
          l--;
          afterlexer.push_back((curr.substr(i, l - i + 1)));
          i = l + 1;
          while (curr[i] == ' ') {
            i++;
          }
          l = i + 1;
          while (l < curr.length() && curr[l] != ' ') {
            l++;
          }
          l--;
          afterlexer.push_back((curr.substr(i, l - i + 1)));
        }
          //->
        else {
          for (unsigned int k = 0; k < curr.length(); k++) {
            if (curr[k] == '(' || curr[k] == ')') {
              curr[k] = ' ';
            }
          }
          afterlexer.push_back("var");
          int j = curr.find('-');
          int i = j - 1;
          unsigned int l;
          while (curr[i] == ' ') {
            i--;
          }
          l = i - 1;
          while (curr[l] != ' ') {
            l--;
          }
          l++;
          afterlexer.push_back((curr.substr(l, i - l + 1)));
          afterlexer.push_back("->");

          i = j + 2;
          while (curr[i] == ' ') {
            i++;
          }
          l = i + 1;
          while (curr[l] != ' ') {
            l++;
          }
          l--;
          afterlexer.push_back((curr.substr(i, l - i + 1)));
          i = l + 1;
          while (curr[i] == ' ') {
            i++;
          }
          l = i + 1;
          while (l < curr.length() && curr[l] != ' ') {
            l++;
          }
          l--;
          afterlexer.push_back((curr.substr(i, l - i + 1)));
        }
      }
        //not sim
      else {
        afterlexer.push_back("var");
        int i = curr.find(" ");
        i++;
        int j = i;
        while (curr[j] != ' ' && curr[j] != '=')
          j++;
        afterlexer.push_back((curr.substr(i, j - (i))));
        j = curr.find("=");
        afterlexer.push_back("=");
        if (curr[j + 1] == ' ') {
          afterlexer.push_back((curr.substr(j + 2, curr.length())));
        } else {
          afterlexer.push_back((curr.substr(j + 1, curr.length())));
        }
      }
      //If Print Command
    } else if ((*it).find("Print") != curr.npos) {
      int i = curr.find('(');
      afterlexer.push_back((curr.substr(0, i)));
      afterlexer.push_back(curr.substr(i + 1, curr.length() - (i + 2)));
      //If Sleep Command
    } else if ((*it).find("Sleep") != curr.npos) {
      int i = curr.find('(');
      afterlexer.push_back((curr.substr(0, i)));
      afterlexer.push_back(curr.substr(i + 1, curr.length() - (i + 2)));
      //If Loop Command
    } else if ((*it).find("while") != curr.npos) {
      int op, size;
      if ((*it).find("==") != curr.npos) {
        op = (*it).find("==");
        size = 2;
      } else if ((*it).find("!=") != curr.npos) {
        op = (*it).find("!=");
        size = 2;
      } else if ((*it).find("<=") != curr.npos) {
        op = (*it).find("<=");
        size = 2;
      } else if ((*it).find(">=") != curr.npos) {
        op = (*it).find(">=");
        size = 2;
      } else if ((*it).find("<") != curr.npos) {
        op = (*it).find("<");
        size = 1;
      } else if ((*it).find(">") != curr.npos) {
        op = (*it).find(">");
        size = 1;
      }
      afterlexer.push_back("while");
      if (size == 1) {
        int i = op-1;
        while (curr[i]==' ') {
          i--;
        }
        int t=5;
        while(curr[t]==' '){
          t++;
        }
        afterlexer.push_back(curr.substr(t, i -t +1));
        afterlexer.push_back(curr.substr(op, 1));
        op++;
        while(curr[op]==' '){
          op++;
        }
        int j = curr.find('{', op);
        string temp=curr.substr(op, j-op+1);
        int p=temp.length()-1;
        while(temp[p]==' '||temp[p]=='{'){
          p--;
        }
        p++;
        afterlexer.push_back(temp.substr(0, p));
        afterlexer.push_back("{");
      }
      if (size == 2) {
        int i = op-1;
        while (curr[i]==' ') {
          i--;
        }
        int t=5;
        while(curr[t]==' '){
          t++;
        }
        afterlexer.push_back(curr.substr(t, i -t +1));
        afterlexer.push_back(curr.substr(op, 2));
        op+=2;
        while(curr[op]==' '){
          op++;
        }
        int j = curr.find('{', op);
        string temp=curr.substr(op, j-op+1);
        int p=temp.length()-1;
        while(temp[p]==' '||temp[p]=='{'){
          p--;
        }
        p++;
        afterlexer.push_back(temp.substr(0, p));
        afterlexer.push_back("{");
      }
      //If IfCommand
    } else if ((*it).find("if") != curr.npos) {
      int op, size;
      if ((*it).find("==") != curr.npos) {
        op = (*it).find("==");
        size = 2;
      } else if ((*it).find("!=") != curr.npos) {
        op = (*it).find("!=");
        size = 2;
      } else if ((*it).find("<=") != curr.npos) {
        op = (*it).find("<=");
        size = 2;
      } else if ((*it).find(">=") != curr.npos) {
        op = (*it).find(">=");
        size = 2;
      } else if ((*it).find("<") != curr.npos) {
        op = (*it).find("<");
        size = 1;
      } else if ((*it).find(">") != curr.npos) {
        op = (*it).find(">");
        size = 1;
      }
      afterlexer.push_back("if");
      if (size == 1) {
        int i = op-1;
        while (curr[i]==' ') {
          i--;
        }
        int t=2;
        while(curr[t]==' '){
          t++;
        }
        afterlexer.push_back(curr.substr(t, i -t +1));
        afterlexer.push_back(curr.substr(op, 1));
        op++;
        while(curr[op]==' '){
          op++;
        }
        int j = curr.find('{', op);
        string temp=curr.substr(op, j-op+1);
        int p=temp.length()-1;
        while(temp[p]==' '||temp[p]=='{'){
          p--;
        }
        p++;
        afterlexer.push_back(temp.substr(0, p));
        afterlexer.push_back("{");
      }
      if (size == 2) {
        int i = op-1;
        while (curr[i]==' ') {
          i--;
        }
        int t=2;
        while(curr[t]==' '){
          t++;
        }
        afterlexer.push_back(curr.substr(t, i -t +1));
        afterlexer.push_back(curr.substr(op, 2));
        op+=2;
        while(curr[op]==' '){
          op++;
        }
        int j = curr.find('{', op);
        string temp=curr.substr(op, j-op+1);
        int p=temp.length()-1;
        while(temp[p]==' '||temp[p]=='{'){
          p--;
        }
        p++;
        afterlexer.push_back(temp.substr(0, p));
        afterlexer.push_back("{");
      }
      //If line is just }
    } else if ((*it).find("}") != curr.npos) {
      afterlexer.push_back("}");
      //Otherwise
    } else {
      string curr2;
      for (unsigned int k = 0; k < curr.length(); k++) {
        if (curr[k] != ' ') {
          curr2 += curr[k];
        }
      }
      int i = curr2.find('=');
      afterlexer.push_back((curr.substr(0, i)));
      afterlexer.push_back("=");
      afterlexer.push_back(curr2.substr(i + 1, curr2.length() - (i + 1)));
    }
    afterlexer.push_back("$");
    it++;
    pos++;
  }

  return afterlexer;
}

using namespace std;
int main(int, char *argv[]) {
  string permline;
  list<string> linesread;
  ifstream file;
  file.open(argv[1], ios::binary);
  string line;
  //Opens text file
  if (file.is_open()) {
  } else {
    cout << "Unable to open file" << endl;
    return 1;
  }
  map<string, VarData *> var2val;
  //Reads line by line
  while (!file.eof()) {
    getline(file, line);
    if (line != "") {
      linesread.push_back(line);
    }
  }

  //Moving to afterlexer
  list<string> templist = lexer(linesread);
  string* afterlexer=new string[templist.size()+1];
  afterlexer[0] = to_string(templist.size());
  int i = 1;
  for (auto l:templist) {
    afterlexer[i] = l;
    i++;
  }

  //Parsing what we got back from the lexer
  Parser(afterlexer, &var2val);

  //Closes file
  file.close();

  //Checks if ack1 was true
  while (true) {
    mutex_lock5.lock();
    if (ack1) {
      mutex_lock5.unlock();
      break;
    }
    mutex_lock5.unlock();
    this_thread::sleep_for(chrono::milliseconds(100));
  }

  //Checks if ack2 was true
  while (true) {
    mutex_lock6.lock();
    if (ack2) {
      mutex_lock6.unlock();
      break;
    }
    mutex_lock6.unlock();
    this_thread::sleep_for(chrono::milliseconds(100));
  }
  //Deletes allocated memory
  delete i1;
  delete [] afterlexer;
  for (pair<string, VarData *> p:var2val){
    delete(var2val[p.first]);
  }
  return 100;
}

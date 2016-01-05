/*******************************************************************
 * Project 1 UNIX Shell and History Feature
 * CSC 716 Advanced Operating System
 * Created by Yu Tian on 10/2/15.
 * Copyright © 2015 Yu Tian. All rights reserved.
 * ****************************************************************/
#include <iostream>
#include <list>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include <sstream>

using namespace std;

bool background = false; // check ampersand (&) at end of string
vector<char*> callCommand(string input);

int main() {

  list<string> history;
  int count = 0;

  while (1) {

    string input;
    cout << "osh> ";
    getline(cin, input);

    //vector<char*> commandVector = callCommand(input);
    //char **command = & commandVector[0];
    string commandStr = input;

    if (input.at(0) == '!') {
      // execute most recent command
      if (input.at(1) == '!' && !history.empty()) {

        // call most recent command
        auto it = history.rbegin();
        // cout << "Most recent command is " << *it << endl;
        commandStr = *it;

        // execute specific number of command in history
      } else if ((input.at(1) - '0') <= count && (input.at(1) - '0') > 0 && !history.empty()) {
        int p = input.at(1) - '0' - 1;
        auto it = history.begin();
        while (p > 0) {
            ++it;
            p--;
        }
        // call the function string at *it
        cout << "The command is " << *it << endl;
        commandStr = *it;

      } else if (history.empty()) {
        cout << "No commands in history" << endl;
      } else if ((input.at(1) - '0' <= 0 || input.at(1) - '0' > count)) {
        cout << "No such command in history" << endl;
      }

    } else {
      // if number of commands in history is less than 10
      if (count < 10) {
        history.push_back(input);
        count++;
      // if number of commands in history is more than 10
      } else if (count == 10) {
        history.pop_front();
        history.push_back(input);
      }
    }

    // if the command is history, just replace the commands in vector
    if (commandStr == "history") {
      int temp = count;
      for (auto it = history.rbegin(); it != history.rend() && !it->empty(); ++it) {
        cout << temp << " " << *it << endl;
        temp--;
      }
    }

    // call the function to split string, prepare c-string for child process
    vector<char*> commandVector = callCommand(commandStr);
    char **command = & commandVector[0];

    // basic fork() and create child process to execute the command
    pid_t pid;
    pid = fork();

    if (pid < 0) { // fail to fork()
      cerr << "Fork Failed";
      return 1;
    }
    else if (pid == 0) { // child process
      execvp(command[0], command);
      exit(0);
    }
    else { // parent process
      if (!background) wait(NULL);
      cout << "Child Complete\n";
    }
  }
  return 0;
}

/*
 * Function to split command string
 */
vector<char*> callCommand(string input) {
  vector<char*> commandVector;
  vector<string> tokens;
  istringstream iss(input);

  copy(istream_iterator<string>(iss), istream_iterator<string>(),
      back_inserter(tokens));
  for (size_t i = 0; i < tokens.size(); ++i) {
    commandVector.push_back(const_cast<char*>(tokens[i].c_str()));
  }

  // Check &
  if (*commandVector.back() == '&') {
    background = true;
    commandVector.pop_back();
  }

  commandVector.push_back(NULL);
  return commandVector;
}


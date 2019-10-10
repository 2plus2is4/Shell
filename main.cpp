#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <bits/stdc++.h>
#include <sys/wait.h>

using namespace std;


ofstream logger("log.txt");


vector<string> splitInput(string str, vector<string> args, bool *bg) {

    string word = "";
    for (auto x : str) {
        if (x == ' ') {
            args.push_back(word);
            if (word == "&")
                *bg = true;
            word = "";
        } else {
            word = word + x;
        }
    }
    args.push_back(word);
    if (word == "&")
        *bg = true;
    return args;
}


void initLogger() {
    logger << "==============================/nLOGGER NEW SESSION/n==============================/n";
}

void log(string str) {
    logger << str << endl;
}

void closeLogger() {
    logger.close();
}


int main() {
    string input;
    int status;
    initLogger();
    while (true) {
        cout << "\033[1;32m[Shell]$\033[37m" << " ";
        getline(cin, input);
        vector<string> args;
        bool bg = false;
        args = splitInput(input, args, &bg);
        log("command: " + input);
        if (args[0] == "exit") {
            closeLogger();
            exit(0);
        }
        int pid = fork();
        if (pid == 0) {
            log("forked child process successfully");
            sleep(2);
            char *cmd = const_cast<char *>(args[0].c_str());
            char *argv[args.size() + 1];
            int i = 0;
            for (; i < args.size(); ++i) {
                argv[i] = const_cast<char *>(args[i].c_str());
            }
            argv[i] = NULL;
            execvp(cmd, argv);
            exit(0);
        } else if (pid < 0)
            cout << "Couldn't create process" << endl;
        else {
            if (!bg)
                wait(&status);
            log("child process terminated with status " + status);
        }
        continue;
    }
}
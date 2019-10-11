#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <bits/stdc++.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <time.h>

using namespace std;


ofstream logger("log.txt", ios_base::app);
time_t my_time = time(NULL);


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
    logger << "============================== LOGGER NEW SESSION ==============================\n";
}

void log(string str) {
    my_time = time(NULL);
    logger << ctime(&my_time) << "\t" << str << endl;
}

void closeLogger() {
    logger.close();
}

void SIGHANDLER(int signum) {
//    int p = wait(NULL);
//    cout << "singal detected from " << p << endl;
    log("background child process terminated");
}


int main() {
    string input;
    int status;
    initLogger();
    while (true) {
        sleep(1);
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
//            sleep(2);
            char *argv[args.size() + 1];
            int i = 0;
            for (; i < args.size(); ++i) {
                argv[i] = const_cast<char *>(args[i].c_str());
            }
            argv[i] = NULL;
            __pid_t pid_t = getpid();
            cout << "child pid: " << pid_t << endl;
            execvp(argv[0], argv);
//            exit(0);
        } else if (pid < 0)
            cout << "Couldn't create process" << endl;
        else {
            cout << "parent here," << endl;
            if (!bg) {
                cout << "parent waiting for child to die" << endl;
                int killed_pid = wait(&status);
                cout << "killed pid: " << killed_pid << endl;
                log("child process terminated with status " + status);
            } else {
                cout << "waiting for signal" << endl;
                signal(SIGCHLD, SIGHANDLER);
            }
        }
        continue;
    }
}
#include "func.h"

#include <iostream>
#include <signal.h>

using namespace std;

int main() {

	init();

	cout << "Garbage collector is running now." << endl;

	while(true) {
		vector<string> users = list();
		
		cout << "======= GC PID " << getpid() << " =======" << endl;

		int pid;
		for(int i = 0; i < users.size(); i++) {
			pid = name2pid(users[i]); 
			cout << users[i] << " <" << pid << "> - ";
			
			
			if(kill(pid, SIGCHLD) == 0) {
				cout << "online." << endl;
			} else {
				deleteUser(pid);
				cout << "killed." << endl;
			}
		}


		sleep(5);
	}

	return 0;
}


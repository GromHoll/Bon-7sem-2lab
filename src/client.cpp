#include "func.h"

const char help[] = "Hotkeys list:\n"
		    "[S] - Send to <user>.\n"
		    "[A] - Sent to last user again.\n"
		    "[B] - Broadcast message.\n"
		    "[R] - Read new messages.\n"
		    "[L] - List of user.\n"
		    "[?] - Look this menu again.\n"
		    "[Q] - Exit.\n\n";
		    

int main(int argc, char * argv[]){
	string init_str = "Init messager`s settings";
	cout << init_str << " . . ." << endl;
	if(init() != 0){	// 0 - OK, -1 - exit
		cout << "Failure." << endl;
		cout << "Good bay." << endl; 
		exit(52);
	}
	cout << "Success." << endl << endl; 
	
	cout << endl;
	cout << "\tMessager v.2.3" << endl;
	cout << "\tPID : " << getpid() << endl << endl;
	
	
	string login_str =  "For exit enter <q>\nYour login please - ";
	string login = "Alex";
	
	bool thp = true;
	bool loop = true;
	while(loop){
		if(thp){
			cout << login_str;
			getline(cin, login);
		}
		cout <<  "Login user \"" << login << "\" . . ." << endl;
		switch(Login(login)){
			case SUCCESS:
				cout << "Success." << endl << endl; 
				loop = false;
				break;
			case INCORRECT_LOGIN:
				break;
			case EXIST_LOGIN:
				cout << "Your login exist in messager`s list. " << endl;
				if(!thp) loop = false;
				break;
			case LIST_IS_FULL:
				cout << "Sorry, messager`s list is full. Login later." << endl;
				cout << "Good bay."<< endl;
				exit(LIST_IS_FULL);
			case EXIT_USER:
				cout << "Bay." << endl;
				return 0;
			default:
				cout << "Incorrect return Login." << endl;
				exit(44);
				
		}
	} 
	cout << help << endl;
	
	string cmd;
	string user_name;
	int user_pid;
	string user_msg;
	vector <pair< string, string > > iMsg;
	vector< string > iList;
	
	loop = true;
	while(loop){
		cout << "Enter your command." << endl;
		getline(cin, cmd);
		if(cmd.empty()){
			continue;
		}
		switch(cmd[0]){
			case 'S': case 's':
				cout << "Enter name user you have send: ";
				getline(cin, user_name);
				if(name2pid(user_name) < 0){
					cout << "User \"" << user_name << "\" not exist in messager`s list" << endl;
					break;
				}
				cout << "Enter message from \"" << user_name << "\"" << endl;
				getline(cin, user_msg);
				if(user_msg.empty()){
					cout << "Message is empty." << endl;
					break;
				}
				if(send(login, user_name, user_msg) != 0){
					cout << "Send error" << endl;
					break;
				}
				
				cout << "Success \"Send\"." << endl << endl;
				break;
				
			case 'A': case 'a':
				if(user_name.empty()){
					cout << "Not found previous send user" << endl;
					break;
				}
				cout << "Enter message from \"" << user_name << "\"" << endl;
				getline(cin, user_msg);
				
				if(user_msg.empty()){
					cout << "Message is empty." << endl;
					break;
				}
				if(send(login ,user_name, user_msg) != 0){
					cout << "Send error" << endl;
					break;
				}
				
				cout << "Success \"Again send\"." << endl << endl;
				break;
				
			case 'B': case 'b':
				cout << "Enter message from Broadcast" << endl;
				getline(cin, user_msg);
				
				if(user_msg.empty()){
					cout << "Message is empty." << endl;
					break;
				}
				if(broadcast(login, user_msg) != 0){
					cout << "Send error" << endl;
					break;
				}
				
				cout << "Success \"Broadcast\"." << endl << endl;
				break;
			case 'R': case 'r':
				iMsg = receive();
				if(iMsg.empty()){
					cout << "Not input messages." << endl;
					break;
				}
				
				for(int i=0; i<iMsg.size(); i++){
					cout << i << ") from: \"" << iMsg[i].first << "\"\tmessage: \"" << iMsg[i].second << "\"" << endl;
				}
				
				cout << "\nSuccess \"Read\"." << endl << endl;
				break;
			case 'L': case 'l':
				iList = list();
				if(iList.empty()){
					cout << "Empty messager`s list." << endl;
					break;
				}
				
				for(int i=0; i<iList.size(); i++){
					cout << i << ") \"" << iList[i] << "\"" << endl;
				}
				
				cout << "\nSuccess \"receive list users\"." << endl << endl;
				break;
			case 'Q': case 'q':
				deleteUser(getpid());
				cout << "Good bay." << endl;
				return 0;
			case '?':
				cout << help << endl;
				break;
			default:
				cout << "Unknown command." << endl;
				break;
		}
		
		
//		loop = false;
		
	}
	
	printf("@\n");
	return 0;
}
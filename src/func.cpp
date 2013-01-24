#include "func.h"

int shmID;
int msgID;
int semShmID;
int semMsgID;

struct sembuf lock[] = {{0,0,0}, {0,1,0}};
struct sembuf unlock = {0,-1,0};


typedef pair<int, char[NICKNAME_MAX_LENGHT]> UserType;
UserType *users;

int init(){
	int shmsize = sizeof(UserType);
	if((shmID = shmget(ftok(PATH, 'a'), shmsize, 0666 | IPC_CREAT)) < 0)
		return 1;

	if((users = (UserType*)shmat(shmID, 0, 0)) < 0)
		return 2;

	if((semShmID = semget(ftok(PATH, 'b'), 1, 0666 | IPC_CREAT)) < 0)
		return 3;

	if((msgID = msgget(ftok(PATH, 'c'), 0666 | IPC_CREAT)) < 0)
		return 4;

	if((semMsgID = semget(ftok(PATH, 'd'), 1, 0666 | IPC_CREAT)) < 0)
		return 5;	
	
	
	
	return 0;
}

void lookShmem(){
	UserType * cur = users;
	for(int i = 0; i < CLIENT_MAX_NUM; i++, cur++) {
		cout << i << " -> " << cur->first << " : " << cur->second <<  endl;
	}
}
int Login(string login){
	
	if(login.find(' ') != -1 || login.find('\t') != -1) {
		cout << "Incorrect login. Don't use spaces and tabs." << endl;
		return INCORRECT_LOGIN;
	}
	
	if(login.size() >= NICKNAME_MAX_LENGHT){
		cout << "Incorrect login. Max size "<< NICKNAME_MAX_LENGHT  << endl;
		return INCORRECT_LOGIN;
	}
	
	if(login.size()==1 && (login[0] == 'q' || login[0] == 'Q')){
		return EXIT_USER;
	}
	
	UserType * cur = users;
	semop(semShmID, lock, 2);

	for(int i = 0; i < CLIENT_MAX_NUM; i++, cur++) {
		if(login.compare(cur->second) == 0) {			
			semop(semShmID, &unlock, 1);
			return EXIST_LOGIN;
		}
	}
	
	cur = users;
	for(int i = 0; i < CLIENT_MAX_NUM; i++, cur++) {
		if(cur->first == 0) {
			strcpy(cur->second, login.c_str());
      			cur->first = getpid();			
			semop(semShmID, &unlock, 1);
			return SUCCESS;	
		}
	}
	
	semop(semShmID, &unlock, 1);
	return LIST_IS_FULL;
	
}

int name2pid(string name){
	int output;
	UserType * cur = users;
	semop(semShmID, lock, 2);
	
	for(int i = 0; i < CLIENT_MAX_NUM; i++, cur++) {
		if(name.compare(cur->second) == 0) {
			output = cur->first;
			semop(semShmID, &unlock, 1);
			return output;
		}
	}
	
	semop(semShmID, &unlock, 1);
	return -1;
	
}
int send(string from, string to, string msg_str){
	
	semop(semMsgID, lock, 2);
	Message msg;

	if((msg.toPID = name2pid(to)) != -1) {
		strcpy(msg.fromName, from.c_str());
		strcpy(msg.message, msg_str.c_str());
	}
	msgsnd(msgID, &msg, sizeof(Message) - sizeof(long), IPC_NOWAIT);	

	semop(semMsgID, &unlock, 1);
	return 0;
}

int broadcast(string name, string msg_str){
	vector< string > iList;
	iList = list();
	for(int i=0;i<iList.size();i++){
		if(name.compare(iList[i]) == 0) continue;
		send(name, iList[i], msg_str);
	}
	
	return 0;
}

vector< pair< string, string > > receive(){
	vector<pair<string, string > > res;
	pair<string, string> tmp;

	semop(semMsgID, lock, 2);
	while(true) {
		Message message;
		if(msgrcv(msgID, &message, sizeof(Message) - sizeof(long int), getpid(), IPC_NOWAIT) != -1){
			tmp.first = message.fromName;
			tmp.second = message.message;
			res.push_back(tmp);
		}else 
			break;
	}
	semop(semMsgID, &unlock, 1);

	return res;
}
	
vector< string > list(){
	vector< string > res;
	
	semop(semShmID, lock, 2);
	UserType * cur = users;
	for(int i = 0; i < CLIENT_MAX_NUM; i++, cur++) {
		if(cur->first != 0) {	
			res.push_back(cur->second);
		}
	}
	
	semop(semShmID, &unlock, 1);
	return res;
}

void deleteUser(int PID) {
	UserType * cur = users;

	semop(semShmID, lock, 2);
	for(int i = 0; i < CLIENT_MAX_NUM; i++, cur++) {
		if(cur->first == PID) {			
			strcpy(cur->second, "");
      			cur->first = 0;	
			semop(semShmID, &unlock, 1);
			return;
		}
	}
	semop(semShmID, &unlock, 1);
}

#ifndef __FUNC_H__
#define __FUNC_H__


#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string>
#include <cstring>
#include <vector>
#include <utility>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/msg.h>

#define PATH "./obj/client.o"
#define NICKNAME_MAX_LENGHT 32
#define MESSAGE_MAX_LENGHT 256

#define CLIENT_MAX_NUM 20
#define SHMSIZE sizeof(int)*NICKNAME_MAX_LENGHT*CLIENT_MAX_NUM

using namespace std;

enum LOGIN_RETURNS{
	SUCCESS = 0,
	EXIST_LOGIN = 1,
	LIST_IS_FULL = 2,
	INCORRECT_LOGIN = 3,
	EXIT_USER = 4
};


struct Message {
	long int toPID;
	char fromName[NICKNAME_MAX_LENGHT];
	char message[MESSAGE_MAX_LENGHT];
};


int init();
int Login(string login);
int check(int ret, string msg);
int name2pid(string a);
int send(string from, string to, string msg);
int broadcast(string login, string msg);
vector< pair< string, string > > receive();
vector< string > list();
void deleteUser(int PID);

void lookShmem();


#endif//__FUNC_H__

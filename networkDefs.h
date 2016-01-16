#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PORT_NUMBER 60002

void setupSocket(int*, struct sockaddr_in*, char*, char*);
void bindSocket(int*, struct sockaddr_in*);
void waitForConnection(int*, struct sockaddr_in*, int*, struct sockaddr_in*);
void connectAsClient(int*, struct sockaddr_in*);
void leaveError(int);
void establishConnection(char*, int*, char*);


#include "networkDefs.h"

int main(int argc, char *argv[]) {
	int replay = 1;
	//Needed to allocate dynamic memory since the client may change to the server at some point
	char* type = malloc(sizeof(char));

	//No ip address passed in, so setup game as server
	if (argc == 1) {
		strcpy(type, "server");
		establishConnection(type, &replay, ""); 
	}
	//Ip address passed in, so try to connect to it as client
	else if (argc == 2) {
		strcpy(type, "client");
		establishConnection(type, &replay, argv[1]);
	}
	//Free allocated memory
	free(type);
	return 0;
}

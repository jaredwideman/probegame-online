#include "networkDefs.h"

//Global variable
int myListenSocket;

//Sets up a socket for either the client or the server
void setupSocket(int *socketName, struct sockaddr_in *addr, char *type, char *ipAddress) {
	//Create the socket
	*socketName = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (*socketName < 0) {
		fprintf(stderr, "Could not open socket.\n");
		exit(-1);
	}
	
	//Setup server address
	memset(addr, 0, sizeof(*addr));
	addr->sin_family = AF_INET;
	addr->sin_port = htons((unsigned short) PORT_NUMBER);

	//Ensure server listens to any address
	if (strcmp(type, "server") == 0) {
		addr->sin_addr.s_addr = htonl(INADDR_ANY);
	}
	//Ensure client is connecting to the server's ip
	if (strcmp(type, "client") == 0)
		addr->sin_addr.s_addr = inet_addr(ipAddress);
}

//Binds the server's socket
void bindSocket(int *socketName, struct sockaddr_in *addr) {
	int i;
	//Bind socket
	i = bind(*socketName, (struct sockaddr*) addr, sizeof(*addr));
	if (i < 0) {
		fprintf(stderr, "Could not bind socket.\n");
		exit(-1);
	}
}

//Allows the server to wait for an incoming connection
void waitForConnection(int *socketName, struct sockaddr_in *addr, int *clientSocket, struct sockaddr_in *clientAddr) {
	int i;
	fprintf(stdout, "Waiting for someone to connect...\n");
	//Listen
	i = listen(*socketName, 5);
	if (i < 0) {
		fprintf(stderr, "Could not listen.\n");
		exit(-1);
	}
	
	int addrSize;
	//Wait for Connection Request
	addrSize = sizeof(*clientAddr);
	*clientSocket = accept(*socketName, (struct sockaddr*) clientAddr, &addrSize);
	if (*clientSocket < 0) {
		fprintf(stderr, "Could not accept connection.\n");
		exit(-1);
	}
	fprintf(stdout, "Someone connected! Let's go.\n");
}

//Allows the client to connect
void connectAsClient(int *socketName, struct sockaddr_in *addr) {
	int i;
	//Connect to Server
	i = connect(*socketName, (struct sockaddr*) addr, sizeof(*addr));
	if (i < 0) {
		fprintf(stderr, "Client could not connect.\n");
		exit(-1);
	}
}

//New SIGINT message
void leaveError(int x) {
	fprintf(stdout, "\nSorry, as a prisonerrrr- I mean as a game player, you are not allowed to leave.\n");
}

//Begins the connection
void establishConnection(char *type, int *replay, char *ip) {
	//Ensure players cannot leave
	signal(SIGINT, leaveError);
	
	//Server variables
	struct sockaddr_in dataAddr;
	int clientSocket;
	struct sockaddr_in myAddr;
	int i, addrSize;
	char buffer[50];
	
	//Game variables
	char o_phrase[40];
	char my_phrase[40];
	char h_o_phrase[40];
	char h_my_phrase[40];
	char board[1000];
	board[0] = '\0';
	char guess[80];
	guess[0] = '\0';
	int bytesRcv;
	int win = 0;
	

	//Server connect
	if (strcmp(type, "server") == 0) {
		setupSocket(&myListenSocket, &myAddr, type, "");
		bindSocket(&myListenSocket, &myAddr);
		waitForConnection(&myListenSocket, &myAddr, &clientSocket, &dataAddr); //fix
	}
	
	//Client connect
	if (strcmp(type, "client") == 0) {
		setupSocket(&clientSocket, &dataAddr, type, ip);
		connectAsClient(&clientSocket, &dataAddr);
	}
	
	//Loop while the game is supposed to be played again
	while (*replay == 1) {
		//Creates the player's phrase, hides it and stores the unhidden phrase in the buffer
		game_createPhrase(my_phrase);
		game_toHidden(my_phrase, h_my_phrase);
		strcpy(buffer, my_phrase);
	
		//Send the phrase to the other player and wait for them to send back
		send(clientSocket, buffer, strlen(buffer), 0);
		fprintf(stdout, "Waiting for your opponent to create their secret phrase...\n");

		//Store what was sent back in the opponent's phrase variable
		bytesRcv = recv(clientSocket, buffer, sizeof(buffer), 0);
		buffer[bytesRcv] = '\0';
		strcpy(o_phrase, buffer);
		
		//Hide their phrase and create the board to print on screen
		game_toHidden(o_phrase, h_o_phrase);
		game_makeBoard(h_my_phrase, h_o_phrase, board);
		
		//Show the player their own phrase
		fprintf(stdout, "Your phrase: %s\n\n", my_phrase);
		
		//Counter to make the client go first initially
		i = 0;
	
		//Game loop
		while (1) {
			//Ensures that the client goes first
			if (i > 0 || strcmp(type, "server") == 0) {
				//Wait for opponent to guess and store guess in buffer
				fprintf(stdout, "Waiting for your opponent's guess...\n");
				bytesRcv = recv(clientSocket, buffer, sizeof(buffer), 0);
				buffer[bytesRcv] = '\0';
				strcpy(guess, buffer);
				
				//Compare guess with the actual phrase and check for win condition (1 = win, 0 = no win)
				game_guess(guess, my_phrase, h_my_phrase, &win);
				if (win == 1) {
					win = 0;
					//Wait to see if the opponent wants to play again
					fprintf(stdout, "Your opponent has won. Please wait while he decides he wants to play again...\n");
					bytesRcv = recv(clientSocket, buffer, sizeof(buffer), 0);
					buffer[bytesRcv] = '\0';
					
					
					//If opponent sends back "left", we need to do some things depending on if the opponent was the host or not
					if (strcmp(buffer, "left") == 0) {
						fprintf(stdout, "Your opponent left!\n");
						
						//If you were the server and lost
						if (strcmp(type, "server") == 0) {
							//Re-setup the socket and listen for a new connection
							setupSocket(&clientSocket, &myAddr, type, "");
							waitForConnection(&myListenSocket, &myAddr, &clientSocket, &dataAddr);
							i = 0;
							break;
						}
						//Close the client socket, change player type to server and listen for a new connection
						else {
							close(clientSocket);
							strcpy(type, "server");
							setupSocket(&myListenSocket, &dataAddr, type, "");
							waitForConnection(&clientSocket, &dataAddr, &myListenSocket, &myAddr); //TODO fix
							break;
						}
						*replay = -1;
						return;
					}
					//If they didn't send back "left", then they want to play again
					//So simply break out of the game loop to start a new game with new phrases
					else {
						system("clear");
						fprintf(stdout, "Your opponent wants to play again, let's go!\n");
						break;
					}
				}
				//Update the game board
				game_makeBoard(h_my_phrase, h_o_phrase, board);
				fprintf(stdout, "Your phrase: %s\n\n", my_phrase);
				fprintf(stdout, "Your opponent guessed: %s\n", guess);
			}
		
			//Ask for user input
			fprintf(stdout, "Guess a letter or the entire phrase.\n");
			fgets(guess, sizeof(guess), stdin);
			guess[strlen(guess)-1] = '\0';
			
			//Compare guess with the actual phrase and check for win condition (1 = win, 0 = no win), then update the game baord
			game_guess(guess, o_phrase, h_o_phrase, &win);
			game_makeBoard(h_my_phrase, h_o_phrase, board);
			fprintf(stdout, "Your phrase: %s\n\n", my_phrase);
	
			//Send the opponent what the user guessed
			strcpy(buffer, guess);
			send(clientSocket, buffer, strlen(buffer), 0);
			
			//If the user won, then see if they want to play again
			if (win == 1) {
				game_playAgain(replay);
				//Wants to play again
				if (*replay == 1) {
					strcpy(buffer, "stay");
					send(clientSocket, buffer, strlen(buffer), 0);
					win = 0;
					break;
				}
				//Doesn't want to play again
				else {
					strcpy(buffer, "left");
					send(clientSocket, buffer, strlen(buffer), 0);
					//Reset SIGINT to its default functionality
					signal(SIGINT, SIG_DFL);
					return;
				}
			}
			//Increment i allowing for the server to have a guess
			i++;
		}
		
	}
	//Close the final listen socket (never occurs)
	close(myListenSocket);
}


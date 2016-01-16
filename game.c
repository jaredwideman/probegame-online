#include "gameDefs.h"

//Creates a simple game board and prints it to the screen
void game_makeBoard(char *c_phrase, char *s_phrase, char *board) {
	system("clear");
	board[0] = '\0';
	strcat(board, "----------------------------------\nYour Phrase:\t\t");
	strcat(board, c_phrase);
	strcat(board, "\nOpponent's Phrase:\t");
	strcat(board, s_phrase);
	strcat(board, "\n----------------------------------\n");
	fprintf(stdout, "%s\n", board);
}

//Converts all non-spaces to astericks
void game_toHidden(char *phrase, char *out) {
	int i;
	strcpy(out, phrase);
	for (i = 0; i < strlen(out); i++) {
		if (out[i] != ' ') {
			out[i] = '*';
		}
	}
}

//Prompts user to input a phrase
void game_createPhrase(char* phrase) {
	char anotherStr[80];
	printf("Please enter your phrase: \n");
	fgets(anotherStr, sizeof(anotherStr), stdin);
	sscanf(anotherStr, "%[^\n]", phrase);
	phrase[strlen(phrase)] = '\0';
}

//Checks either full phrase guesses or single letter guesses and outputs the win condition
void game_guess(char* guess, char* phrase, char* hidden, int *winCondition) {
	int i;
	if (strlen(guess)>1) {
		if (strcmp(guess, phrase) == 0) {
			strcpy(hidden, guess);
			*winCondition = 1;
		}
	}
	else {
		for (i = 0; i < strlen(phrase); i++) {
			if (tolower(phrase[i]) == tolower(guess[0])) {
				hidden[i] = phrase[i];
			}
		}
		if (strcmp(guess, phrase) == 0) {
			*winCondition = 1;
		}
	}
}

//Prompts user whether or not they would like to play again (after they have won)
void game_playAgain(int *replay) {
	char option[20] = "";
	int correctOutput = 0;
	fprintf(stdout, "You win. Congratulations, you are freed from the game curse! You don't HAVE to leave though.\n\n");
	fprintf(stdout, "So. Would you like to stay as a prisoner- errr, play again (yes/no)?\n");
	
	do {
		fgets(option, sizeof(option), stdin);
		if (strstr(option, "y") != NULL || strstr(option, "Y") != NULL) {
			system("clear");
			fprintf(stdout, "Yo-you do? Alright, back in your cell- I mean back to the game with your old partner!\n");
			++correctOutput;
			*replay = 1;
		}
		else if (strstr(option, "n") != NULL || strstr(option, "N") != NULL) {
			fprintf(stdout, "Alright you're free to go\n");
			++correctOutput;
			*replay = 0;
		}
		else {
			fprintf(stdout, "Sorry, I didn't get that. Do you want to be a PRISONER here for any longer (yes/no)?\n");
		}
	} while (correctOutput == 0);
	
}


----------------------
| probegame - online |
----------------------
Description:	A two-player game similar to Probe written in C. Each player will be prompted to enter a phrase that
				the other player will try to guess, one letter at a time. Players each will take a turn trying to guess either one letter
				from the other player’s word, or the entire word. The first player to guess the opponent's word wins.
Author:			Jared Wideman
Website:		iamja.red

Use make command in unix OS to compile the program. 
To run the game, one player must create a server that the 2 players play on with './play'.
The second player then joins with './play <serverip>' to connect.

The losing player is essentially trapped playing the game forever making this a very high stakes game.


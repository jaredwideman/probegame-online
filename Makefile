a5:					game.o network.o main.o
						gcc -o play game.o network.o main.o
          
a5.o:				game.c network.c main.c
						gcc -c game.c network.c main.c

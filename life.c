#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//[ROW Y][COL X]

#define MIN_NEIGHBOURS 2
#define MAX_NEIGHBOURS 3
#define REPRODUCE 3

//This leaks memory like a sieve


//Add in board edges
void setup_board(char** board, int x, int y) {

	for(int i = 0; i < x; i++) {
		board[0][i] = '_';
		board[y - 1][i] = '_';	
	
	}
	
	for(int i = 1; i < y; i++) {
		board[i][0] = '|';
		board[i][x - 1] = '|';
	}



}

//Create malloc 2d array and populate with blank squares
char** create_board(int x, int y) {
	char** board = (char **)malloc(y * sizeof(char*));
	for(int i = 0; i < y; i++) {
		
		board[i] = malloc(x * sizeof(char));
	}

	for (int i = 0; i < y; i++) {
		for(int j = 0; j < x; j++) {
			board[i][j] = '.';
		}
	}

	setup_board(board, x, y);

	return board;


}


void print_board(char** board, int x, int y) {

	for (int i = 0; i < y; i++) {
		for(int j = 0; j < x; j++) {
			printf("%c ", board[i][j]);
		}
		printf("\n");
	}
}


//Assumes there are always cells on all 4 sides so it never segfaults
int count_adjacent_x(char** gameboard, int xcoord, int ycoord) {
	
	int count = 0;
	for(int i = (ycoord - 1); i <= (ycoord + 1); i++) {
		for(int j = (xcoord - 1); j <= (xcoord + 1); j++) {
	//		printf("%c", gameboard[i][j]);
			if (gameboard[i][j] == 'X') {
				count++;
			}
		}
	//	printf("\n");
	}
	//Discount square being looked at
	if (gameboard[ycoord][xcoord] == 'X') {
		count--;
	}
	return count;

}

//Determine next state of cell based on number of neighbours
char update_cell(char** gameboard, int xcoord, int ycoord) {
	
	char new_cell_value = '.';
	
	int neighbours = count_adjacent_x(gameboard, xcoord, ycoord);

	if (neighbours < MIN_NEIGHBOURS || neighbours > MAX_NEIGHBOURS) {
		new_cell_value = '.';
	}

	if (neighbours >= MIN_NEIGHBOURS && neighbours <= MAX_NEIGHBOURS && gameboard[ycoord][xcoord] == 'X') {
		new_cell_value = 'X';
	}

	if (neighbours == REPRODUCE) {	
		new_cell_value = 'X';
	}

	//printf("%d %d count %d\n\n", xcoord, ycoord, neighbours);
	//printf("%c\n\n", new_cell_value);
	
	return new_cell_value;
	
}

//Update every cell on the board
char** update_board(char** gameboard, int x, int y) {
	
	char** nextstate = create_board(x, y);
	
	for(int i = 1; i < (y - 1); i++) {
		for(int j = 1; j < (x-1); j++) {
			nextstate[i][j] = update_cell(gameboard, j, i);
		}
	}

	free(gameboard);

	return nextstate;

}

//Manually set state of a particular row
char** set_row(char** gameboard, int row, char* initialCondition) {

	for(int i = 0; i < strlen(initialCondition); i++) {
		gameboard[row][i + 1] = initialCondition[i];
	}
	
	return gameboard;


}





void main (int argc, char** argv) {
	
	printf("Conway's Game of Life (standard rules)\n");

	int x, y;
	char** gameboard;
	char* buffer = malloc(sizeof(char) * 100);
	int currentgen = 0;	
	
	if (argc >= 3) {
		x = atoi(argv[1]) + 2;
		y = atoi(argv[2]) + 2;
		
	} else {
		x = 5;
		y = 5;
	}

	gameboard = create_board(x, y);

	//Get initial condition from user
	printf("Enter initial condition line by line. Input should be shorter than board width. Use 'n' to indicate an empty line\n");

	for (int i = 1; i < y; i++) {
		
		printf("Enter intial condition for row %d: >>>", i);
		scanf("%s", buffer);

		if (strcmp(buffer, "n") == 0) {
			continue; //No initial condition for that line
		} else {
			gameboard = set_row(gameboard, i, buffer);
		}
	}

	print_board(gameboard, x, y);	
	currentgen++;	

	int n;
	
	//main loop
	while (1) {
		printf("How many ticks?   ");
		
		scanf("%d", &n);

		if (n < 0) {
			return;
		}
		
		for (int i = 0; i < n; i++) {
			gameboard = update_board(gameboard, x, y);
			currentgen++;
		}
		
		printf("Generation %d\n", currentgen);
		print_board(gameboard, x, y);
	}

	
}

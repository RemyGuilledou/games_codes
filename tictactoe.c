#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <conio.h>
#include <Windows.h>

#define GRID_SIZE 3
#define GRID_X 10
#define GRID_Y 5

char grid[GRID_SIZE][GRID_SIZE];
char player = 'X';
char computer = 'O';
char current_player;
char full;
char winner;

void put_sign(int, int, char);
void computer_plays(void);
void draw_grid(int, int);
char check_victory(void);
char grid_full(void);
void gotoxy(int, int);
void textcolor(int);
void display_rules(void);
void init_game(void);


int main(void) {
	

	char play_again = 'y';
	int coord_x, coord_y;
	int valid;
	
	while (play_again == 'y') {
		
		init_game();
		do {
			

			gotoxy(0, 3);
			textcolor(15);
			if (current_player == player) {
				do {
					valid = 0;
					printf("Player choose, which coordinates to put 'X'? [x/y] > ");
					scanf_s("%d%d", &coord_x, &coord_y);
					rewind(stdin);
					gotoxy(0, 3);
					textcolor(0);
					printf("Player choose, which coordinates to put 'X'? [x/y] >        ");
					if (grid[coord_x][coord_y] == 'E') {
						valid = 1;
					}
				} while (valid == 0);
				put_sign(coord_x, coord_y, player);
			}
			else if (current_player == computer) {
				printf("Computer chose...");
				_getch();
				computer_plays();
				gotoxy(0, 3);
				textcolor(15);
				printf("Computer played");
				_getch();
				gotoxy(0, 3);
				textcolor(0);
				printf("Computer played");
			}

			//gotoxy(0, 3);
			//textcolor(0);
			//printf("Player choose, which coordinates to put 'X'? [x/y] >        ");

			if (current_player == player) {
				current_player = computer;
			}
			else {
				current_player = player;
			}
			winner = check_victory();
			full = grid_full();
		} while (winner == 'n' && full == 'n');

		gotoxy(0, 10);
		textcolor(15);
		if (full == 'n') {
			printf("The winner is %s!!!\n", (winner == player) ? "the player" : "the computer");
		}
		else {
			printf("Nobody won...\n");
		}
		printf("Do you want to play again? [y/n] > ");
		scanf_s("%c", &play_again, 1);
		rewind(stdin);
		system("cls");
	}
	gotoxy(0, 20);
	return 0;
}

void put_sign(int x, int y, char symbol) {
	grid[x][y] = symbol;
	textcolor(15);
	gotoxy(x*2 + GRID_X + 1, y + GRID_Y);
	printf("%c", symbol);
}

void computer_plays(void) {
	int has_chosen = 0;
	int x, y;
	while (!has_chosen) {
		
		for (int i = 0; i < GRID_SIZE; i++) {
			for (int j = 0; j < GRID_SIZE; j++) {
				
				if (grid[j][i] == computer) {
					if (grid[(j + 1) % 3][i] == computer && grid[(j + 2) % 3][i] != player) {
						x = (j + 2) % 3;
						y = i;
						break;
					}
					else if (grid[j][(i + 1) % 3] == computer && grid[j][(i + 2) % 3] != player) {
						x = j;
						y = (i + 2) % 3;
						break;
					}
					else if (grid[(j + 1) % 3][(i + 1) % 3] == computer && grid[(j + 2) % 3][(i + 2) % 3] != player) {
						x = (j + 2) % 3;
						y = (i + 2) % 3;
						break;
					}
					else if (grid[(j - 1) % 3][(i + 1) % 3] == computer && grid[(j - 2) % 3][(i + 2) % 3] != player) {
						x = (j - 2) % 3;
						y = (i + 2) % 3;
						break;
					}

				}
				else if (grid[j][i] == player) {
					if (grid[(j + 1) % 3][i] == player && grid[(j + 2) % 3][i] != computer) {
						x = (j + 2) % 3;
						y = i;
						break;
					}
					else if (grid[j][(i + 1) % 3] == player && grid[j][(i + 2) % 3] != computer) {
						x = j;
						y = (i + 2) % 3;
						break;
					}
					else if (grid[(j + 1) % 3][(i + 1) % 3] == player && grid[(j + 2) % 3][(i + 2) % 3] != computer) {
						x = (j + 2) % 3;
						y = (i + 2) % 3;
						break;
					}
					else if (grid[(j - 1)%3][(i + 1) % 3] == player && grid[(j - 2) % 3][(i + 2) % 3] != computer) {
						x = (j - 2) % 3;
						y = (i + 2) % 3;
						break;
					}
				}
				
				else {
					x = rand() % 3;
					y = rand() % 3;
				}
				
			}
		}	

		if (grid[x][y] != player && grid[x][y] != computer) {
			has_chosen = 1;
		}
		
	}
	put_sign(x, y, computer);
}

char grid_full(void) {
	char is_full = 'n';
	for (int i = 0; i < GRID_SIZE; i++) {
		for (int j = 0; j < GRID_SIZE; j++) {
			if (grid[j][i] != player || grid[j][i] != computer) {
				break;
			}
			else {
				is_full = 'y';
			}
		}
	}
	return is_full;
}

char check_victory(void) {
	char winner = 'n';
	for (int i = 0; i < GRID_SIZE; i++) {
		for (int j = 0; j < GRID_SIZE; j++) {
			if (grid[i][0] == player && grid[i][1] == player && grid[i][2] == player) {
				winner = player;
				break;
			}
			if (grid[0][j] == player && grid[1][j] == player && grid[2][j] == player) {
				winner = player;
				break;
			}
			if (grid[0][0] == player && grid[1][1] == player && grid[2][2] == player) {
				winner = player;
				break;
			}
			if (grid[2][0] == player && grid[1][1] == player && grid[0][2] == player) {
				winner = player;
				break;
			}
			if (grid[i][0] == computer && grid[i][1] == computer && grid[i][2] == computer) {
				winner = computer;
				break;
			}
			if (grid[0][j] == computer && grid[1][j] == computer && grid[2][j] == computer) {
				winner = computer;
				break;
			}
			if (grid[0][0] == computer && grid[1][1] == computer && grid[2][2] == computer) {
				winner = computer;
				break;
			}
			if (grid[2][0] == computer && grid[1][1] == computer && grid[0][2] == computer) {
				winner = computer;
				break;
			}
		}
	}
	return winner;
}

void draw_grid(int x, int y) {
	int i, j;
	for(i = y;  i< y+GRID_SIZE; i++){
		for (j = x; j <= x + (GRID_SIZE * 2); j +=2) {
			gotoxy(j, i);
			printf("|");
		}
		putchar('\n');
	}
}

void init_game(void) {
	gotoxy(10, 0);
	textcolor(15);
	printf("TIC TAC TOE");
	gotoxy(5, 1);
	printf("Press any key to start");
	gotoxy(5, 2);
	printf("Press r to see the rules");
	char res = _getch();
	if (res == 'r') {
		display_rules();
	}
	else {
		gotoxy(GRID_X, GRID_Y);
		textcolor(15);
		draw_grid(GRID_X, GRID_Y);
		current_player = ((rand() % 2 != 0) ? player : computer);
		for (int i = 0; i < GRID_SIZE; i++) {
			for (int j = 0; j < GRID_SIZE; j++) {
				grid[j][i] = 'E';
			}
		}
		winner = 'n';
		full = 'n';
		gotoxy(5, 1);
		textcolor(0);
		printf("Press any key to start");
		gotoxy(5, 2);
		textcolor(0);
		printf("Press r to see the rules");
	}
}

void display_rules(void) {
	system("cls");
	gotoxy(0, 0);
	textcolor(15);
	printf("\tRULES\n");
	printf("->each player put his symbol (X for the player, O for the computer) on the grid once per turn\n");
	printf("->to place a symbol choose the x and y coordinates of the location you aim, top left location is [0/0], bottom right is [2/2]\n");
	printf("->to win align three symbols in any direction\n");
	printf("[PRESS ANY KEY]");
	_getch();
	system("cls");
	init_game();
}

void textcolor(int color) {
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(h, color);
}

void gotoxy(int x, int y) {
	COORD c = { x, y };
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(h, c);
}
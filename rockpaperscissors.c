#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <Windows.h>

#define MAX_SCORE 5

enum hand{ ROCK, PAPER, SCISSORS };



void gotoxy(int, int);
void textcolor(int);
char choose_object(void);
char computer_choose(void);
char check_victory(char, char);
char play_again(char);
void introduction(void);
void see_rules(void);
void print_choices(char, char);

int main(void) {
	char player, computer;
	char winner;
	char play_game = 'y';
	int points_player, points_computer;
	srand(time(NULL));
	while (play_game == 'y') {
		points_player = 0;
		points_computer = 0;
		introduction();
		do {
			system("cls");
			gotoxy(0, 0);
			printf("PLAYER: %d/%d\n", points_player, MAX_SCORE);
			printf("COMPUTER: %d/%d\n", points_computer, MAX_SCORE);
			
			player = choose_object();
			
			computer = computer_choose();
			
			print_choices(player, computer);

			winner = check_victory(player, computer);
			gotoxy(0, 5);
			textcolor(15);
			if (winner == 'p') {
				printf("Player got the point\n");
				points_player++;
			}
			else if (winner == 'c') {
				printf("Computer got the point\n");
				points_computer++;
			}
			else if (winner == 'E') {
				printf("Draw, nobody got the point\n");
			}
			_getch();
		} while (points_player < MAX_SCORE && points_computer < MAX_SCORE);

		play_game = play_again(winner);
	}
	

	return 0;
}

void print_choices(char player, char computer) {
	gotoxy(0, 5);
	textcolor(15);
	printf("player = %c\n", player);
	printf("computer = %c\n", computer);
	_getch();
	gotoxy(0, 5);
	textcolor(0);
	printf("player = %c\n", player);
	printf("computer = %c\n", computer);
}

void introduction(void) {
	system("cls");
	char res;
	gotoxy(10, 0);
	textcolor(15);
	printf("ROCK PAPER SCISSORS");

	gotoxy(0, 4);
	printf("Press any key to play\n");
	printf("Press r to see the rules");
	res = _getch();
	if (res == 'r') {
		see_rules();
	}
}

void see_rules(void) {
	system("cls");
	gotoxy(0, 0);
	printf("-> Choose one between Rock, Paper, Scissor, by pressing r, p, s respectively\n");
	printf("-> Rock wins against Scissor, Paper wins againts Rock, Scissor wins against Paper\n");
	printf("-> Win %d confrontations to win the game\n", MAX_SCORE);
	printf("[PRESS ANY KEY TO CONTINUE]");
	_getch();
	introduction();
}

char choose_object(void) {
	char choice = 'E';

	while (choice != 'r' && choice != 'p' && choice != 's') {
		gotoxy(0, 5);
		textcolor(15);
		printf("Choose between Rock, paper, scissors: ");
		scanf_s("%c", &choice, 1);
		rewind(stdin);
		gotoxy(0, 5);
		textcolor(0);
		printf("Choose between Rock, paper, scissors:  ");
	}
	return choice;
}

char computer_choose(void) {
	char choice = 'E';
	int res, time_wait = 1000;
	res = rand() % 3;
	switch (res) {
	case ROCK:
		choice = 'r';
		break;
	case PAPER:
		choice = 'p';
		break;
	case SCISSORS:
		choice = 's';
		break;
	}
	gotoxy(0, 5);
	textcolor(15);
	printf("Computer choose");
	for (int i = 0; i < 3; i++) {
		putchar('.');
		int start = clock();
		while(clock() < start + time_wait){}
	}
	gotoxy(0, 5);
	textcolor(0);
	printf("Computer choose...");
	return choice;
}

char check_victory(char player, char computer) {
	if ((player == 'r' && computer == 's') || (player == 'p' && computer == 'r') || (player == 's' && computer == 'p')) {
		return 'p';
	}
	else if ((computer == 'r' && player == 's') || (computer == 'p' && player == 'r') || (computer == 's' && player == 'p')) {
		return 'c';
	}
	else {
		return 'E';
	}
}

char play_again(char winner) {
	
	char choice = 'E';
	gotoxy(0, 6);
	textcolor(15);
	if (winner == 'p') {
		printf("Player won!");
	}
	else if (winner == 'c') {
		printf("Computer won!");
	}
	_getch();
	system("cls");
	gotoxy(0, 5);
	printf("Do you want to play again? [y/n] > ");
	scanf_s("%c", &choice, 1);
	rewind(stdin);
	return choice;
}

void gotoxy(int x, int y) {
	COORD c = { x, y };
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(h, c);
}

void textcolor(int color) {
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(h, color);
}
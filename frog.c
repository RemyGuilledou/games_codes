#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <time.h>
#include <Windows.h>

#define MAX_WIDTH 60
#define MAX_HEIGHT 30
#define FROG 'F'
#define FROG_INITIAL_X MAX_WIDTH / 2
#define FROG_INITIAL_Y MAX_HEIGHT / 2 - 2
#define LEFT 0
#define RIGHT 1
#define YES 'y'
#define NO 'n'
#define MAX_LEVEL 5
#define CAR_NUMBER 3
#define LEVEL_WIDTH 30

typedef struct character {
	int x, y, color;
	char visual;
}CHARACTER;

typedef struct car {
	int x, y, dir, speed, row, time, color;
	char model;
}CAR;

typedef struct level {
	int nb_car, id, width;
}LEVEL;

CHARACTER frog;
LEVEL level;
CAR* all_cars = NULL;

char play_again(void);
void draw_screen(void);
void gotoxy(int, int);
void textcolor(int);
CAR init_car(int);
CAR* init_all_cars(int);
void move_cars(void); 
void vroom(CAR*);
int check_collision(void);
LEVEL init_level(int);
void display_level(void);
void newlevel(int);
void display_nblvl(int);
void introduction(void);
void see_rules(void);
int victory(void);

int main(void) {
	int end_game, current_level;
	char replay = YES;
	do {
		introduction();
		end_game = 1;
		current_level = 1;
		newlevel(current_level);
		while (end_game) {
			if (_kbhit()) {
				char res = _getch();
				gotoxy(frog.x, frog.y);
				putchar(' ');
				switch (res) {
				case 'w':
					if (frog.y >= FROG_INITIAL_Y)
						frog.y--;
					break;
				case 's':
					frog.y++;
					break;
				case 'a':
					if (frog.x > (MAX_WIDTH / 2 - (level.width) / 2) + 1)
						frog.x--;
					break;
				case 'd':
					if (frog.x < (MAX_WIDTH / 2 + (level.width) / 2) - 1)
						frog.x++;
					break;
				}
				gotoxy(frog.x, frog.y);
				textcolor(frog.color);
				putchar(frog.visual);
			
			}
			move_cars();
			end_game = check_collision();
			
			if (frog.y == (MAX_HEIGHT / 2) + level.nb_car) {
				if (current_level == MAX_LEVEL) {
					end_game = victory();
				}
				else {
					newlevel(++current_level);
				}
			}
		}
		system("cls");
		gotoxy(MAX_WIDTH/2, MAX_HEIGHT/2);
		textcolor(15);
		printf("GAME OVER");
		_getch();
		replay = play_again();
	} while (replay == YES);
	gotoxy(0, 30);
	return 0;
}


CAR init_car(int number) {
	CAR newcar;
	newcar.row = number;
	newcar.dir = (newcar.row % 2 == 0) ? LEFT : RIGHT;
	newcar.model = (newcar.dir == LEFT) ? '<' : '>';
	newcar.speed = 200 - (10 * number)-(10*level.id);
	newcar.x = (MAX_WIDTH / 2) - (level.width / 2) + (1+(rand() % (level.width-2 )));
	newcar.y = MAX_HEIGHT / 2 + newcar.row;
	newcar.time = clock();
	newcar.color = 1 + rand() % 15;
	return newcar;
}

CAR* init_all_cars(int nb_cars) {
	CAR* all_cars;
	all_cars = (CAR*)malloc(sizeof(CAR) * nb_cars);
	for (int i = 0; i < nb_cars; i++) {
		all_cars[i] = init_car(i);
		gotoxy(all_cars[i].x, all_cars[i].y);
		textcolor(all_cars[i].color);
		putchar(all_cars[i].model);
	}

	return all_cars;
}

int check_collision(void) {
	for (int i = 0; i < level.nb_car; i++) {
		if (frog.x == all_cars[i].x && frog.y == all_cars[i].y) {
			return 0;
		}
	}
	return 1;
}

void vroom(CAR* vehicule) {
	while (clock() > vehicule->speed + vehicule->time) {
		vehicule->time = clock();
		gotoxy(vehicule->x, vehicule->y);
		putchar(' ');
		if (vehicule->dir == 0 && vehicule->x == (MAX_WIDTH / 2 - (level.width) / 2) + 1) {
			vehicule->x = (MAX_WIDTH / 2 + (level.width) / 2) - 1;
		}
		else if (vehicule->dir == 1 && vehicule->x == (MAX_WIDTH / 2 + (level.width) / 2) - 1) {
			vehicule->x = (MAX_WIDTH / 2 - (level.width) / 2) + 1;
		}
		else {
			vehicule->x += (vehicule->dir == 0) ? -1 : 1;
		}
		gotoxy(vehicule->x, vehicule->y);
		textcolor(vehicule->color);
		putchar(vehicule->model);
	}
}

void move_cars(void) {
	for (int i = 0; i < level.nb_car; i++) {
		vroom(&all_cars[i]);
	}
}

void draw_screen(void) {
	textcolor(15);
	for (int i = 0; i < MAX_WIDTH; i++) {
		gotoxy(i, 0);
		putchar('_');
		gotoxy(i, MAX_HEIGHT);
		putchar('_');
	}
	for (int i = 1; i < MAX_HEIGHT; i++) {
		gotoxy(0, i);
		putchar('|');
		gotoxy(MAX_WIDTH, i);
		putchar('|');
	}
}



char play_again(void) {
	char answer = 'E';
	while (answer != YES && answer != NO) {
		system("cls");
		gotoxy(MAX_WIDTH / 2, MAX_HEIGHT / 2);
		textcolor(15);
		printf("Do you want to play again> [y/n] > ");
		scanf_s("%c", &answer, 1);
		rewind(stdin);
		if (answer == 'Y') {
			answer = YES;
		}
		if (answer == 'N') {
			answer = NO;
		}
	}
	return answer;
}

LEVEL init_level(int current_level) {
	LEVEL level;
	level.id = current_level;
	level.nb_car = CAR_NUMBER + level.id;
	level.width = LEVEL_WIDTH-(level.id*3);
	return level;
}

void display_level(void) {
	textcolor(15);
	for (int i = 0; i < level.nb_car; i++) {
		gotoxy(MAX_WIDTH / 2 - (level.width)/2, MAX_HEIGHT / 2 + i);
		putchar('|');
		gotoxy(MAX_WIDTH / 2 + (level.width) / 2, MAX_HEIGHT / 2 + i);
		putchar('|');
	}
}

void newlevel(int current_level) {
	system("cls");
	display_nblvl(current_level);
	frog.x = FROG_INITIAL_X;
	frog.y = FROG_INITIAL_Y;
	frog.color = 2;
	gotoxy(frog.x, frog.y);
	putchar(' ');
	gotoxy(frog.x, frog.y);
	frog.visual = FROG;
	textcolor(frog.color);
	putchar(frog.visual);
	level = init_level(current_level);
	all_cars = init_all_cars(level.nb_car);
	display_level();
	draw_screen();
}

void display_nblvl(int level) {
	gotoxy(MAX_WIDTH / 2, MAX_HEIGHT / 2);
	textcolor(15);
	printf("LEVEL %d", level);
	_getch();
	system("cls");
}

void introduction(void) {
	system("cls");
	char res;
	gotoxy(MAX_WIDTH / 2, MAX_HEIGHT / 2);
	textcolor(15);
	printf("FROGGER");
	gotoxy(MAX_WIDTH / 2-10, MAX_HEIGHT / 2+2);
	printf("Press any key to play, press r to see the rules");
	res = _getch();
	if (res == 'r') {
		see_rules();
	}
}

void see_rules(void) {
	system("cls");
	gotoxy(0, MAX_HEIGHT / 2);
	textcolor(15);
	printf("-> You are a frog that try to cross the road\n");
	printf("-> Press wasd keys to move\n");
	printf("-> Reach the bottom of the screen to go to the next level\n");
	printf("-> If you hit a car you lose\n");
	printf("[PRESS ANY KEY TO CONTINUE]");
	_getch();
	introduction();
}

int victory(void) {
	system("cls");
	gotoxy(MAX_WIDTH / 2, MAX_HEIGHT / 2);
	textcolor(15);
	printf("YOU WON!");
	_getch();
	return 0;
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
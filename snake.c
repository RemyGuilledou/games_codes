#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <Windows.h>

// CONSTANTS TIME-----------------------------
#define TPS 150

//MAP DATA-----------------------------------
#define MAP_WIDTH 40
#define MAP_HEIGHT 20

//SNAKE LENGTH------------------------------
#define INITIAL_LENGTH 5
#define MAX_LENGTH (MAP_WIDTH-2)*(MAP_HEIGHT-2)

//DIRECTIONS------------------------------
#define UP 0
#define RIGHT 1
#define DOWN 2
#define LEFT 3


//STRUCTURES-----------------------------
typedef struct body {
	int x, y; // Coordinates
	char body_part; // character drawn during the game
	int dir; // direction
}BODY;

typedef struct food {
	int x, y; // coordinates
}FOOD;

// GLOBAL VARIABLES-----------------------------
int snake_length; 
int game_over; 
int score; 
BODY* snake; 
FOOD food; 




//FUNCTION DECLARATIONS----------------------------------
void display_map(void);
void gotoxy(int, int);
void init_snake(void);
void move_snake(void);
void init_food(void);
char end_game(void);
void add_body_part(void);
int check_collision(void);
int change_dir(void);
void update(void);
void init_game(void);
void print_rules();



int main(void) {
	char replay = 'n';
	int change_direction;
	
	do {
		init_game();
		while (!game_over) {
			change_direction = 0;
			int start_game = clock();
			while (clock() < start_game + (TPS-score)) {}
			
			change_direction = change_dir();
			if (!change_direction) {
				move_snake();
			}
			
			update();
			game_over = check_collision();
		}
		replay = end_game();

	} while (replay == 'y' || replay == 'Y');
	

	gotoxy(0, MAP_HEIGHT + 5);
	return 0;
}

void init_snake(void) { // creation of the snake at the beginning of the game
	snake = (BODY*)malloc(sizeof(BODY)*MAX_LENGTH);
	snake[0].body_part = 'M';
	snake[0].x = MAP_WIDTH / 2;
	snake[0].y = MAP_HEIGHT / 2;
	snake[0].dir = UP;
	for (int i = 1; i < INITIAL_LENGTH; i++) {
		snake[i].body_part = 'O';
		snake[i].x = snake[0].x;
		snake[i].y = snake[0].y + i;
		snake[i].dir = snake[i-1].dir;
	}
	for (int i = 0; i < INITIAL_LENGTH; i++) {
		gotoxy(snake[i].x, snake[i].y);
		printf("%c", snake[i].body_part);
	}
}

void display_map(void) { // creation of the grid where the game plays
	int horizontal, vertical;
	for (horizontal = 0; horizontal <= MAP_WIDTH; horizontal++) { // creation of the top and bottom lines
		gotoxy(horizontal, 0);
		printf("!");
		gotoxy(horizontal, MAP_HEIGHT);
		printf("!");
	}
	for (vertical = 0; vertical <= MAP_HEIGHT; vertical++) { // creation of the left and right lines
		gotoxy(0, vertical);
		printf("!");
		gotoxy(MAP_WIDTH, vertical);
		printf("!");
	}
}

void move_snake(void) {
	gotoxy(snake[snake_length - 1].x, snake[snake_length - 1].y); // erase the last body part before moving
	putchar(' ');

	for(int i = snake_length; i >= 0; i--){ // move every snake body part from the last to the first; so the latest copy the direction of the part it follows
		switch (snake[i].dir) {
		case UP:
			snake[i].y -= 1;
			break;
		case RIGHT:
			snake[i].x += 1;
			break;
		case DOWN:
			snake[i].y += 1;
			break;
		case LEFT:
			snake[i].x -= 1;
		}
		if(i != 0)
			snake[i].dir = snake[i-1].dir;
	}
	
	for (int i = 0; i < snake_length; i++) { // print every body part after the new coordinates have been computed
		gotoxy(snake[i].x, snake[i].y);
		printf("%c", snake[i].body_part);
	}
}

void init_food(void) { // generate food each time the snake got one
	int res = 0;
	while (!res) {
		res = 1;
		food.x = (rand() % (MAP_WIDTH-1))+1;
		food.y = (rand() % (MAP_HEIGHT-1))+1;
		for (int i = 0; i < snake_length; i++) {
			if (food.x == snake[i].x && food.y == snake[i].y) {
				res = 0;
				break;
			}
		}
	}
	
	gotoxy(food.x, food.y);
	putchar('F');
}

void add_body_part(void) { // after snake eats food add a body part at the end of its body
	snake_length += 1;
	switch (snake[snake_length - 2].dir) { // look at the coordinates of the body it follows to be adjusted accordingly
	case UP:
		snake[snake_length - 1].x = snake[snake_length - 2].x;
		snake[snake_length - 1].y = snake[snake_length - 2].y+1;
		break;
	case RIGHT:
		snake[snake_length - 1].x = snake[snake_length - 2].x-1;
		snake[snake_length - 1].y = snake[snake_length - 2].y;
		break;
	case DOWN:
		snake[snake_length - 1].x = snake[snake_length - 2].x;
		snake[snake_length - 1].y = snake[snake_length - 2].y-1;
		break;
	case LEFT:
		snake[snake_length - 1].x = snake[snake_length - 2].x+1;
		snake[snake_length - 1].y = snake[snake_length - 2].y;
		break;
	}
	snake[snake_length-1].body_part = 'O';
	snake[snake_length-1].dir = snake[snake_length - 2].dir; // takes the direction of the former last body part
}

int check_collision(void) { // check if snake hits the wall or itself
	int collision = 0;

	if (snake[0].x <= 0 || snake[0].x >= MAP_WIDTH || snake[0].y <= 0 || snake[0].y >= MAP_HEIGHT) { // the wall
		collision = 1;
	}
	for (int i = 1; i < snake_length; i++) { // itself
		if (snake[0].x == snake[i].x && snake[0].y == snake[i].y) {
			collision = 1;
		}
	}

	return collision; // return 1 if there is collision or 0 if no collision
}

int change_dir(void) { // change the direction that the head goes to
	int moving = 0;
	if (_kbhit()) {
		
		char key = _getch();
		switch (key) {
		case 'w': // direction changes according to the keyboard key press wasd
			if (snake[0].dir != DOWN && snake[0].dir !=UP) { // if the direction is not the expected one or the opposite one the direction changes
				snake[0].dir = UP;
				moving = 1; // if the direction changes return 1
			}
			if (snake[0].dir == UP) { // if the direction of the snake corresponds to the direction of the key pressed the snake moves faster towards that direction
				move_snake();
			}
			break;
		case 'd':
			if (snake[0].dir != LEFT && snake[0].dir != RIGHT) {
				snake[0].dir = RIGHT;
				moving = 1;
			}
			if (snake[0].dir == RIGHT) {
				move_snake();
			}
			break;
		case 's':
			if (snake[0].dir != UP && snake[0].dir != DOWN) {
				snake[0].dir = DOWN;
				moving = 1;
			}
			if (snake[0].dir == DOWN) {
				move_snake();
			}
			break;
		case 'a':
			if (snake[0].dir != RIGHT && snake[0].dir != LEFT) {
				snake[0].dir = LEFT;
				moving = 1;
			}
			if (snake[0].dir == LEFT) {
				move_snake();
			}
			break;
		default:
			break;
		}
		update(); // check if snake gets food
		
	}
	return moving;
}

void update() { // if snake gets food add another food on the map and add a body part to the snake
	if (snake[0].x == food.x && snake[0].y == food.y) {
		score += 1;
		gotoxy(MAP_WIDTH + 5, 0);
		printf("SCORE: %02d", score);
		init_food();
		add_body_part();
	}
}

void init_game(void) { // initialise the game with title screen and starting variables
	char key;
	display_map();
	gotoxy(MAP_WIDTH / 2 - 5, MAP_HEIGHT / 2);
	printf("SNAKE  GAME");
	_getch();
	gotoxy(MAP_WIDTH / 2 - 10, (MAP_HEIGHT / 2)+3);
	printf("Press any key to play");
	gotoxy(MAP_WIDTH / 2 - 10, (MAP_HEIGHT / 2) + 4);
	printf("press r to see the rules");
	
	key = _getch();
	if (key == 'r') {
		print_rules();
	}
	else {
		system("cls");
		game_over = 0;
		display_map();
		init_snake();
		init_food();
		snake_length = INITIAL_LENGTH;
		gotoxy(MAP_WIDTH + 5, 0);
		score = 0;
		printf("SCORE: %02d", score);
	}

	fflush(stdin);
}

void print_rules(void) { // indicates the rules and controls of the game
	system("cls");
	gotoxy(5, MAP_HEIGHT / 2);
	printf("->the snakes move automatically one case at a time, you can change its direction and speed by pressing the wasd keys");
	gotoxy(5, (MAP_HEIGHT / 2) + 1);
	printf("->reach the food represented by the F symbol to increase your score and the snake size by one");
	gotoxy(5, (MAP_HEIGHT / 2) + 2);
	printf("->the game ends when the snake hits a wall or itself");
	gotoxy(5, (MAP_HEIGHT / 2) + 3);
	printf("[PRESS ANY KEY]");
	_getch();
	fflush(stdin);
	system("cls");
	init_game();
}

char end_game(void) { // when the game ends printf GAME OVER and score, asks for new game
	char choice;
	system("cls");
	gotoxy(MAP_WIDTH / 2, MAP_HEIGHT / 2);
	printf("GAME OVER");
	gotoxy(MAP_WIDTH / 2, MAP_HEIGHT / 2 + 1);
	printf("SCORE: %02d", score);
	gotoxy((MAP_WIDTH / 2)-2, MAP_HEIGHT / 2 + 3);
	printf("[PRESS ANY KEY]");
	_getch();
	gotoxy((MAP_WIDTH / 2) - 2, MAP_HEIGHT / 2 + 3);
	printf("                 ");
	gotoxy(MAP_WIDTH / 2, (MAP_HEIGHT / 2)+6);
	printf("Do you want to play again? [y/n] > ");
	scanf_s("%c", &choice, 1);
	rewind(stdin);
	system("cls");

	return choice;
}

void gotoxy(int x, int y) { // function for moving
	COORD c = { x, y };
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(h, c);
}
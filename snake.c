//SNAKE GAME

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>

int i = 0; //for for loops

void endGame(int points) {
	system("cls");
	printf("Game Over!\nYou got: %d points\nPress any key to close...", points);
	getch();
	exit(0);
}

void updateScreen(char *screen) {
	//clears screen
	system("cls");
	//prints screen
	printf("Snake!\n--------------------\n");
	for (i = 0; i < 100; i++) {
		if (i % 10 == 0 && i != 0) {
			printf("|\n");
		}
		printf("%c ", *screen);
		screen++;
	}
	printf("|\n--------------------\n");
}

void loadScreen(char *screen) {
	for (i = 0; i < 100; i++) {
		*screen = ' '; 
		screen++;
	}
}

//move snake and return new location of snake
int moveSnake(char *screen, int *sLoc, int hLoc, int dir, int points, int *fruitEaten) {
	
	//get new head location 
	//check if snake crash into left or right wall
	switch (dir) {
		case 1:
			hLoc -= 10;
			break;
		case 2:
			hLoc++;
			if (hLoc % 10 == 0) {
				endGame(points);
			}
			break;
		case 3:
			hLoc += 10;
			break;
		case 4:
			hLoc--;
			if (hLoc % 10 == 9) {
				endGame(points);
			}
	}
	
	//check if snake crashed into top or bottom wall
	if (hLoc < 0 || hLoc > 100) {
		endGame(points);
	}
	
	
	//check if snake crash into body
	if (screen[hLoc] == '0') {
		endGame(points);
	}

	//update screen and move the snake's body
	//if snake just ate fruit: grow. else: current tail pixel left empty
	if (! *fruitEaten) {
		screen[sLoc[points]] = ' ';
	}
	else {
		*fruitEaten = 0;
	}
	//move the snake's body
	for (i = points; i > 0; i--) {
		sLoc[i] = sLoc[i-1];
		screen[sLoc[i-1]] = '0';
	}
	//move the snake's head
	sLoc[0] = hLoc;
	screen[hLoc] = '8';
	
	return hLoc;
}

//change location of fruit and return its new location
int updateFruit(char *screen, int *sLoc, int fLoc, int points) {
	time_t sec;
    struct tm *sec_tm;
    int hour;
    sec = time(NULL); 
    sec_tm = localtime(&sec);
    hour = sec_tm->tm_hour;
	
	srand(hour*sec);
	int r = rand() % 100; 
	screen += r;	
	while (1) {	
		if (*screen == ' ') { //if empty location:
			*screen = 'x'; //set fruit to be there 
			return r;
		}
		//else: loop again
		screen++;
		r++; 
	}
}

//-------------------------------------------------------------------------------

int main(){
	printf("Press any key to start...");
	getch();
	system("cls");
	
	char screen[100];
	//initialize screen with spaces
	loadScreen(&screen[0]);
	
	//draw snake's head
	int headLocation = 50; //[headLocation/10][headLocation%10]
	screen[50] = '8'; //head is 0
	
	//draw fruit
	int fruitLocation = 57; //like headLocation
	screen[57] = 'x'; //fruit is x   
	
	int snakeLocation[100]; //array of each body part's location
    snakeLocation[0] = 50; //only head at first
	
	int direction = 2; //1 = up, 2 = right, 3 = down, 4 = left 
	int points = 0; //points + 1 is the number of snake's bodyparts
	int fruitEaten = 0; //to know if fruit has been eaten	
	
	while (1) {
		
		//check if key pressed		
		if (kbhit()) {
			switch(getch()) {
			
				//w key, up
				case 119:
					direction = (direction == 3) ? 3 : 1;
					break;
				//d key, right
				case 100:
					direction = (direction == 4) ? 4 : 2;
					break;
				//s key, down
				case 115:
					direction = (direction == 1) ? 1 : 3;
					break;	
				//a key, left	
				case 97:
					direction = (direction == 2) ? 2 : 4;
					break;
				//t key, terminate
				case 116:
					endGame(points);
					break;
				//p key, pause
				case 112:
					getch();
			}
		}
		
		//move snake
		headLocation = moveSnake(&screen[0], &snakeLocation[0], headLocation, direction, points, &fruitEaten);
			
		//if head on fruit
		if (headLocation == fruitLocation) {
			printf("\a"); 
			points++;
			fruitEaten = 1;
			fruitLocation = updateFruit(&screen[0], &snakeLocation[0], fruitLocation, points);
		}		
		
		updateScreen(&screen[0]);
		printf("Points: %d", points);
		usleep(50000);
	}	
	
	return 0;
}

#include "GLCD.h" 
#include "../RIT/RIT.h" 
#include "../timer/timer.h" 
#include "LPC17xx.h"
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

#define WALL 'W'
#define EMPTY 'E'
#define PILL 'p'
#define POWER_PILL 'P'
#define UNAVAILABLE 'U'
#define GHOST_DOOR 'D'
#define GHOST 'G'
#define PACMAN 'M'
#define ROWS 23   //Number of rows in the matrix
#define COLS 24   //Number of columns in the matrix
#define TOTAL_PILLS 240
#define TOTAL_POWER_PILLS 6
#define INITIAL_PACMAN_X 11
#define INITIAL_PACMAN_Y 19
extern int rit_on;
extern char previousCell;
int missing_pills = TOTAL_PILLS + TOTAL_POWER_PILLS;
int total_points = 0;
int remaining_lives = 1;
int countdown = 60;
int cellSize = 10;
int margin_top = 50;
int	current_state_x;
int current_state_y;
int pacman_is_eaten = 0;
int ghost_current_state_x;
int ghost_current_state_y;
int pacman_is_dead = 0;
char grid[ROWS][COLS] = {
    {'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W'},
    {'W', 'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p', 'W', 'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p', 'W'},
    {'W', 'p', 'W', 'p', 'W', 'p', 'W', 'p', 'W', 'W', 'p', 'W', 'p', 'W', 'W', 'p', 'W', 'p', 'W', 'p', 'W', 'W', 'p', 'W'},
    {'W', 'p', 'W', 'p', 'W', 'p', 'W', 'p', 'W', 'W', 'p', 'W', 'p', 'W', 'W', 'p', 'W', 'p', 'W', 'p', 'W', 'W', 'p', 'W'},
		{'W', 'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p', 'W'},
		{'W', 'p', 'W', 'W', 'W', 'p', 'W', 'p', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'p', 'W', 'p', 'W', 'W', 'W', 'W', 'p', 'W'},
		{'W', 'p', 'p', 'p', 'p', 'p', 'W', 'p', 'p', 'p', 'p', 'W', 'p', 'p', 'p', 'p', 'W', 'p', 'p', 'p', 'p', 'p', 'p', 'W'},
		{'W', 'W', 'W', 'W', 'W', 'p', 'W', 'W', 'W', 'W', 'p', 'W', 'p', 'W', 'W', 'W', 'W', 'p', 'W', 'p', 'W', 'W', 'W', 'W'},
		{'U', 'U', 'U', 'U', 'W', 'p', 'W', 'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p', 'W', 'p', 'W', 'p', 'W', 'U', 'U', 'U'},
		{'U', 'U', 'U', 'U', 'W', 'p', 'W', 'p', 'W', 'W', 'W', 'D', 'W', 'W', 'W', 'p', 'W', 'p', 'W', 'p', 'W', 'U', 'U', 'U'},
		{'W', 'W', 'W', 'W', 'W', 'p', 'W', 'p', 'W', 'U', 'U', 'G', 'U', 'U', 'W', 'p', 'W', 'p', 'W', 'p', 'W', 'W', 'W', 'W'},
		{'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p', 'W', 'U', 'U', 'U', 'U', 'U', 'W', 'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p'},
		{'W', 'W', 'W', 'W', 'W', 'p', 'W', 'p', 'W', 'U', 'U', 'U', 'U', 'U', 'W', 'p', 'W', 'p', 'W', 'p', 'W', 'W', 'W', 'W'},
		{'U', 'U', 'U', 'U', 'W', 'p', 'W', 'p', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'p', 'W', 'p', 'p', 'p', 'W', 'U', 'U', 'U'},
		{'U', 'U', 'U', 'U', 'W', 'p', 'W', 'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p', 'W', 'p', 'W', 'p', 'W', 'U', 'U', 'U'},
		{'W', 'W', 'W', 'W', 'W', 'p', 'W', 'p', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'p', 'W', 'p', 'W', 'p', 'W', 'W', 'W', 'W'},
		{'W', 'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p', 'W', 'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p', 'W'},
    {'W', 'p', 'W', 'W', 'W', 'p', 'W', 'p', 'W', 'W', 'p', 'W', 'p', 'W', 'W', 'W', 'p', 'W', 'W', 'p', 'W', 'W', 'p', 'W'},
    {'W', 'p', 'W', 'W', 'W', 'p', 'W', 'p', 'W', 'W', 'p', 'W', 'p', 'W', 'W', 'W', 'p', 'W', 'W', 'p', 'W', 'W', 'p', 'W'},
		{'W', 'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p', 'M', 'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p', 'W'},
		{'W', 'p', 'W', 'W', 'W', 'p', 'W', 'p', 'W', 'W', 'W', 'W', 'p', 'W', 'W', 'W', 'p', 'W', 'W', 'W', 'W', 'W', 'p', 'W'},
		{'W', 'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p', 'W'},
    {'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W'}
};

void generatePowerPills() {
	int countP = 0;
	int targetCount;
	int i, j, nPP = 0;
	targetCount = rand() % (TOTAL_PILLS + TOTAL_POWER_PILLS);
	for (i = 0; i < ROWS; i++) {
		for (j = 0; j < COLS; j++) {
			if (grid[i][j] == 'p') {
				if (countP == targetCount) {
					// Replace 'p' with 'P' at the target position
					grid[i][j] = 'P';
					return;
				}
				countP++; // Increment the count of 'p' cells
			}
		}
	}

}

void printGrid() {
	unsigned int customSeed = 542; //Here I set the seed for the randomness of the power pills
    srand(customSeed);	
	int i, j, dx, dy, radius;
	// Circle center is at the center of the cell
	int halfCell = cellSize / 2;
	int nPP = 0;
	while(TOTAL_POWER_PILLS != nPP){
		generatePowerPills();
		nPP++;
	}
	for (i = 0; i < ROWS; i++) {
		for (j = 0; j < COLS; j++) {
			//WALL
			if(grid[i][j] == WALL)
				for (dx = 0; dx < cellSize; dx++) {
					for (dy = 0; dy < cellSize; dy++) {
						LCD_SetPoint(j * cellSize + dy, i * cellSize + dx + margin_top, Blue);
					}
				}
			else if(grid[i][j] == GHOST_DOOR){
				DrawDoor(j * cellSize + cellSize/2, i * cellSize + cellSize/2 + margin_top, cellSize, Grey);
				}
			//GHOST DOOR
			else if(grid[i][j] == GHOST_DOOR){
				DrawDoor(j * cellSize + cellSize/2, i * cellSize + cellSize/2 + margin_top, cellSize, Grey);
				}
			//GHOST
			else if(grid[i][j] == GHOST){
				ghost_current_state_y = i;
				ghost_current_state_x = j;
				DrawGhost(j * cellSize + cellSize/2, i * cellSize + cellSize/2 + margin_top, cellSize, Red);
				}
			//PACMAN
			else if(grid[i][j] == PACMAN){
				current_state_y = i;
				current_state_x = j;
				DrawFilledCircle(j * cellSize + cellSize/2, i * cellSize + cellSize/2 + margin_top, 5 , Yellow);
			}
			//PILL
			else if(grid[i][j] == PILL)
				DrawPill(j, i, halfCell, 1);
				//POWER PILLS
			else if(grid[i][j] == POWER_PILL)
				DrawPill(j, i, halfCell, 2);	
		}
	}
}

void print_information(){
	GUI_Text(5, 5, (uint8_t *) " Game over in ", White, Black);
	GUI_Text(160, 5, (uint8_t *) " Score ", White, Black);
	char str[4];  
  sprintf(str, "%d", countdown);  
	GUI_Text(40, 25, (uint8_t *)str, White, Black);
  sprintf(str, "00%d", total_points);  
	GUI_Text(175, 25, (uint8_t *)str, White, Black);
	DrawPacManRight(10, 290, 5, Yellow);
	GUI_Text(92, 162, (uint8_t *) "READY!", Black, Black);
	GUI_Text(102, 162, (uint8_t *) "GO!", Yellow, Black);
}

void print_total_points(int newPoints){
	total_points += newPoints;
	if(total_points > 999){
		remaining_lives++;
		print_remaining_lives();
		total_points -= 1000;
	}
	char str[4];  
	if(total_points<10){
	sprintf(str, "00%d", total_points);  
	GUI_Text(175, 25, (uint8_t *)str, White, Black);
}
	else if(total_points<100){
	sprintf(str, "0%d", total_points);  
	GUI_Text(175, 25, (uint8_t *)str, White, Black);
}
	else{ 	
		sprintf(str, "%d", total_points); 
		GUI_Text(175, 25, (uint8_t *)str, White, Black);
	}
	missing_pills -= 1;
	if(!missing_pills)winner();
}
void print_remaining_lives(){
	int i = 0;
	int d = 0;
	for(i=0; i < remaining_lives; i++){
	DrawPacManRight(10 + d, 290, 5, Yellow);
  d += 20;
	}
	DrawPacManRight(10 + d, 290, 5, Black);
}

	
void check_points(){
	int newPoints = 0;
	if(grid[current_state_y][current_state_x] == 'p'){
			newPoints = 10;
		}else if(grid[current_state_y][current_state_x] == 'P'){
			newPoints = 50;
			frightened_mode_ghost(); 
		}else if(grid[current_state_y][current_state_x] == 'G'){
			if(previousCell == 'p')
				newPoints = 110;
			else if(previousCell == 'P')
				newPoints = 150;
			else{ 
				newPoints = 100;
				missing_pills += 1;
			}}else if(grid[current_state_y][current_state_x] == 'E')
				return;
		print_total_points(newPoints);
}

void winner(){
	GUI_Text(88, 160, (uint8_t *) "You won!", Yellow, Black);
	disable_timer(1);
	rit_on = 0;
	NVIC_DisableIRQ(EINT0_IRQn);		/* disable Button interrupts			 */
}

void pacman_eaten(){
		if(remaining_lives > 0 && pacman_is_dead == 0){
			pacman_is_dead++;
			remaining_lives--;
			current_state_x = INITIAL_PACMAN_X;
			current_state_y = INITIAL_PACMAN_Y;
			direction = 0;
			ghost_ready = 0;
			DrawFilledSquare(ghost_current_state_x * cellSize + cellSize/2, ghost_current_state_y * cellSize + cellSize/2 + margin_top, cellSize, Black);
			DrawFilledCircle(current_state_x * cellSize + cellSize/2, current_state_y * cellSize + cellSize/2 + margin_top, 5, Yellow);
			grid[current_state_x][current_state_y] = 'M';
			Ghost_respawn();
			print_remaining_lives();
		}else if(remaining_lives == 0){ 
			game_over();
		}
}
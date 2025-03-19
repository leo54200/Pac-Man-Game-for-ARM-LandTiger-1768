#include <stdint.h>
#include "../GLCD/GLCD.h" 

#define ROWS 23   //Number of rows in the matrix
#define COLS 24   //Number of columns in the matrix

extern int TOTAL_PILLS;           // Total number of pills
extern int TOTAL_POWER_PILLS;     // Total number of power pills

extern uint8_t score;             // Player score
extern int remaining_lives;       // Remaining lives
extern uint8_t countdown_timer;   // Countdown timer for game duration
extern int cellSize;              // Size of each cell in the grid
extern int margin_top;            // Margin from the top of the display
extern int current_x_state;       // Current x position state
extern int current_y_state;       // Current y position state
extern char grid[ROWS][COLS];         // Game grid
void generatePowerPills() ;
void printGrid();
void print_information();
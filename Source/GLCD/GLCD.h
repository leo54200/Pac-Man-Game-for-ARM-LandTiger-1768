/****************************************Copyright (c)**************************************************                         
**
**                                 http://www.powermcu.com
**
**--------------File Info-------------------------------------------------------------------------------
** File name:			GLCD.h
** Descriptions:		Has been tested SSD1289、ILI9320、R61505U、SSD1298、ST7781、SPFD5408B、ILI9325、ILI9328、
**						HX8346A、HX8347A
**------------------------------------------------------------------------------------------------------
** Created by:			AVRman
** Created date:		2012-3-10
** Version:				1.3
** Descriptions:		The original version
**
**------------------------------------------------------------------------------------------------------
** Modified by:			
** Modified date:	
** Version:
** Descriptions:		
********************************************************************************************************/

#ifndef __GLCD_H 
#define __GLCD_H

/* Includes ------------------------------------------------------------------*/
#include "LPC17xx.h"
#include "../RIT/RIT.h" 
#include "../timer/timer.h" 
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <math.h>
/* Private define ------------------------------------------------------------*/

/* LCD Interface */
#define PIN_EN		(1 << 19)
#define PIN_LE		(1 << 20)
#define PIN_DIR		(1 << 21)
#define PIN_CS      (1 << 22)
#define PIN_RS		(1 << 23)
#define PIN_WR		(1 << 24)
#define PIN_RD		(1 << 25)   

#define LCD_EN(x)   ((x) ? (LPC_GPIO0->FIOSET = PIN_EN) : (LPC_GPIO0->FIOCLR = PIN_EN));
#define LCD_LE(x)   ((x) ? (LPC_GPIO0->FIOSET = PIN_LE) : (LPC_GPIO0->FIOCLR = PIN_LE));
#define LCD_DIR(x)  ((x) ? (LPC_GPIO0->FIOSET = PIN_DIR) : (LPC_GPIO0->FIOCLR = PIN_DIR));
#define LCD_CS(x)   ((x) ? (LPC_GPIO0->FIOSET = PIN_CS) : (LPC_GPIO0->FIOCLR = PIN_CS));
#define LCD_RS(x)   ((x) ? (LPC_GPIO0->FIOSET = PIN_RS) : (LPC_GPIO0->FIOCLR = PIN_RS));
#define LCD_WR(x)   ((x) ? (LPC_GPIO0->FIOSET = PIN_WR) : (LPC_GPIO0->FIOCLR = PIN_WR));
#define LCD_RD(x)   ((x) ? (LPC_GPIO0->FIOSET = PIN_RD) : (LPC_GPIO0->FIOCLR = PIN_RD));

/* Private define ------------------------------------------------------------*/
#define DISP_ORIENTATION  0  /* angle 0 90 */ 

#if  ( DISP_ORIENTATION == 90 ) || ( DISP_ORIENTATION == 270 )

#define  MAX_X  320
#define  MAX_Y  240   

#elif  ( DISP_ORIENTATION == 0 ) || ( DISP_ORIENTATION == 180 )

#define  MAX_X  240
#define  MAX_Y  320   

#endif

/* LCD color */
#define White          0xFFFF
#define Black          0x0000
#define Grey           0xF7DE
#define Blue           0x001F
#define Blue2          0x051F
#define Red            0xF800
#define Magenta        0xF81F
#define Green          0x07E0
#define Cyan           0x7FFF
#define Yellow         0xFFE0

/******************************************************************************
* Function Name  : RGB565CONVERT
* Description    : 24位转换16位
* Input          : - red: R
*                  - green: G 
*				   - blue: B
* Output         : None
* Return         : RGB 颜色值
* Attention		 : None
*******************************************************************************/
#define RGB565CONVERT(red, green, blue)\
(uint16_t)( (( red   >> 3 ) << 11 ) | \
(( green >> 2 ) << 5  ) | \
( blue  >> 3 ))

/* Private function prototypes -----------------------------------------------*/
void LCD_Initialization(void);
void LCD_Clear(uint16_t Color);
uint16_t LCD_GetPoint(uint16_t Xpos,uint16_t Ypos);
void LCD_SetPoint(uint16_t Xpos,uint16_t Ypos,uint16_t point);
void LCD_DrawLine( uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1 , uint16_t color );
void PutChar( uint16_t Xpos, uint16_t Ypos, uint8_t ASCI, uint16_t charColor, uint16_t bkColor );
void GUI_Text(uint16_t Xpos, uint16_t Ypos, uint8_t *str,uint16_t Color, uint16_t bkColor);
void DrawFilledCircle(int centerX, int centerY, int radius, uint16_t color);
void DrawPacManRight(int centerX, int centerY, int radius, uint16_t color);
void DrawPacManLeft(int centerX, int centerY, int radius, uint16_t color);
void DrawFilledSquare(int centerX, int centerY, int cellLength, uint16_t color);
void DrawDoor(int centerX, int centerY, int size, uint16_t color);
void DrawGhost(int centerX, int centerY, int size, uint16_t color);
void DrawPill(int x, int y, int halfCell,int radius);
#include <stdint.h>

#define ROWS 23   //Number of rows in the matrix
#define COLS 24   //Number of columns in the matrix
typedef struct Node {
    int x, y;           // Position in the grid
    float g;            // Cost from the start node
    float h;            // Heuristic cost to the goal node
    float f;            // Total cost: f = g + h
    struct Node* parent; // Parent node for path reconstruction
} Node;

// Min-heap structure
typedef struct Heap {
    Node** nodes;       // Array of node pointers
    int size;           // Current size of the heap
    int capacity;       // Maximum capacity of the heap
} Heap;

extern int powerful_pacman;
extern int power_pills_time;
extern int victory;
extern int total_points;             // Player score
extern int remaining_lives;       // Remaining lives
extern int countdown;   // Countdown timer for game duration
extern int cellSize;              // Size of each cell in the grid
extern int margin_top;            // Margin from the top of the display
extern int current_state_x;       // Current x position state
extern int current_state_y;       // Current y position state
extern int ghost_current_state_x;
extern int ghost_current_state_y;
extern char previousCell;
extern int is_closed;
extern int pacman_is_dead;
extern char grid[ROWS][COLS];         // Game grid
void generatePowerPills() ;
void printGrid();
void print_information();
void print_score();
int move_up();
int move_down();
int move_right();
int move_left();
void print_remaining_lives();
void winner();
void frightened_mode_ghost();
void chase_mode_ghost();
void check_points();
void ghost_next_move(int ghost_next_state_x, int ghost_next_state_y);
void DrawPreviousCell();
void reconstruct_path(Node* end_node);
void a_star(int start_x, int start_y, int goal_x, int goal_y);
void Ghost_respawn();
void ghost_eaten();
void run_away();
void a_star(int start_x, int start_y, int goal_x, int goal_y);

#endif 

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/

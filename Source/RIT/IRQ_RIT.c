/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_RIT.c
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        functions to manage T0 and T1 interrupts
** Correlated files:    RIT.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include "LPC17xx.h"
#include "music/music.h"
#include "RIT.h"
#include "../timer/timer.h"
#include "../GLCD/GLCD.h" 
#include "../button_EXINT/button.h" 


/******************************************************************************
** Function name:		RIT_IRQHandler
**
** Descriptions:		REPETITIVE INTERRUPT TIMER handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/
volatile int rit_on = 1;
int direction = 0;
int is_ghostTimeToMove = 3;
int ghost_speed = 3;
extern int ghost_ready;
extern int is_closed;
extern int is_pause;
void RIT_IRQHandler (void)
{		
	disable_RIT();
	reset_RIT();
	if(is_pause){
		is_pause = 0;			
		NVIC_EnableIRQ(EINT0_IRQn);							 /* disable Button interrupts			*/
	}
		if ((LPC_GPIO1->FIOPIN & (1 << 29)) == 0) { // UP pressed
			if(move_up()){
				direction = 1;
				enable_RIT();
				LPC_RIT->RICTRL |= 0x1;	/* clear interrupt flag */
				return;
			}
		}
		else if ((LPC_GPIO1->FIOPIN & (1 << 28)) == 0) { // RIGHT pressed
			if(move_right()){
				direction = 2;
				enable_RIT();
				LPC_RIT->RICTRL |= 0x1;	/* clear interrupt flag */					
				return;
			}
		}
		else if ((LPC_GPIO1->FIOPIN & (1 << 27)) == 0) { // LEFT pressed
			if(move_left()){
				direction = 3;
				enable_RIT();	
				LPC_RIT->RICTRL |= 0x1;	/* clear interrupt flag */					
				return;
			}
		}
		else if ((LPC_GPIO1->FIOPIN & (1 << 26)) == 0) { // DOWN pressed
			if(move_down()){
				direction = 4;
				enable_RIT();		
				LPC_RIT->RICTRL |= 0x1;	/* clear interrupt flag */					
				return;
			}
		}
		switch(direction){
			case 1:
				if(!move_up())
					direction = 0;
				break;
			case 2:
				if(!move_right())
					direction = 0;
				break;
			case 3:
				if(!move_left())
					direction = 0;
				break;
			case 4:
				if(!move_down())
					direction = 0;
				break;			
			default:
				break;			
    }
		if(ghost_ready != 0){
			is_ghostTimeToMove--;
			if(is_ghostTimeToMove == 0){
				if(ghost_ready == 1 && powerful_pacman == 1)
					run_away();
				else chase_mode_ghost();
				is_ghostTimeToMove = ghost_speed;
			}
		}else is_ghostTimeToMove = ghost_speed;
		if(is_closed)
			is_closed = 0;
		else is_closed = 1;
	if(rit_on)
		enable_RIT(); 
	LPC_RIT->RICTRL |= 0x1;	/* clear interrupt flag */
	return;
}

/******************************************************************************
**                            End Of File
******************************************************************************/

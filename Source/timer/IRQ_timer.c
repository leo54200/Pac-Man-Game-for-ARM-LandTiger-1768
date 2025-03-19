/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_timer.c
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        functions to manage T0 and T1 interrupts
** Correlated files:    timer.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include "LPC17xx.h"
#include "timer.h"
#include "../GLCD/GLCD.h" 
#include "../button_EXINT/button.h"
#include "../RIT/RIT.h" 
#include "music/music.h"
#include "adc/adc.h"
#include "CAN/CAN.h"
#include <string.h>
#include <stdio.h>
/******************************************************************************
** Function name:		Timer0_IRQHandler
**
** Descriptions:		Timer/Counter 0 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/
extern int countdown;
extern int rit_on;
extern power_pills_time;
extern int ghost_speed;
extern int ghost_ready;
extern int pause_on;
extern int pacman_is_dead;
int ghost_respawn_countdown = 3;
// beat 1/4 = 1.65/4 seconds
#define RIT_SEMIMINIMA 8
#define RIT_MINIMA 16
#define RIT_INTERA 32

#define UPTICKS 1


//SHORTENING UNDERTALE: TOO MANY REPETITIONS
NOTE song[] = 
{
    // Pac-Man Theme Intro
    {e4, time_semiminima},
    {g4, time_semiminima},
    {g4s, time_semiminima},
    {a4, time_semiminima},
    {g4s, time_semiminima},
    {g4, time_semiminima},
    {f4, time_semiminima},
    {d4, time_minima},
    
    // Looping Melody - Segment 1
    {a3, time_semiminima},
    {c4, time_semiminima},
    {d4, time_semiminima},
    {f4, time_semiminima},
    {d4, time_semiminima},
    {c4, time_semiminima},
    {b3, time_semiminima},
    {g3, time_semiminima},
    
    // Looping Melody - Segment 2
    {g3, time_semiminima},
    {b3, time_semiminima},
    {c4, time_semiminima},
    {d4, time_semiminima},
    {c4, time_semiminima},
    {b3, time_semiminima},
    {a3, time_semiminima},
    {f3, time_semiminima},

    // Looping Melody - Segment 3
    {f3, time_semiminima},
    {a3, time_semiminima},
    {b3, time_semiminima},
    {c4, time_semiminima},
    {b3, time_semiminima},
    {a3, time_semiminima},
    {g3, time_semiminima},
    {e3, time_semiminima},

    // Looping Melody - Segment 4
    {e3, time_semiminima},
    {g3, time_semiminima},
    {a3, time_semiminima},
    {b3, time_semiminima},
    {a3, time_semiminima},
    {g3, time_semiminima},
    {f3, time_semiminima},
    {d3, time_semiminima},

    // Repeat if needed
};
void game_over(){
	GUI_Text(80, 160, (uint8_t *) "Game Over!", Yellow, Black);
	rit_on = 0;	
	disable_timer(1);
	disable_timer(0);
	disable_timer(2);
	disable_timer(3);

	NVIC_DisableIRQ(EINT0_IRQn);		/* disable Button interrupts			 */
return;
}

volatile int is_pause = 0;
void TIMER0_IRQHandler (void)
{
static int currentNote = 0; // Tracks the current note being played 
static int ticks = 0;       // Counts the number of ticks until the next note

		/*if (pause_on) {
        disable_timer(0);
				return;
  }*/
    // Check if the note is still playing
    if (!isNotePlaying()) {
        ++ticks;

        // If ticks match UPTICKS, play the next note
        if (ticks == UPTICKS) {
            ticks = 0;
            playNote(song[currentNote++]); // Play the current note and move to the next
        }
    }

    // Reset currentNote to 0 to loop the song endlessly
    if (currentNote == (sizeof(song) / sizeof(song[0]))) {
        currentNote = 0;
    }

    // Clear the TIMER0 interrupt flag
    LPC_TIM0->IR = 1;
    return;
}


/******************************************************************************
** Function name:		Timer1_IRQHandler
**
** Descriptions:		Timer/Counter 1 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/
void TIMER1_IRQHandler (void)
{
	if(pacman_is_dead > 0)
		pacman_is_dead = 0;
	countdown -=1;
	if(power_pills_time > 0)
		power_pills_time--;
	if(power_pills_time == 0){
		end_frightened_mode_ghost();
		power_pills_time = - 1;
	}
	if(ghost_ready == 0)
		ghost_respawn_countdown--;
	if (ghost_respawn_countdown == 0){
	  Ghost_respawn();
		ghost_respawn_countdown = 3;
	}
	if(ghost_ready == 1 && powerful_pacman == 0)
		start_a_star_algorithm();

	if(countdown == 40 || 	countdown == 15)
		ghost_speed--;
	char str[4];
	if(countdown == 58)  
		GUI_Text(102, 160, (uint8_t *) "GO!", Black, Black);

	if(countdown<10)	
		sprintf(str, "0%d", countdown);  
  else 	
		sprintf(str, "%d", countdown);  
	GUI_Text(40, 25, (uint8_t *)str, White, Black);
	if(countdown == 0) 
		game_over();
	
	CAN_TxMsg.data[0] = ((countdown ) & 0xFF);
					CAN_TxMsg.data[1] = (total_points) & 0xFF;
					CAN_TxMsg.data[2] = (total_points & 0xFF00 ) >> 8;
					CAN_TxMsg.data[3] = remaining_lives & 0xFF;
					CAN_TxMsg.len = 4;
					CAN_TxMsg.id = 2;
					CAN_TxMsg.format = STANDARD_FORMAT;
					CAN_TxMsg.type = DATA_FRAME;
					CAN_wrMsg (1, &CAN_TxMsg);  

  LPC_TIM1->IR = 1;			/* clear interrupt flag */
  return;
}

/******************************************************************************
** Function name:		Timer2_IRQHandler
**
** Descriptions:		Timer/Counter 2 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/
uint16_t SinTable[45] =                                       /* ÕýÏÒ±í                       */
{
    410/1, 467/1, 523/1, 576/1, 627/1, 673/1, 714/1, 749/1, 778/1,
    799/1, 813/1, 819/1, 817/1, 807/1, 789/1, 764/1, 732/1, 694/1, 
    650/1, 602/1, 550/1, 495/1, 438/1, 381/1, 324/1, 270/1, 217/1,
    169/1, 125/1, 87/1 , 55/1 , 30/1 , 12/1 , 2/1  , 0/1  , 6/1  ,   
    20/1 , 41/1 , 70/1 , 105/1, 146/1, 193/1, 243/1, 297/1, 353/1
};
void TIMER2_IRQHandler (void)
{
	static int sineticks=0;
	/* DAC management */	
	static int currentValue; 
	currentValue = SinTable[sineticks];
	currentValue -= 410;
	currentValue /= 1;
	currentValue += 410;
	LPC_DAC->DACR = currentValue <<6;
	sineticks++;
	if(sineticks==45) sineticks=0;

  LPC_TIM2->IR = 1;			/* clear interrupt flag */
  return;
}

/******************************************************************************
** Function name:		Timer3_IRQHandler
**
** Descriptions:		Timer/Counter 3 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/
void TIMER3_IRQHandler (void)
{
	disable_timer(2);
  LPC_TIM3->IR = 1;			/* clear interrupt flag */
  return;
}

/******************************************************************************
**                            End Of File
******************************************************************************/

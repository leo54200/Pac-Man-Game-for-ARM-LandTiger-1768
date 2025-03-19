/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           timer.h
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        Prototypes of functions included in the lib_timer, funct_timer, IRQ_timer .c files
** Correlated files:    lib_timer.c, funct_timer.c, IRQ_timer.c
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#ifndef __TIMER_H
#define __TIMER_H

#include "../music/music.h"
#include "LPC17xx.h"

/* init_timer.c */
extern unsigned int init_timer( char timer_num, unsigned int timerInterval );
extern void enable_timer( char timer_num );
extern void disable_timer( char timer_num );
extern void reset_timer( char timer_num );
/* IRQ_timer.c */
extern void TIMER0_IRQHandler (void);
extern void TIMER1_IRQHandler (void);
extern volatile int musicIsPaused;
extern int board_off;

#endif /* end __TIMER_H */
/*****************************************************************************
**                            End Of File
******************************************************************************/

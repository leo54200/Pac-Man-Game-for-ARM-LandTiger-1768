/****************************************Copyright (c)****************************************************
**                                      
**                                 http://www.powermcu.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:               main.c
** Descriptions:            The GLCD application function
**
**--------------------------------------------------------------------------------------------------------
** Created by:              AVRman
** Created date:            2010-11-7
** Version:                 v1.0
** Descriptions:            The original version
**
**--------------------------------------------------------------------------------------------------------
** Modified by:             Paolo Bernardi
** Modified date:           03/01/2020
** Version:                 v2.0
** Descriptions:            basic program for LCD and Touch Panel teaching
**
*********************************************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "LPC17xx.h"
#include "GLCD/GLCD.h" 
#include "TouchPanel/TouchPanel.h"
#include "timer/timer.h"
#include "button_EXINT/button.h"


#ifdef SIMULATOR
extern uint8_t ScaleFlag; // <- ScaleFlag needs to visible in order for the emulator to find the symbol (can be placed also inside system_LPC17xx.h but since it is RO, it needs more work)
#endif

int main(void)
{
  SystemInit();  												/* System Initialization (i.e., PLL)  */
	LED_init();                           /* LED Initialization                 */
	init_RIT(0xAC4B40);									/* RIT Initialization */
  joystick_init();
	LCD_Initialization();
	CAN_Init();
	init_timer(0, 0x0098968);
	ADC_init();
		
	LCD_Clear(Black);
	GUI_Text(92, 160, (uint8_t *) "READY!", Yellow, Black);
	printGrid();
	print_information();
	start_a_star_algorithm();
	// timer0 is used for the debouncing
	//init_timer(0, 0x14E2 ); 						    /* 500us * 25MHz = 1.25*10^3 = 0x4E2 */
	// timer1 is my countdown timer
	init_timer(1, 0x17D7840); 						    /* 1s * 25MHz = 25*10^6 = 0x17D7840 */
	
	BUTTON_init();
	enable_RIT();
	enable_timer(1);
	enable_timer(0);	


	LPC_SC->PCON |= 0x1;									/* power-down	mode										*/
	LPC_SC->PCON &= ~(0x2);						
	
	LPC_PINCON->PINSEL1 |= (1<<21);
	LPC_PINCON->PINSEL1 &= ~(1<<20);
	LPC_GPIO0->FIODIR |= (1<<26);
	
	SCB->SCR |= 0x2;

	__ASM("wfi");
}

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/

/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           funct_joystick.h
** Last modified Date:  2018-12-30
** Last Version:        V1.00
** Descriptions:        High level joystick management functions
** Correlated files:    lib_joystick.c, funct_joystick.c
**--------------------------------------------------------------------------------------------------------       
*********************************************************************************************************/
/*
#include "LPC17xx.h"
#include "../timer/timer.h"
#include "../GLCD/GLCD.h" 
#include "../TouchPanel/TouchPanel.h"
#include "../button_EXINT/button.h"
#include "../RIT/RIT.h"
#include "joystick.h"
*/

/*----------------------------------------------------------------------------
  Function that turns on requested led
 *----------------------------------------------------------------------------*/
 /*
 void move_up(){
	GUI_Text(86, 160, (uint8_t *) "UP", Yellow, Black);
}
void move_down(){
	GUI_Text(86, 160, (uint8_t *) "DOWN", Yellow, Black);
}
void move_right(){	
	GUI_Text(86, 160, (uint8_t *) "RIGTH", Yellow, Black);
}        
void move_left(){
	GUI_Text(86, 160, (uint8_t *) "LEFT", Yellow, Black);
}

void joystick_On(unsigned int num) {

    if ((LPC_GPIO1->FIOPIN & (1 << 29)) != 0) { // Pulsante UP premuto
        reset_timer(0);
        move_up();
    }
    else if ((LPC_GPIO1->FIOPIN & (1 << 28)) == 0) { // Pulsante RIGHT premuto
        reset_timer(0);
        move_right();
    }
    else if ((LPC_GPIO1->FIOPIN & (1 << 27)) == 0) { // Pulsante LEFT premuto
        reset_timer(0);
        move_left();
    }
    else if ((LPC_GPIO1->FIOPIN & (1 << 26)) == 0) { // Pulsante DOWN premuto
        reset_timer(0);
        move_down();
    }
    else { // Nessun pulsante premuto
        reset_timer(0);
    }
    // Clear interrupt flag di TIMER0
    LPC_TIM0->IR |= (1 << 0);
    return;
}
*/
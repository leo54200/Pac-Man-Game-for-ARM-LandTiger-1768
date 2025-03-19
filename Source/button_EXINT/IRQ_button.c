#include "button.h"
#include "LPC17xx.h"
#include "../timer/timer.h"
#include "../GLCD/GLCD.h" 
#include "../RIT/RIT.h" 
extern int rit_on;
extern int is_pause;
int pause_on = 0;
void EINT0_IRQHandler (void)	  	/* INT0														 */
{		
	NVIC_DisableIRQ(EINT0_IRQn);		/* disable Button interrupts			 */
	if(pause_on == 0){
		GUI_Text(95, 160, (uint8_t *) "PAUSE", Black, White);
		pause_on = 1;
		disable_timer(1);
		rit_on = 0;
		disable_timer(0);
		disable_timer(2);
		disable_timer(3);
		enable_RIT();
	}
	else{
		GUI_Text(95, 160, (uint8_t *) "PAUSE", Black, Black);
		pause_on = 0;
		enable_timer(1);
		rit_on = 1;
		enable_RIT();
		enable_timer(0);
		enable_timer(2);
		enable_timer(3);
	}
	enable_timer(0);
	LPC_PINCON->PINSEL4    &= ~(1 << 22);     /* GPIO pin selection */
	is_pause = 1;

	LPC_SC->EXTINT &= (1 << 0);     /* clear pending interrupt         */
}


void EINT1_IRQHandler (void)	  	/* KEY1														 */
{
	NVIC_DisableIRQ(EINT1_IRQn);		/* disable Button interrupts			 */
	LPC_PINCON->PINSEL4    &= ~(1 << 22);     /* GPIO pin selection */
	LPC_SC->EXTINT &= (1 << 1);     /* clear pending interrupt         */
}

void EINT2_IRQHandler (void)	  	/* KEY2														 */
{
	
  LPC_SC->EXTINT &= (1 << 2);     /* clear pending interrupt         */    
}



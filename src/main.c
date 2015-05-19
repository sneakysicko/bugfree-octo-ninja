#include "LPC17xx.h"                    // Device header
#include "Board_Joystick.h"             // ::Board Support:Joystick
#include "Board_Buttons.h"              // ::Board Support:Buttons

#include"ssd1289_init.h"
#include"lcdDraw.h"

#define BUTTON_INT0                    (1 << 0)
#define BUTTON_KEY1                    (1 << 1)
#define BUTTON_KEY2                    (1 << 2)

typedef struct sPoint {
	uint16_t xb;
	uint16_t yb;
	uint16_t xe;
	uint16_t ye;
} Point;

Point a = {116, 156, 119, 159};

/*
void DMAConf(void) {
	LPC_SC->PCONP |= 1<<29;
	LPC_GPDMA->DMACConfig = 1;
	LPC_GPDMA->DMACIntTCClear = 1;
	LPC_GPDMA->DMACIntErrClr = 1;
}

void DMA0ChannelConf(void) {
	LPC_GPDMACH0->DMACCSrcAddr = (uint32_t)&leds; // tu zrodlo dzwieku
	LPC_GPDMACH0->DMACCDestAddr = (uint32_t)&LPC_DAC->DACR; //DAC
	LPC_GPDMACH0->DMACCLLI = (uint32_t)(&head);
	LPC_GPDMACH0->DMACCControl = (uint32_t)(8 + (1<<26)) | (uint32_t)(1UL<<31);
	LPC_GPDMACH0->DMACCConfig = (uint32_t)(1 + (1<<11) + (10<<6)) | (uint32_t)(1<<15);
}
*/

/*
SystemCoreClock/4 == 1s ??
*/
void Timer1Conf(void) {
	LPC_TIM1->PR = 0; //Dzielnik PCLK zanim idzie do Time Countera, 0 oznacza ze na kazde PCLK Timer Counter zwieksza sie o jeden
	LPC_TIM1->MCR = 3; //Wlacza przerwanie progu 0 oraz reset TC przy osiagnieciu progu
	LPC_TIM1->MR0 = SystemCoreClock/16; //Wartosc progu, co ile bedziemy osiagac przerwanie
	LPC_TIM1->TCR = 1; //wlacza timer
	NVIC_EnableIRQ(TIMER1_IRQn); //Wlacza przerwanie
}

void TIMER1_IRQHandler(void) {
	//Inicjucje redraw calego ekranu
	lcdRectangle(a.xb, a.yb, a.xe, a.ye, 1, LCDBlack);
	a.xb = (a.xb+4)%240;
	a.xe = (a.xe+4)%240;
	lcdRectangle(a.xb, a.yb, a.xe, a.ye, 1, LCDWhite);
	
	LPC_TIM1->IR = 1; //reset przerwania
}

int main() {
	Joystick_Initialize();
	Buttons_Initialize();
	initDisplay();
	lcdClean();
	Timer1Conf();
	//lcdRectangle(220, 120, 200, 100, 1, LCDWhite);
	
	while(1);
	
	return 0;
}

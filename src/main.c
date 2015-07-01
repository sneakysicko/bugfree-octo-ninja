#include "LPC17xx.h"                    // Device header
#include "Board_Joystick.h"             // ::Board Support:Joystick
#include "PIN_LPC17xx.h"                // Keil::Device:PIN
#include "Board_Buttons.h"              // ::Board Support:Buttons

#include"ssd1289_init.h"
#include"lcdDraw.h"
#include"snake.h"
#include"simple.h"

#include<stdio.h>

#define BUTTON_INT0                    (1 << 0)
#define BUTTON_KEY1                    (1 << 1)
#define BUTTON_KEY2                    (1 << 2)

extern Element *head;
extern Element *eatable;
extern Element *magazyn;

//zmienen do opcji szybkosci
int snakeSpeed = 1;
const int snakeSpeedArray[4] = {4,8,16,32};
const int snakeSpeedArraySize = 4;

uint8_t tickCounter = 0;
uint8_t reactCount = 0;
uint8_t menu = 1;// przenioslem ta zmienan tutaj
uint8_t options = 0;
//Zmienna pokazuje, czy trwa gra
uint8_t game = 0;

unsigned int seed = 1;
const unsigned int m = (1<<15)-1;
//Liczba zwracana przez generator liczb losowych
unsigned int out = 0;
uint8_t initialized = 0;

void randomInit() {
	out = seed%m;
	initialized = 1;
}

int random() {
    const unsigned int a = 48271;
    static int n = 0;
    
    out = (a*out)%m;
    ++n;
    return out;
}

//zmienna przechowujaca wejscie z joysticka
//pierwsze cztery bity aktualne wejscie
//kolejne cztery bity to kierunek z ktorego waz przyszedl
//opis bitow:
//1 - gora, 2 - prawo, 4 - dol, 8 - lewo
uint8_t inputControl = 0;
uint8_t oldControl = 8;
//gdzie isc niemozna
uint8_t forbidden = 2;

void DACConf(void) {
	PIN_Configure(0, 26, PIN_FUNC_2, PIN_PINMODE_TRISTATE, PIN_PINMODE_NORMAL);
	LPC_DAC->DACCTRL = 1<<1 | 1<<2 | 1<<3;
	LPC_DAC->DACCNTVAL = 567;
}

void DMAConf(void) {
	LPC_SC->PCONP |= 1<<29; //uruchomienie peryferium DMA
	LPC_GPDMA->DMACConfig = 1; //Uruchomienie DMA
	LPC_SC->DMAREQSEL = 1; //DMA aktywowane dla M0 Timer0
	LPC_GPDMA->DMACIntTCClear = 1; //Czyszczenie interruptow
	LPC_GPDMA->DMACIntErrClr = 1; //czyszczenie errorow
}

void DMA0ChannelConf(void) {
	LPC_GPDMACH0->DMACCSrcAddr = (uint32_t)&sound[0]; // tu zrodlo dzwieku
	LPC_GPDMACH0->DMACCDestAddr = (uint32_t)&(LPC_DAC->DACR); //DAC
	LPC_GPDMACH0->DMACCLLI = 0; //(uint32_t)(&head); //Linked List transfer
	LPC_GPDMACH0->DMACCControl = (uint32_t)(20000  | 1<<18 | 1 << 21 | (1<<26) |  1UL<<31);
	LPC_GPDMACH0->DMACCConfig = (uint32_t)(1  | (7<<6) | (1<<11) | (1<<15));
}

void Timer1Disable(void) {
	NVIC_DisableIRQ(TIMER1_IRQn);
	LPC_TIM1->TCR = 0;
}

void Timer1Conf(void) {
	Timer1Disable();
	LPC_TIM1->PR = 0; //Dzielnik PCLK zanim idzie do Time Countera, 0 oznacza ze na kazde PCLK Timer Counter zwieksza sie o jeden
	LPC_TIM1->MCR = 3; //Wlacza przerwanie progu 0 oraz reset TC przy osiagnieciu progu
	LPC_TIM1->MR0 = SystemCoreClock/(snakeSpeedArray[snakeSpeed]); //Wartosc progu, co ile bedziemy osiagac przerwanie
	LPC_TIM1->TCR = 1; //wlacza timer
	NVIC_EnableIRQ(TIMER1_IRQn); //Wlacza przerwanie
}

void TIMER1_IRQHandler(void) {
	++reactCount;
	if(reactCount > 100)
		reactCount = 1;
	
	LPC_TIM1->IR = 1; //reset przerwania - musi byc
}

void SysTick_Handler(void) {
	++tickCounter;
	if(initialized == 0) ++seed;
}
//to mozna przezucic do lcdDraw.c/h
void lcdMenu()
{
	lcdString(200, 300, "MENU");
	if(game == 0)
		lcdString(200, 250, "1. KEY1 - Nowa Gra");
	if(game == 1)
		lcdString(200, 250, "1. KEY1 - Kontynuuj");
	lcdString(200, 200, "2. KEY2 - Opcje");
}

void lcdOptions(const char levelSpeed)
{
	lcdString(200, 300, "POZIOM SZYBKOSCI");
    lcdCharacter(120,250,levelSpeed);
    lcdString(200,200,"KEY1 +");
    lcdString(200,150,"KEY2 -");
    lcdString(200,100,"INT0 Start");
}

void gameF()
{
	if(game == 0) {
		randomInit();
		initSnake();
		oldControl = 8;
		game = 1;
	}
	Timer1Conf();
	drawSnake();
	drawEatable();
	reactCount = 0;
	while(game == 1) 
	{
    inputControl = Joystick_GetState();

    switch(inputControl) {
        case JOYSTICK_UP:
            inputControl = 1;
            break;
        case JOYSTICK_LEFT:
            inputControl = 2;
            break;
        case JOYSTICK_DOWN:
            inputControl = 4;
            break;
        case JOYSTICK_RIGHT:
            inputControl = 8;
						break;
				default:
						inputControl = oldControl;
        }

        if(inputControl == forbidden)
            inputControl = oldControl;
				
		if(reactCount > 0) {
			game = react(inputControl);
			--reactCount;
		}
		switch(Buttons_GetState()) {
			case BUTTON_KEY1:
			case BUTTON_KEY2:
			case BUTTON_INT0:
				menu = 1;
				break;
			default:
				break;
		}
		if(menu == 1) {
			reactCount = 0;
			break;
		}
  }
	if(game == 0) {
		lcdString(200, 200, "Koniec gry");
		while(1);
	}
}

void menuF() {
	lcdClean();
	lcdMenu();
	
    while(menu == 1) //
    {
        if(seed > m)
            seed = 1;
		switch(Buttons_GetState())
		{
			case BUTTON_KEY1:
				lcdClean();
				menu = 0;
				break;
			case BUTTON_KEY2:
				lcdClean();
				lcdOptions('0' + snakeSpeed);
        options = 1;
        menu = 0;
				while(options == 1)
				{
					while(tickCounter < 15);
					tickCounter = 0;
          if(seed > m)
              seed = 1;
					switch(Buttons_GetState())
        			{
        				case BUTTON_INT0:
        					lcdClean();
                  options = 0;
									menu = 0;
        					break;
        				case BUTTON_KEY1:
        					++snakeSpeed;
            				if(snakeSpeed >= snakeSpeedArraySize)
											snakeSpeed = 0;
								lcdCharacter(120,250,'0' + snakeSpeed);
        					break;
        				case BUTTON_KEY2:
        					--snakeSpeed;
            				if(snakeSpeed < 0)
											snakeSpeed = snakeSpeedArraySize - 1;
								lcdCharacter(120,250,'0' + snakeSpeed);
        					break;
        			}
				}
				break;
		}
	}
}

int main() {
	SysTick_Config(SystemCoreClock/100); //Ticki co 10ms
	magazyn = malloc(sizeof(Element)*1200);
	Joystick_Initialize();
	Buttons_Initialize();
	initDisplay();
	foo();
	DACConf();
	DMAConf();
	
	while(1) {
		if(menu == 1)
			menuF();
		if(game >= 0)
			gameF();
	}
	
	return 0;
}

#include "LPC17xx.h"                    // Device header
#include "Board_Joystick.h"             // ::Board Support:Joystick
#include "Board_Buttons.h"              // ::Board Support:Buttons

#include"ssd1289_init.h"
#include"lcdDraw.h"
#include"snake.h"
//#include"uart.h"

#include<stdio.h>

#define BUTTON_INT0                    (1 << 0)
#define BUTTON_KEY1                    (1 << 1)
#define BUTTON_KEY2                    (1 << 2)

extern Element *head;
extern Element *eatable;
extern Element *magazyn;

//zmienen do opcji szybkosci
int snakeSpeed = 0;
const int snakeSpeedArray[4] = {4,8,16,32};
const int snakeSpeedArraySize = 4;
const char levelSpeedChar = '0';

uint8_t reactCount = 0;
uint8_t menu = 1;// przenioslem ta zmienan tutaj
uint8_t options = 0;

unsigned int seed = 1;
const unsigned int m = (1<<15)-1;
unsigned int out = 0;

void randomInit() {
	out = seed%m;
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

//Zmienna pokazuje, czy trwa gra
uint8_t game = 0;

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
	LPC_TIM1->MR0 = SystemCoreClock/(snakeSpeedArray[snakeSpeed]); //Wartosc progu, co ile bedziemy osiagac przerwanie
	LPC_TIM1->TCR = 1; //wlacza timer
	NVIC_EnableIRQ(TIMER1_IRQn); //Wlacza przerwanie
}

void Timer0Conf() {
    //LPC_SC->PCONP |= 1<<22;
    LPC_TIM0->PR = 0;
    LPC_TIM0->MCR = 3;
    LPC_TIM0->MR0 = SystemCoreClock/16;
    LPC_TIM0->TCR = 1;
    NVIC_EnableIRQ(TIMER0_IRQn);
}

void TIMER0_IRQHandler(void) {
    seed = (seed+1);

    LPC_TIM0->IR = 1;
}


void TIMER1_IRQHandler(void) {
	++reactCount;
	
	LPC_TIM1->IR = 1; //reset przerwania - musi byc
}

void Timer0Disable() {
    NVIC_DisableIRQ(TIMER0_IRQn);
    LPC_TIM0->MCR = 0;
    LPC_TIM0->TCR = 0;
}

void SysTick_Handler(void) {
	++seed;
}
//to mozna przezucic do lcdDraw.c/h
void lcdMenu()
{
	lcdString(200, 300, "MENU");
	lcdString(150, 250, "1. INT0 - Nowa Gra");
	lcdString(150, 200, "2. KEY1 - Opcje");
	lcdString(150, 150, "3. KEY2 - Wyjscie");
}

void lcdOptionLevelSpeed(const char levelSpeed)
{
	// wzialem to z lcdDraw aby przy opcjach gdy zmieniamy +/- szybksoc nie przerysowywac calego ekranu
	// tylko sam poziom
	int i,j;
	for( i  = 0 ; i < 16 ; ++i)
		for ( j = 0; j < 8 ; ++j)
		{
			lcdSetCursor(120 + j , 250 - i );
			lcdWriteReg(DATA_RAM, LCDBlack);
		}
	lcdCharacter(120,250,levelSpeed);
}
void lcdOptions(const char levelSpeed)
{
	lcdString(150, 300, "POZIOM SZYBKOSCI");
    lcdCharacter(120,250,levelSpeed);
    lcdString(150,210,"KEY1 +");
    lcdString(150,170,"KEY2 -");
    lcdString(150,140,"INT0 Start");
}

void game()
{
	Timer1Conf();
	//Timer0Disable();
    SysTick_Config(0); //Wylacza systicka?
	randomInit();
	initSnake();
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
  }
  lcdString(200, 240, "Koniec gry");
  while(1);
}

int main() {
    //uint8_t menu = 1;  zrobilem jako globalna 
	//UART0Conf();
	SysTick_Config(SystemCoreClock/100); //Ticki co 10ms
	//printf("Hwello\n");
	magazyn = malloc(sizeof(Element)*1200);
	Joystick_Initialize();
	Buttons_Initialize();
	initDisplay();
    //Timer0Conf(); //Seed gen via timer0

	lcdClean();
	lcdMenu();
	
	//Timer1Conf(); wrzucilem go do game()
    while(menu == 1) //
    {
        if(seed > m)
            seed = 1;
		switch(Buttons_GetState())
		{
			case BUTTON_INT0:
				lcdClean();	
                menu = 0;
				game();		
				break;
			case BUTTON_KEY1:
				lcdClean();
				lcdOptions(levelSpeedChar);
                options = 1;
                menu = 0;
				while(options == 1)
				{
                    if(seed > m)
                        seed = 1;
					switch(Buttons_GetState())
        			{
        				case BUTTON_INT0:
        					lcdClean();
                            options = 0;
        					game();
        					break;
        				case BUTTON_KEY1:
        					++snakeSpeed;
            				snakeSpeed %= snakeSpeedArraySize;
            				lcdOptionLevelSpeed(levelSpeedChar + snakeSpeed);
        					break;
        				case BUTTON_KEY2:
        					--snakeSpeed;
            				if(snakeSpeed < 0)snakeSpeed = snakeSpeedArraySize - 1;
            				snakeSpeed %= snakeSpeedArraySize;
            				lcdOptionLevelSpeed(levelSpeedChar + snakeSpeed);
        					break;
        			}
				}
                menu = 1;
				break;
			case BUTTON_KEY2:
				lcdClean();
			    lcdString(150, 250, "Zakonczenie gry");
                while(1);
				break;
		}

	}
	//calosciowo to przerzucilem do game()
	
	/*
    Timer0Disable();
	randomInit();
    initSnake();
	while(game == 1) {
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
  }
	lcdString(200, 240, "Koniec gry");
	while(1);*/ 
	

	return 0;
}

#include"uart.h"

void UART0Conf(void) {
	PIN_Configure(0, 2, PIN_FUNC_1, PIN_PINMODE_TRISTATE, PIN_PINMODE_NORMAL);
  PIN_Configure(0, 3, PIN_FUNC_1, PIN_PINMODE_TRISTATE, PIN_PINMODE_NORMAL);
	
	LPC_UART0->LCR = 1<<7;
	LPC_UART0->DLL = 13;
	LPC_UART0->DLM = 0;
	LPC_UART0->LCR = 3;
	LPC_UART0->FCR = 6;
}

void sendByte(uint32_t data){
	static uint32_t test = 1<<5;
	while(!(LPC_UART0->LSR&test));
	LPC_UART0->THR = data;
}

uint32_t receiveByte() {
	while(!(LPC_UART0->LSR&1));
	return LPC_UART0->RBR;
}

void sendMsg(const char* msg) {
	int i=0;
	for(i=0; msg[i]; ++i) sendByte(msg[i]);
}

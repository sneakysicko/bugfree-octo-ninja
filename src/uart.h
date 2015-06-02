#include "LPC17xx.h"                    // Device header
#include "PIN_LPC17xx.h"                // Keil::Device:PIN

void UART0Conf(void);

void sendByte(uint32_t data);

uint32_t receiveByte(void);

void sendMsg(const char* msg);

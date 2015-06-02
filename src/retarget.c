#pragma import(__use_no_semihosting_swi)
#include"rt_sys.h"
#include<stdio.h>
#include"uart.h"

struct __FILE {
	int handle;
};

FILE __stdout;
FILE __stdin;

int fputc(int a, FILE* s) {
	sendByte(a);
	return 0;
}

/*int ferror(FILE* f) {
	return 0;
}*/

int __backspace(FILE* f) {
	return 0;
}

int fgetc(FILE* s) {
	int a = receiveByte();
	sendByte(a);
	return a;
}

void _sys_exit(int in) {
	sendMsg("\nPo return\n");
	while(1);
}

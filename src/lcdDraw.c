#include"lcdDraw.h"

/* Wymiary ekranu:
x = 240
y = 320
*/

void lcdClean() {
	int count = 240*320;
	lcdSetCursor(0, 0);
	while(--count)
		lcdWriteReg(DATA_RAM, LCDBlack);
}

void lcdPoint(uint16_t x, uint16_t y, uint16_t color) {
	lcdSetCursor(x, y);
	lcdWriteReg(DATA_RAM, color);
}

void lcdLine(uint16_t xBegin, uint16_t yBegin, uint16_t xEnd, uint16_t yEnd, uint16_t color) {
	int d, dx, dy, ai, bi, xi, yi;
	int x = xBegin, y = yBegin;
	
	if(xBegin < xEnd) {
		xi = 1;
		dx = xEnd - xBegin;
	}
	else {
		xi = -1;
		dx = xBegin - xEnd;
	}
	
	if(yBegin < yEnd) {
		yi = 1;
		dy = yEnd - yBegin;
	}
	else {
		yi = -1;
		dy = yBegin - yEnd;
	}
	
	lcdPoint(x, y, color);
	
	if(dx > dy) {
		ai = (dy - dx) * 2;
		bi = dy * 2;
		d = bi - dx;
		while(x != xEnd) {
			if(d >= 0) {
				x += xi;
				y += yi;
				d += ai;
			}
			else {
				d += bi;
				x += xi;
			}
			lcdPoint(x, y, color);
		}
	}
	else {
		ai = (dx - dy) * 2;
		bi = dx * 2;
		d = bi - dy;
		while(y != yEnd) {
			if(d >= 0) {
				x += xi;
				y += yi;
				d += ai;
			}
			else {
				d += bi;
				y += yi;
			}
			lcdPoint(x, y, color);
		}
	}
}

void lcdRectangle(uint16_t xBegin, uint16_t yBegin, uint16_t xEnd, uint16_t yEnd, int fill, uint16_t color) {
	int i;
	
	if(fill == 1) {
        //do zaimplementowania metoda z autoinkrementacja w ograniczonym obszarze
        /*
		for(i = yBegin; i <= yEnd; ++i)
			lcdLine(xBegin, i, xEnd, i, color);
            */
        for(i = yBegin; i <= yEnd; ++i) {
            lcdSetCursor(xBegin, i);
            lcdWriteReg(DATA_RAM, color);
            lcdWriteReg(DATA_RAM, color);
            lcdWriteReg(DATA_RAM, color);
            lcdWriteReg(DATA_RAM, color);
        }
	}
	if(fill == 0) {
		lcdLine(xBegin, yBegin, xEnd, yBegin, color);
		lcdLine(xBegin, yEnd, xEnd, yEnd, color);
		lcdLine(xBegin, yBegin, xBegin, yEnd, color);
		lcdLine(xEnd, yBegin, xEnd, yEnd, color);
	}
}

void lcdCharacter(uint16_t xBegin, uint16_t yBegin, const unsigned char character) {
	unsigned char buffer[16], mask, bit;
	int i, j;
	
	GetASCIICode(0, buffer, character);
	for(i = 0; i < 16; ++i) {
		mask = 1;
		for(j = 0; j < 8; ++j) {
			bit = buffer[i] & mask;
			if(bit)
				lcdPoint(xBegin + j, yBegin - i, LCDBlack);
			else
				lcdPoint(xBegin + j, yBegin - i, LCDWhite);
			mask <<= 1;
		}
	}
}

void lcdString(uint16_t xBegin, uint16_t yBegin, const unsigned char* string) {
	uint16_t x = xBegin, y = yBegin;
	int i = 0;
	
	while(string[i]) {
		lcdCharacter(x, y, string[i]);
		x -=10 ;
		++i;
	}
}

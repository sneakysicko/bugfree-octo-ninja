#include"lcdas.h"
#include"asciiLib.h"

void lcdClean(void);
void lcdPoint(uint16_t x, uint16_t y, uint16_t color);
void lcdLine(uint16_t xBegin, uint16_t yBegin, uint16_t xEnd, uint16_t yEnd, uint16_t color);
void lcdRectangle(uint16_t xBegin, uint16_t yBegin, uint16_t xEnd, uint16_t yEnd, int fill, uint16_t color);
void lcdCharacter(uint16_t xBegin, uint16_t yBegin, const unsigned char character);
void lcdString(uint16_t xBegin, uint16_t yBegin, const unsigned char* string);

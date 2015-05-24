#ifndef SNAKE_H
#define SNAKE_H
#include<stdlib.h>
#include<time.h>

#include"lcdDraw.h"

typedef struct sElement {
    uint16_t x;
    uint16_t y;
    struct sElement *next;
    struct sElement *prev;
} Element;

Element *head = NULL;
Element *eatable = NULL;

void initSnake(void);
void newEatable(void);
uint8_t checkCollision(uint8_t input);

#endif

#ifndef SNAKE_H
#define SNAKE_H
#include<stdlib.h>
#include<time.h>

#include"lcdDraw.h"
#include"random.h"
#define ELSIZE 8

typedef struct sElement {
    uint16_t x;
    uint16_t y;
    struct sElement *next;
    struct sElement *prev;
} Element;

void initSnake(void);
void newEatable(void);
uint8_t checkCollision(uint8_t input);
Element* factory(void);
uint8_t react(uint8_t inputControl);
void drawSnake(void);
void drawEatable(void);

#endif

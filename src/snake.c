#include "snake.h"
//#include "dotArray.h"

Element *head = NULL;
Element *eatable = NULL;
Element *magazyn = NULL;
extern uint8_t oldControl;
extern uint8_t forbidden;

Element* factory() {
	static int returned = 0;
	return &(magazyn[returned++]);
}

void initSnake() {
    Element *iter;
    head = factory();
    if(head != NULL) {
        head->x = 15*8;
        head->y = 20*8;
        head->prev = NULL;
        head->next = factory();
        if(head->next != NULL) {
            head->next->x = head->x + ELSIZE;
            head->next->y = head->y;
            head->next->prev = head;
            head->next->next = factory();
            if(head->next->next != NULL) {
                head->next->next->x = head->x + ELSIZE*2;
                head->next->next->y = head->y;
                head->next->next->prev = head->next;
                head->next->next->next = NULL;
            }
        }
    }
    else {
        //Jakas wiadomosc na lcd?
    }

    for(iter = head; iter != NULL; iter = iter->next)
        lcdRectangle(iter->x, iter->y, iter->x+ELSIZE-1, iter->y+ELSIZE-1, 1, fgCOLOR);

    newEatable();
}

//generujemy nowy jadalny element, element jest poprawny jesli jego next
//ma wartosc 0xEA, w trakcie sprawdzania jesli next ma wartox 0xEE
void newEatable() {
    while(eatable == NULL || eatable->next != (Element*)0xEA) {
        if(eatable == NULL)
            eatable = factory();
        eatable->x = (random()%30)*8;//rand()%240;
        eatable->y = (random()%40)*8;//rand()%320;
        eatable->next = (Element*)0xEE;
        eatable->prev = NULL;
        if(checkCollision(0) == 1)
            eatable->next = (Element*)0xEA;
    }
    lcdRectangle(eatable->x, eatable->y, eatable->x+ELSIZE-1, eatable->y+ELSIZE-1, 1, fgCOLOR);
}

//1 oznacza brak confilktow, 0 oznacza konflikt
uint8_t checkCollision(uint8_t input) {
    Element *current;
    if(eatable->next == (Element*)0xEE) {
        //sprawdzamy eatable
        current = head;
        while(current != NULL && (eatable->x != current->x || eatable->y != current->y))
            current = current->next;
        if(current == NULL)
            return 1;
        else
            return 0;
    }
    else {
        //sprawdzamy wunsza, do implementacji
        //jesli wunsz zjadl eatable to eatable jest nowa glowa
        //jesli wunsz nie zjadl eatable i nie wszedl w nic to zmienia wspolrzedne wunsza
        switch(input & 15) {
            case 1:
                head->y = (head->y+ELSIZE)%320;
                break;
            case 2:
                head->x = (head->x+ELSIZE)%240;
                break;
            case 4:
                head->y = (head->y-ELSIZE+320)%320;
                break;
            case 8:
                head->x = (head->x-ELSIZE+240)%240;
        }
        if(head->x == eatable->x && head->y == eatable->y) {
            switch(input & 15) {
                case 1:
                    head->y = (head->y-ELSIZE+320)%320;
                    break;
                case 2:
                    head->x = (head->x-ELSIZE+240)%240;
                    break;
                case 4:
                    head->y = (head->y+ELSIZE)%320;
                    break;
                case 8:
                    head->x = (head->x+ELSIZE)%240;
            }
            eatable->next = head;
            head = eatable;
            eatable = NULL;
            return 2;
        }
        else {
            current = head->next;
            while(current != NULL && (current->x != head->x || current->y != head->y))
                current = current->next;
            if(current == NULL)
                return 1;
            else
                return 0;
        }
    }
}

uint8_t react(uint8_t inputControl) {
    Element *iter = head, copy = *head;
    uint8_t result;
	//Inicjucje redraw calego ekranu
    /*
	lcdRectangle(a.xb, a.yb, a.xe, a.ye, 1, LCDBlack);
	a.xb = (a.xb+4)%240;
	a.xe = (a.xe+4)%240;
	lcdRectangle(a.xb, a.yb, a.xe, a.ye, 1, LCDWhite);
    */
    //presuwamy koniec weza
    result = checkCollision(inputControl);
    if(result == 1) {
        while(iter->next != NULL)
            iter = iter->next;
        lcdRectangle(iter->x, iter->y, iter->x+ELSIZE-1, iter->y+ELSIZE-1, 1, bgCOLOR);
        while(iter->prev != head) {
            iter->x = iter->prev->x;
            iter->y = iter->prev->y;
            iter = iter->prev;
        }
        iter->x = copy.x;
        iter->y = copy.y;
        //rysuje nowa glowe
        lcdRectangle(head->x, head->y, head->x+ELSIZE-1, head->y+ELSIZE-1, 1, fgCOLOR);
        oldControl = inputControl;
        switch(oldControl) {
            case 1:
                forbidden = 4;
                break;
            case 2:
                forbidden = 8;
                break;
            case 4:
                forbidden = 1;
                break;
            case 8:
                forbidden = 2;
                break;
        }
		return 1;
    }
    else
        return 0;
}

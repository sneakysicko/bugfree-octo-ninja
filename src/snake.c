#include "snake.h"

void initSnake() {
    Element *iter;
    head = malloc(sizeof(Element));
    if(head != NULL) {
        head->x = 116;
        head->y = 156;
        head->prev = NULL;
        head->next = malloc(sizeof(Element));
        if(head->next != NULL) {
            head->next->x = head->x + 4;
            head->next->y = head->y;
            head->next->prev = head;
            head->next->next = malloc(sizeof(Element));
            if(head->next->next != NULL) {
                head->next->next->x = head->x + 8;
                head->next->next->y = head->y;
                head->next->next->prev = head->next;
                head->next->next->next = NULL;
            }
        }
    }
    else {
        //Jakas wiadomosc na lcd?
    }

    for(iter = head; iter != null; iter = iter->next)
        lcdRectangle(iter->x, iter->y, iter->x+3, iter->y+3, 1, fgCOLOR);

    newEatable();
}

//generujemy nowy jadalny element, element jest poprawny jesli jego next
//ma wartosc 0xEA, w trakcie sprawdzania jesli next ma wartox 0xEE
void newEatable() {
    while(eatable == NULL || eatable->next != 0xEA) {
        if(eatable == NULL)
            eatable = malloc(sizeof(Element));
        eatable->x = rand()%240;
        eatable->y = rand()%320;
        eatable->next = 0xEE;
        eatable->prev = NULL
        if(checkCollision() == 1)
            eatable->next = 0xEA;
    }
    lcdRectangle(eatable->x, eatable->y, eatable->x+3, eatable->y+3, 1, fgCOLOR);
}

//1 oznacza brak confilktow, 0 oznacza konflikt
uint8_t checkCollision(uint8_t input) {
    Element *current;
    if(eatable->next == 0xEE) {
        //sprawdzamy eatable
        current = head;
        while((eatable->x != current->x && eatable->y != current->y) || current != NULL)
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
                head->y = (head->y+4)%320;
                break;
            case 2:
                head->x = (head->x+4)%240;
                break;
            case 4:
                head->y = (head->y-4+320)%320;
                break;
            case 8:
                head->x = (head->x-4+240)%240;
        }
        if(head->x == eatable->x && head->y == eatable->y) {
            switch(input & 15) {
                case 1:
                    head->y = (head->y-4+320)%320;
                    break;
                case 2:
                    head->x = (head->x-4+240)%240;
                    break;
                case 4:
                    head->y = (head->y+4)%320;
                    break;
                case 8:
                    head->x = (head->x+4)%240;
            }
            eatable->next = head;
            head = eatable;
            eatable = NULL;
            return 2;
        }
        else {
            current = head->next;
            while((current->x != head->x && current->y != head->y) || current != NULL)
                current = current->next;
            if(current == NULL)
                return 1;
            else
                return 0;
        }
    }
}

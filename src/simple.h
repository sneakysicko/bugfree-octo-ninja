#ifndef SIMPLE_H
#define SIMPLE_H
#include<math.h>

#define PI 3.14159265

uint16_t sound[10000];
void foo() {
	int i, j = 0;
	float delta1 = 2*PI/160, delta2 = 2*PI/80;
	float t1 = 0, t2 = 0;
	while(j < 10000) {
		for(i = 0; i<160; ++i) {
			sound[j] = (((int)((sin(t1)+1)*250)) + ((int)((sin(t2)+1)*250)))<<6;
			++j;
			t1 += delta1;
			t2 += delta2;
		}
		t1 = 0;
		t2 = 0;
	}
}

#endif

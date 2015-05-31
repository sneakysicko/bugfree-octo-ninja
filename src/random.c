#include"random.h"

uint32_t seed = 0;
const uint32_t m = (1<<31)-1;

void Timer3Conf() {
    LPC_TIM3->PR = 0;
    LPC_TIM3->MCR = 3;
    LPC_TIM3->MR0 = SystemCoreClock/32;
    LPC_TIM3->TCR = 1;
    NVIC_EnableIRQ(TIMER3_IRQn);
}

void Timer3Disable() {
    LPC_TIM3->MCR = 0;
    LPC_TIM3->TCR = 0;
    NVIC_DisableIRQ(TIMER3_IRQn);
}

void TIMER3_IRQHandler(void) {
    seed = (seed+1)%m;

    LPC_TIM3->IR = 1;
}

int random() {
    const uint32_t a = 48271;
    static int n = 0;
    static uint32_t out = seed;

    out = (a*out)%m;
    ++n;

    return out;
}

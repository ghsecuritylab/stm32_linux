#include "main.h"
#include<stdint.h>

void button_init(void);

void button_init(void) {
    /*1. Enable GPIOA clock */
    /* because BUTTON is connected to GPIOA PIN0 */
    //RCC_AHB1ENR
    RCC->AHB1ENR |= 0X01; //Enables the clock

    /* 2. set the mode of GPIOA pin0 to "INPUT" */
    GPIOA->MODER &= ~0X03;
    GPIOA->PUPDR &= ~0x3;

    //enable clock for RCC
    RCC->APB2ENR |= 0x00004000;

    /*3. set the interrupt triggering level */
    //EXTI_FTSR
    EXTI->FTSR |= 0X01; 

    /*4. enable the interrupt over EXTI0 */
    EXTI->IMR |= 0X01;

    //nvic configuration 
    NVIC->IP[EXTI0_IRQn] = 0x00;
    NVIC_EnableIRQ(EXTI0_IRQn);
}

int main(void)
{
    led_init();
    button_init();

    while(1){}
    return 0;
}

void EXTI0_IRQHandler(void){
    /*clear the pending bit for exti0 */
    if( (EXTI->PR & 0x01) ) {
        EXTI->PR = 0x01;//Writing 1 , clears the pending bit for exti0
    }
    led_toggle(LED_4);

    /*process only which is important and trigger the pendsv to handle rest in the pendsv handler  */
    SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk;
}

/* PendSV Exception handler */

void PendSV_Handler(void){
    // bottom half work can be done here. 
    led_toggle(LED_3);
}

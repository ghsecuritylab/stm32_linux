#include "main.h"

//void button_init(void);
int main(void)
{
//    HAL_Init();
    uint32_t val;
    led_init();

    /*configure systick timing */
    //2000 ticks
    SysTick_Config(2000000UL);//125 mini seconds

    /*configure Systick priority and enable it */
    NVIC_SetPriority(SysTick_IRQn, 0xF0);
    //SCB->SHP[0x0B] = 0XF0;// ( high priority)

    /* Configure EXTI Line0 (connected to PA0 pin) in interrupt mode */
    button_init();

    /* Infinite loop */
    while (1){}
    return 0;
}

/**
  * @brief EXTI line detection callbacks
  * @param GPIO_Pin: Specifies the pins connected EXTI line
  * @retval None
  */

void button_init(void){
    /* Enable GPIOA clock */
    /* because BUTTON is connected to GPIOA */
    RCC->AHB1ENR |= 0x01;

    //set the mode
    GPIOA->MODER &= ~0x3;
    GPIOA->PUPDR  &= ~0x3;

    //enable clock for RCC
    RCC->APB2ENR |= 0x00004000;

    //configure the interrupt
    EXTI->IMR |= 0x01;

    //EXTI->RTSR |= 0X01;
    EXTI->FTSR |= 0X01;

    //nvic configuration 

    /* button is irq number 6. which is connected over EXTI0 line in stm32f4xx */
    NVIC->IP[EXTI0_IRQn] = 0X00;// (low priority )
    NVIC_EnableIRQ(EXTI0_IRQn);
}

void EXTI0_IRQHandler(void){
    uint32_t i=0;
    /* clear the interrupt pending bit of exti0*/

    if( (EXTI->PR & 0x01) ){
	    EXTI->PR &= 0x01;
    }

  //PD12

    led_on(LED_4);
    for(i=0;i<500000;i++){
        //do some work
//        if (!(i%100000)) led_toggle(LED_3);
    }
    led_off(LED_4);
}

/* SysTick Exception handler */

void SysTick_Handler(void){
    //PD13
    led_toggle(LED_3);
}

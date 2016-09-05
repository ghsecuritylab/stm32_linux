#include "main.h"
//#include "led.h"
//#include "my_board.h"

//static int is_privileged_access(void);
//static void go_to_privileged_access_level(void);
//static void go_to_unprivileged_access_level(void);
//static void EXTI0_IRQHandler(void);
//static void button_init(void);
int main(void)
{
    //HAL_Init();
    led_init();
    button_init();

    uint32_t val;
    if(is_privileged_access()) {
        __set_PRIMASK(1);
        __set_PRIMASK(0);
    }

    go_to_unprivileged_access_level();

    __set_PRIMASK(1);
    __set_PRIMASK(0);

    go_to_privileged_access_level();

    __set_PRIMASK(1);
    __set_PRIMASK(0);

    /* Infinite loop */
    while (1) {
        __set_PRIMASK(1);
        __set_PRIMASK(0);
    }

    return 0;
}

/**
  * @brief EXTI line detection callbacks
  * @param GPIO_Pin: Specifies the pins connected EXTI line
  * @retval None
  */

int is_privileged_access(void){
    if ( (__get_CONTROL() & 0X01 )) return 0;//we are in  un privileged mode 
    else return 1;// wer are in prvileged mode 
}

void go_to_privileged_access_level(void){
    uint32_t val;
    val = __get_CONTROL();
    val &= ~0x01; //set to privileged mode 
    __set_CONTROL(val);
}

void go_to_unprivileged_access_level(void) {
    uint32_t val;
    val = __get_CONTROL();
    val |= 0x01; //set to unprivileged mode 
    __set_CONTROL(val);
}

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
    //      EXTI->RTSR |= 0X01;
    EXTI->FTSR |= 0X01;
    //nvic configuration 
    NVIC->IP[EXTI0_IRQn] = 0;
    NVIC_EnableIRQ(EXTI0_IRQn);
}

void EXTI0_IRQHandler(void){
  if( (EXTI->PR & 0x01) ) EXTI->PR &= 0x01;
  led_toggle(LED_4);
  go_to_privileged_access_level();
}


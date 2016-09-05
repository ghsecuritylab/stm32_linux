#include "my_board.h"
#include "main.h"

void led_init(void)	{
	
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOGEN;
	
	//configure LED_4 . 
	GPIOG->MODER  |= (0x01 << (LED_4 * 2));
	GPIOG->OTYPER |= ( 0 << LED_4);
	GPIOG->PUPDR   |= (0x00 << (LED_4 * 2));
	GPIOG->OSPEEDR |= (0X00 << (LED_4 * 2));
	
		
	//configure LED_3 . 
	GPIOG->MODER  |= (0x01 << (LED_3 * 2));
	GPIOG->OTYPER |= ( 0 << LED_3);
	GPIOG->PUPDR   |= (0x00 << (LED_3 * 2));
	GPIOG->OSPEEDR |= (0X00 << (LED_3 * 2));
	
  //BSP_LED_Init(LED3);
  //BSP_LED_Init(LED4);
}


void led_on(uint8_t led_no)
{
	GPIOG->BSRR = ( 1 << led_no );
}

void led_off(uint8_t led_no)
{
	GPIOG->BSRR = ( 1 << (led_no+16) );
}


void led_toggle(uint8_t led_no)
{
	if(GPIOG->ODR & (1 << led_no) )
	{
		led_off(led_no);
	}else
	{
		led_on(led_no);
	}
	
}

void EXTI0_IRQHandler(void){
    led_toggle(LED_4);
    /*clear the exti0 interrupt pending bit */
    if( (EXTI->PR & 0x01) ){
	EXTI->PR &= 0x01;
    }
}



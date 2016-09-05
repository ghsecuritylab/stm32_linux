#include "main.h"
#include<stdint.h>

/* offest of the AHB1ENR register from RCC block */
#define RCC_AHB1ENR_OFFSET          0x30

/* Base address of the RCC.. we got this address by looking into memory map */
#define RCC_BASE_ADDR               (0x40023800)

/* actual address of the RCC_AHB1ENR Register */
#define RCC_AHB1ENR_ADDR            *((volatile unsigned long *)(RCC_BASE_ADDR + RCC_AHB1ENR_OFFSET) )
#define BASE_ADDR_GPIOG             (0x40021800)

#define GPIOG_MODER_OFFSET          0X00
#define GPIOG_MODER                 *((volatile unsigned long *)(BASE_ADDR_GPIOG + GPIOG_MODER_OFFSET))
#define OFFSET_OF_OUTPU_DATA_REG    0X14
#define GPIOG_OPDATAREG             *((volatile unsigned long *)(BASE_ADDR_GPIOG+OFFSET_OF_OUTPU_DATA_REG))
#define OFFSET_OF_INPUT_DATA_REG    0X10
#define GPIOG_IPDATAREG             *((volatile unsigned long *)(BASE_ADDR_GPIOG+OFFSET_OF_INPUT_DATA_REG))

void toggle_led(void){
    if(GPIOG_IPDATAREG & (1<<14) ){
        //LED IS on , turn if off 
        GPIOG_OPDATAREG &= ~(1<<14);
    } else {
        GPIOG_OPDATAREG |= (1<<14);
    }
}

int main(void)
{
    uint32_t i=0;
    /* 1. Enable the clock for GPIOG port */
    RCC_AHB1ENR_ADDR |= (1<<6);//bit 6 enables the clock for the GPIOG port

    /* 2. Configure the GPIO PIN to output mode using MODE register  */
    GPIOG_MODER |= (1 << (14 * 2)); //set 28th bit and 29 bit to 0x01 to make 14pin output

    /* 3. use the DATA REGISTER of GPIOD port to write to or read from LED */
    GPIOG_OPDATAREG |= (1<<14);//turn the led4 on. remember led 4 is connected to GPIOG PIN 14

    while(1){
	toggle_led();
	for(i=0;i<500000;i++);
	toggle_led();
	for(i=0;i<500000;i++);
    }
    return 0;
}


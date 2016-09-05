  .text
  .global SVC_Handler

  .syntax unified
  .cpu cortex-m4
  .fpu softvfp
  .thumb

SVC_Handler:
    TST LR,#4            @ Check the LR bit position 2, to test which Stack pointer was in during stacking operation
    ITE EQ
    MRSEQ R0,MSP         @ if LR bit2 is 0, then copy MSP to RO
    MRSNE R0,PSP         @ if LR bit2 is 1 , then copy PSP TO R0

    B      sv_call_write_data_handler @ Call C function for further processing 

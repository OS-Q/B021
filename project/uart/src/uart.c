/*******************************************************************************
****版本：V1.0.0
****平台：STM8S003
****日期：2021-01-12
****作者：Qitas
****版权：OS-Q
*******************************************************************************/
#include <string.h>
#include <stdint.h>
#include "stm8s.h"
#include "uart.h"

#define __delay() {\
    uint32_t i;\
    for(i = 0; i < 2000000; i++) \
        __asm nop __endasm;\
}

/*******************************************************************************
**函数信息 ：
**功能描述 ：
**输入参数 ：
**输出参数 ：
*******************************************************************************/

inline void port_init()
{
    PA_ODR = 0x00;
    PA_DDR = 0xFF;
    PA_CR1 = 0xFF;
    PA_CR2 = 0x00;

    PB_ODR = 0x00;
    PB_DDR = 0xFF;
    PB_CR1 = 0xFF;
    PB_CR2 = 0x00;

    PC_ODR = 0x00;
    PC_DDR = 0xFF;
    PC_CR1 = 0xFF;
    PC_CR2 = 0x00;

    PD_ODR = 0x00;
    PD_DDR = 0xFF;
    PD_CR1 = 0xFF;
    PD_CR2 = 0x00;
}

/*******************************************************************************
**函数信息 ：
**功能描述 ：Reconfigure System clock, use high speed internal oscillator
**输入参数 ：
**输出参数 ：
*******************************************************************************/
void clock_init()
{
    nointerrupts();
    CLK_ICKR = 0;
    CLK_ICKR |= CLK_ICKR_HSIEN;
    CLK_ECKR = 0;
    CLK_SWR = CLK_SWR_HSI;
    while (0 == (CLK_ICKR & CLK_ICKR_HSIRDY));
    CLK_CKDIVR = 0;
    CLK_PCKENR1 = 0xFF;
    CLK_PCKENR2 = 0xFF;
    CLK_CCOR = 0;
    CLK_HSITRIMR = 0;
    CLK_SWIMCCR = 0;

    CLK_SWCR = 0;
    CLK_SWCR |= CLK_SWCR_SWEN;
    while (0 != (CLK_SWCR & CLK_SWCR_SWBSY));
    interrupts();
}


/*******************************************************************************
**函数信息 ：
**功能描述 ：
**输入参数 ：
**输出参数 ：
*******************************************************************************/
int uart_puts(const char *s)
{
    uint8_t i;
    for(i = 0; i < strlen(s); i++) {
        while(!(UART1_SR & UART_SR_TXE));
        UART1_DR = s[i];
    }

    return(i);
}

/*******************************************************************************
**函数信息 ：
**功能描述 ：
**输入参数 ：
**输出参数 ：
*******************************************************************************/
void uart_init()
{
    /* Configure RX and TX pins */
    PD_DDR = 0xBF;
    PD_CR1 = 0xFF;

    /* Enable TX & RX */
    UART1_CR2 = UART_CR2_TEN | UART_CR2_REN;
    /* 1 stop bit */
    UART1_CR3 &= ~(UART_CR3_STOP1 | UART_CR3_STOP0);
    /* 115200 baud, 16MHz, Error:0.08%   */
    UART1_BRR2 = 0x0B;
    UART1_BRR1 = 0x08;
}

/*******************************************************************************
**函数信息 ：
**功能描述 ：
**输入参数 ：
**输出参数 ：
*******************************************************************************/
int main()
{
    port_init();
    clock_init();
    uart_init();
    while(1)
    {
        PB_ODR ^= 0x20;
        uart_puts("P02/STM8S003 UART baud 115200.It is running on B021!\r\n");
        __delay();
    }
}

/*---------------------------(C) COPYRIGHT 2021 OS-Q -------------------------*/

/*******************************************************************************
****版本：V1.0.0
****平台：STM8S003
****日期：2021-01-12
****作者：Qitas
****版权：OS-Q
*******************************************************************************/
#include <stdint.h>
#include "stm8s.h"
#include "init.h"
#include "pcf8574.h"

/* Interrupt handler uses this to transfer data. */
static volatile i2c_cmd_p _i2c_cmd_p;

#define __delay() {\
    uint32_t i;\
    for(i = 0; i < 40000; i++) \
        __asm nop __endasm;\
}



/*******************************************************************************
**函数信息 ：
**功能描述 ：
**输入参数 ：
**输出参数 ：
*******************************************************************************/
void i2c_send_cmd(i2c_cmd_t *cmd)
{
    _i2c_cmd_p = cmd;
    /* Switch to master mode. */
    I2C_CR2 |= I2C_CR2_START;
}

/*******************************************************************************
**函数信息 ：
**功能描述 ：
**输入参数 ：
**输出参数 ：
*******************************************************************************/
int main()
{
    i2c_cmd_t pcf85741_cmd;

    port_init();
    clk_init();
    i2c_init();

    pcf85741_cmd.address = PCF85741A_ADDR_MASK(0b000);
    pcf85741_cmd.data = 0xAA;

    while (1) {
        for (pcf85741_cmd.data = 0; pcf85741_cmd.data < 0xFF; pcf85741_cmd.data++) {
            i2c_send_cmd(&pcf85741_cmd);
            __delay();
        }
    }
}
/*******************************************************************************
**函数信息 ：
**功能描述 ：
**输入参数 ：
**输出参数 ：
*******************************************************************************/
/* I2C interrupt service routine for master transfer */
void i2c_isr(void) __interrupt(INT_I2C) {
    uint8_t r;
    if (I2C_SR1 & I2C_SR1_SB) {
        /* Start condition bit set, generate start signal and
         * start sending address */
        r = I2C_SR1;
        /* Write slave address to data register */
        I2C_DR = (_i2c_cmd_p->address << 1) | I2C_WRITE;
    } else if (I2C_SR1 & I2C_SR1_ADDR) {
        /* Address sent, ACK received, now send data */
        r = I2C_SR1;
        r = I2C_SR3;
        I2C_DR = _i2c_cmd_p->data;
    } else if (I2C_SR1 & I2C_SR1_TXE) {
        /* Transfer ready, generate stop signal */
        I2C_CR2 |= I2C_CR2_STOP;
    }
}

/*---------------------------(C) COPYRIGHT 2021 OS-Q -------------------------*/

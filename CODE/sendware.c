/*
 * sendware.c
 *
 *  Created on: 2022年10月15日
 *      Author: Lenovo
 */

#include "headfile.h"
//山外波形函数
void vcan_sendware(void *wareaddr, uint32 waresize)
{
#define VCAN_PORT UART_0
#define CMD_WARE     3

    uint8 cmdf[2] = {CMD_WARE, ~CMD_WARE};
    uint8 cmdr[2] = {~CMD_WARE, CMD_WARE};

    uart_putbuff(VCAN_PORT, cmdf, sizeof(cmdf));
    uart_putbuff(VCAN_PORT, (uint8 *) wareaddr, waresize);
    uart_putbuff(VCAN_PORT, cmdr, sizeof(cmdr));
}






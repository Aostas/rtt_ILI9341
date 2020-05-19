#ifndef __ILI9341_PORT_H__
#define __ILI9341_PORT_H__

#include <board.h>
#include "ILI9341.h"

/*LCD MPU保护参数*/
#define LCD_REGION_NUMBER       MPU_REGION_NUMBER0      //LCD使用region0
#define LCD_ADDRESS_START       (0X60000000)            //LCD区的首地址
#define LCD_REGION_SIZE         MPU_REGION_SIZE_256MB   //LCD区大小

/*LCD背光 PB5*/
#define LCD_BL_GPIO_NUM     GET_PIN(B, 5)
/*LCD地址结构体*/
typedef struct
{
    rt_uint16_t LCD_REG;
    rt_uint16_t LCD_RAM;
} LCD_TypeDef;

/*
 * 使用NOR/SRAM的 Bank1.sector4,地址位HADDR[27,26]=11 A18作为数据命令区分线
 * 注意设置时STM32内部会右移一位对其!
 */
#define LCD_BASE        ((rt_uint32_t)(0x60000000 | 0x0007FFFE))
#define LCD             ((LCD_TypeDef *) LCD_BASE)



#endif/*__ILI9341_PORT_H__*/

#ifndef __ILI9341_PORT_H__
#define __ILI9341_PORT_H__

#include <board.h>
#include "ILI9341.h"

/*LCD MPU��������*/
#define LCD_REGION_NUMBER       MPU_REGION_NUMBER0      //LCDʹ��region0
#define LCD_ADDRESS_START       (0X60000000)            //LCD�����׵�ַ
#define LCD_REGION_SIZE         MPU_REGION_SIZE_256MB   //LCD����С

/*LCD���� PB5*/
#define LCD_BL_GPIO_NUM     GET_PIN(B, 5)
/*LCD��ַ�ṹ��*/
typedef struct
{
    rt_uint16_t LCD_REG;
    rt_uint16_t LCD_RAM;
} LCD_TypeDef;

/*
 * ʹ��NOR/SRAM�� Bank1.sector4,��ַλHADDR[27,26]=11 A18��Ϊ��������������
 * ע������ʱSTM32�ڲ�������һλ����!
 */
#define LCD_BASE        ((rt_uint32_t)(0x60000000 | 0x0007FFFE))
#define LCD             ((LCD_TypeDef *) LCD_BASE)



#endif/*__ILI9341_PORT_H__*/

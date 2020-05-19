#include "ILI9341.h"
#include <string.h>


#define DBG_TAG               "ILI9341"
#ifdef RT_ILI9341_DEBUG
#define DBG_LVL               DBG_LOG
#else
#define DBG_LVL               DBG_INFO
#endif
#include <rtdbg.h>

struct lcd_device _lcd;

#define lcd_wr_cmd _lcd.ops->lcd_wr_reg
#define lcd_wr_data _lcd.ops->lcd_wr_data
#define lcd_rd_data _lcd.ops->lcd_rd_data
#define lcd_delay_us _lcd.ops->lcd_delay_us

static void lcd_delay_ms(rt_uint32_t ms){
    for(int i=0;i<ms;i++){
        lcd_delay_us(1000);
    }
}

static void lcd_wr_reg(rt_uint8_t reg_addr,rt_uint8_t data){
    lcd_wr_cmd(reg_addr);
    lcd_wr_data(data);
}

//static rt_uint8_t lcd_rd_reg(rt_uint8_t reg_addr){
//    lcd_wr_cmd(reg_addr);
//    //lcd_delay_us(5);
//    return lcd_rd_data();
//}

void lcd_set_scan_dir(rt_uint8_t dir){
    rt_uint16_t regval=0;
    rt_uint16_t dirreg=0;
    rt_uint16_t temp;
    if(_lcd.dir)
    {
        switch(dir)
        {
            case 0:dir=6;break;
            case 1:dir=7;break;
            case 2:dir=4;break;
            case 3:dir=5;break;
            case 4:dir=1;break;
            case 5:dir=0;break;
            case 6:dir=3;break;
            case 7:dir=2;break;
        }
    }
    switch(dir)
    {
        case L2R_U2D:
            regval|=(0<<7)|(0<<6)|(0<<5);
            break;
        case L2R_D2U:
            regval|=(1<<7)|(0<<6)|(0<<5);
            break;
        case R2L_U2D:
            regval|=(0<<7)|(1<<6)|(0<<5);
            break;
        case R2L_D2U:
            regval|=(1<<7)|(1<<6)|(0<<5);
            break;
        case U2D_L2R:
            regval|=(0<<7)|(0<<6)|(1<<5);
            break;
        case U2D_R2L:
            regval|=(0<<7)|(1<<6)|(1<<5);
            break;
        case D2U_L2R:
            regval|=(1<<7)|(0<<6)|(1<<5);
            break;
        case D2U_R2L:
            regval|=(1<<7)|(1<<6)|(1<<5);
            break;
    }
    dirreg=0X36;
    regval|=0X08;
    lcd_wr_reg(dirreg,regval);
    if(regval&0X20)
    {
        if(_lcd.width<_lcd.height)
        {
            temp=_lcd.width;
            _lcd.width=_lcd.height;
            _lcd.height=temp;
        }
    }else
    {
        if(_lcd.width>_lcd.height)
        {
            temp=_lcd.width;
            _lcd.width=_lcd.height;
            _lcd.height=temp;
        }
    }
    lcd_wr_cmd(_lcd.setxcmd);
    lcd_wr_data(0);lcd_wr_data(0);
    lcd_wr_data((_lcd.width-1)>>8);
    lcd_wr_data((_lcd.width-1)&0XFF);
    lcd_wr_cmd(_lcd.setycmd);
    lcd_wr_data(0);lcd_wr_data(0);
    lcd_wr_data((_lcd.height-1)>>8);
    lcd_wr_data((_lcd.height-1)&0XFF);
}

void lcd_set_display_dir(rt_uint8_t dir){
    if(dir==0)
    {
        _lcd.dir=0;
        _lcd.width=240;
        _lcd.height=320;
        if(_lcd.id==0X9341)
        {
            _lcd.wramcmd=0X2C;
            _lcd.setxcmd=0X2A;
            _lcd.setycmd=0X2B;
            if(_lcd.id==0X5310)
            {
                _lcd.width=320;
                _lcd.height=480;
            }
        }
    }else
    {
        _lcd.dir=1;
        _lcd.width=320;
        _lcd.height=240;
        if(_lcd.id==0X9341)
        {
            _lcd.wramcmd=0X2C;
            _lcd.setxcmd=0X2A;
            _lcd.setycmd=0X2B;
        }
    }
    lcd_set_scan_dir(DFT_SCAN_DIR);
}

void lcd_set_cursor(rt_uint16_t xpos, rt_uint16_t ypos){
    lcd_wr_cmd(_lcd.setxcmd);
    lcd_wr_data(xpos>>8);
    lcd_wr_data(xpos&0XFF);
    lcd_wr_cmd(_lcd.setycmd);
    lcd_wr_data(ypos>>8);
    lcd_wr_data(ypos&0XFF);
}

void lcd_clear(rt_uint32_t color){
    rt_uint32_t index=0;
    rt_uint32_t totalpoint=_lcd.width;
    totalpoint*=_lcd.height;
    lcd_set_cursor(0x00, 0x0000);
    lcd_wr_cmd(_lcd.wramcmd);
    for(index=0;index<totalpoint;index++)
    {
        lcd_wr_data(color);
    }
}
void lcd_set_window(rt_uint16_t sx,rt_uint16_t sy,rt_uint16_t width,rt_uint16_t height)
{
    rt_uint16_t twidth,theight;
    twidth=sx+width-1;
    theight=sy+height-1;
    lcd_wr_cmd(_lcd.setxcmd);
    lcd_wr_data(sx>>8);
    lcd_wr_data(sx&0XFF);
    lcd_wr_data(twidth>>8);
    lcd_wr_data(twidth&0XFF);
    lcd_wr_cmd(_lcd.setycmd);
    lcd_wr_data(sy>>8);
    lcd_wr_data(sy&0XFF);
    lcd_wr_data(theight>>8);
    lcd_wr_data(theight&0XFF);
}

rt_err_t lcd_reg_init(void){
    /*³¢ÊÔ9341 IDµÄ¶ÁÈ¡*/
    lcd_wr_cmd(0XD3);
    _lcd.id=lcd_rd_data();
    _lcd.id=lcd_rd_data();
    _lcd.id=lcd_rd_data();
    _lcd.id<<=8;
    _lcd.id|=lcd_rd_data();

    LOG_D("LCD ID:%X",_lcd.id);
    if(_lcd.id!=0X9341)
    {
        LOG_E("LCD ID:%X",_lcd.id);
        return -RT_ERROR;
    }
    lcd_wr_cmd(0xCF);
    lcd_wr_data(0x00);
    lcd_wr_data(0xC1);
    lcd_wr_data(0X30);
    lcd_wr_cmd(0xED);
    lcd_wr_data(0x64);
    lcd_wr_data(0x03);
    lcd_wr_data(0X12);
    lcd_wr_data(0X81);
    lcd_wr_cmd(0xE8);
    lcd_wr_data(0x85);
    lcd_wr_data(0x10);
    lcd_wr_data(0x7A);
    lcd_wr_cmd(0xCB);
    lcd_wr_data(0x39);
    lcd_wr_data(0x2C);
    lcd_wr_data(0x00);
    lcd_wr_data(0x34);
    lcd_wr_data(0x02);
    lcd_wr_cmd(0xF7);
    lcd_wr_data(0x20);
    lcd_wr_cmd(0xEA);
    lcd_wr_data(0x00);
    lcd_wr_data(0x00);
    lcd_wr_cmd(0xC0);    //Power control
    lcd_wr_data(0x1B);   //VRH[5:0]
    lcd_wr_cmd(0xC1);    //Power control
    lcd_wr_data(0x01);   //SAP[2:0];BT[3:0]
    lcd_wr_cmd(0xC5);    //VCM control
    lcd_wr_data(0x30);   //3F
    lcd_wr_data(0x30);   //3C
    lcd_wr_cmd(0xC7);    //VCM control2
    lcd_wr_data(0XB7);
    lcd_wr_cmd(0x36);    // Memory Access Control
    lcd_wr_data(0x48);
    lcd_wr_cmd(0x3A);
    lcd_wr_data(0x55);
    lcd_wr_cmd(0xB1);
    lcd_wr_data(0x00);
    lcd_wr_data(0x1A);
    lcd_wr_cmd(0xB6);    // Display Function Control
    lcd_wr_data(0x0A);
    lcd_wr_data(0xA2);
    lcd_wr_cmd(0xF2);    // 3Gamma Function Disable
    lcd_wr_data(0x00);
    lcd_wr_cmd(0x26);    //Gamma curve selected
    lcd_wr_data(0x01);
    lcd_wr_cmd(0xE0);    //Set Gamma
    lcd_wr_data(0x0F);
    lcd_wr_data(0x2A);
    lcd_wr_data(0x28);
    lcd_wr_data(0x08);
    lcd_wr_data(0x0E);
    lcd_wr_data(0x08);
    lcd_wr_data(0x54);
    lcd_wr_data(0XA9);
    lcd_wr_data(0x43);
    lcd_wr_data(0x0A);
    lcd_wr_data(0x0F);
    lcd_wr_data(0x00);
    lcd_wr_data(0x00);
    lcd_wr_data(0x00);
    lcd_wr_data(0x00);
    lcd_wr_cmd(0XE1);    //Set Gamma
    lcd_wr_data(0x00);
    lcd_wr_data(0x15);
    lcd_wr_data(0x17);
    lcd_wr_data(0x07);
    lcd_wr_data(0x11);
    lcd_wr_data(0x06);
    lcd_wr_data(0x2B);
    lcd_wr_data(0x56);
    lcd_wr_data(0x3C);
    lcd_wr_data(0x05);
    lcd_wr_data(0x10);
    lcd_wr_data(0x0F);
    lcd_wr_data(0x3F);
    lcd_wr_data(0x3F);
    lcd_wr_data(0x0F);
    lcd_wr_cmd(0x2B);
    lcd_wr_data(0x00);
    lcd_wr_data(0x00);
    lcd_wr_data(0x01);
    lcd_wr_data(0x3f);
    lcd_wr_cmd(0x2A);
    lcd_wr_data(0x00);
    lcd_wr_data(0x00);
    lcd_wr_data(0x00);
    lcd_wr_data(0xef);
    lcd_wr_cmd(0x11); //Exit Sleep
    lcd_delay_ms(120);
    lcd_wr_cmd(0x29); //display on
    lcd_set_display_dir(0);

    return RT_EOK;
}

static rt_err_t lcd_init(struct rt_device *device){
    lcd_device_t lcd = (lcd_device_t)device;
    /* nothing, right now */
    lcd = lcd;
    return RT_EOK;
}

static rt_err_t lcd_control(struct rt_device *device, int cmd, void *args){
    switch (cmd)
    {
        case LCD_CTRL_SET_BK:
        {
            RT_ASSERT(args);
            rt_uint8_t *val=(rt_uint8_t *)args;
            _lcd.ops->lcd_set_bk(*val);
            break;
        }
        case LCD_CTRL_CLEAR:
        {
            RT_ASSERT(args);
            rt_uint32_t *val=(rt_uint32_t *)args;
            lcd_clear(*val);
            break;
        }
        case LCD_CTRL_SET_CURSOR:
        {
            lcd_set_window(0,0,240,240);
            lcd_set_cursor(0, 0);
            break;
        }
        case LCD_CTRL_START_GRAM:
        {
            lcd_wr_cmd(_lcd.wramcmd);
            break;
        }
    }
    return RT_EOK;
}

#ifdef RT_USING_DEVICE_OPS
const static struct rt_device_ops lcd_ops =
{
    lcd_init,
    RT_NULL,
    RT_NULL,
    RT_NULL,
    RT_NULL,
    lcd_control
};
#endif

rt_err_t lcd_hw_init(struct lcd_ops *ops){
    rt_err_t result = RT_EOK;
    struct rt_device *device = &_lcd.parent;
    RT_ASSERT(ops);
    memset(&_lcd, 0x00, sizeof(_lcd));
    result = rt_sem_init(&_lcd.lcd_lock, "lcd_lock", 0, RT_IPC_FLAG_FIFO);
    if (result != RT_EOK)
    {
        LOG_E("init semaphore failed!\n");
        result = -RT_ENOMEM;
        goto __exit;
    }
    device->type    = RT_Device_Class_Graphic;
#ifdef RT_USING_DEVICE_OPS
    device->ops     = &lcd_ops;
#else
    device->init    = lcd_init;
    device->control = lcd_control;
#endif
    rt_device_register(device, "lcd", RT_DEVICE_FLAG_RDWR);
    _lcd.ops=ops;
    if (_lcd.ops->lcd_hal_init() != RT_EOK)
    {
        result = -RT_ERROR;
        goto __exit;
    }
    lcd_delay_ms(50);
    if (lcd_reg_init() != RT_EOK)
    {
        result = -RT_ERROR;
        goto __exit;
    }


__exit:
    if (result != RT_EOK)
    {
        rt_sem_delete(&_lcd.lcd_lock);
    }
    return result;
}

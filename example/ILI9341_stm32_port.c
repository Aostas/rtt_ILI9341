#include "ILI9341_stm32_port.h"

#define DBG_TAG               "ILI9341"

#ifdef PKG_ILI9341_DEBUG
#define DBG_LVL               DBG_LOG
#else
#define DBG_LVL               DBG_INFO
#endif
#include <rtdbg.h>

void lcd_wr_reg(rt_uint16_t val) {
    LCD->LCD_REG=val;
}

void lcd_wr_data(rt_uint16_t val) {
    LCD->LCD_RAM=val;
}

rt_uint16_t lcd_rd_data(void){
    return LCD->LCD_RAM;
}

void lcd_set_bk(rt_uint8_t val)
{
    rt_pin_mode(LCD_BL_GPIO_NUM, PIN_MODE_OUTPUT);
    rt_pin_write(LCD_BL_GPIO_NUM, val);
}

SRAM_HandleTypeDef hsram1;
rt_err_t lcd_hal_init(void){
    FMC_NORSRAM_TimingTypeDef Timing;
    FMC_NORSRAM_TimingTypeDef ExtTiming;
    MPU_Region_InitTypeDef MPU_Initure;

    HAL_MPU_Disable();                          //配置MPU之前先关闭MPU,配置完成以后在使能MPU
    //外部SRAM为region0，大小为2MB，此区域可读写
    MPU_Initure.Enable=MPU_REGION_ENABLE;       //使能region
    MPU_Initure.Number=LCD_REGION_NUMBER;       //设置region，外部SRAM使用的region0
    MPU_Initure.BaseAddress=LCD_ADDRESS_START;  //region基地址
    MPU_Initure.Size=LCD_REGION_SIZE;           //region大小
    MPU_Initure.SubRegionDisable=0X00;
    MPU_Initure.TypeExtField=MPU_TEX_LEVEL0;
    MPU_Initure.AccessPermission=MPU_REGION_FULL_ACCESS;    //此region可读写
    MPU_Initure.DisableExec=MPU_INSTRUCTION_ACCESS_ENABLE;  //允许读取此区域中的指令
    MPU_Initure.IsShareable=MPU_ACCESS_NOT_SHAREABLE;
    MPU_Initure.IsCacheable=MPU_ACCESS_NOT_CACHEABLE;
    MPU_Initure.IsBufferable=MPU_ACCESS_BUFFERABLE;
    HAL_MPU_ConfigRegion(&MPU_Initure);
    HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);     //开启MPU


    /** Perform the SRAM1 memory initialization sequence
    */
    hsram1.Instance = FMC_NORSRAM_DEVICE;
    hsram1.Extended = FMC_NORSRAM_EXTENDED_DEVICE;
    /* hsram1.Init */
    hsram1.Init.NSBank = FMC_NORSRAM_BANK1;
    hsram1.Init.DataAddressMux = FMC_DATA_ADDRESS_MUX_DISABLE;
    hsram1.Init.MemoryType = FMC_MEMORY_TYPE_SRAM;
    hsram1.Init.MemoryDataWidth = FMC_NORSRAM_MEM_BUS_WIDTH_16;
    hsram1.Init.BurstAccessMode = FMC_BURST_ACCESS_MODE_DISABLE;
    hsram1.Init.WaitSignalPolarity = FMC_WAIT_SIGNAL_POLARITY_LOW;
    hsram1.Init.WaitSignalActive = FMC_WAIT_TIMING_BEFORE_WS;
    hsram1.Init.WriteOperation = FMC_WRITE_OPERATION_ENABLE;
    hsram1.Init.WaitSignal = FMC_WAIT_SIGNAL_DISABLE;
    hsram1.Init.ExtendedMode = FMC_EXTENDED_MODE_ENABLE;
    hsram1.Init.AsynchronousWait = FMC_ASYNCHRONOUS_WAIT_DISABLE;
    hsram1.Init.WriteBurst = FMC_WRITE_BURST_DISABLE;
    hsram1.Init.ContinuousClock = FMC_CONTINUOUS_CLOCK_SYNC_ONLY;
    hsram1.Init.WriteFifo = FMC_WRITE_FIFO_ENABLE;
    hsram1.Init.PageSize = FMC_PAGE_SIZE_NONE;
    /* Timing */
    Timing.AddressSetupTime = 15;
    Timing.AddressHoldTime = 0;
    Timing.DataSetupTime = 80;
    Timing.BusTurnAroundDuration = 15;
    Timing.CLKDivision = 16;
    Timing.DataLatency = 17;
    Timing.AccessMode = FMC_ACCESS_MODE_A;
    /* ExtTiming */
    ExtTiming.AddressSetupTime = 5;
    ExtTiming.AddressHoldTime = 0;
    ExtTiming.DataSetupTime = 5;
    ExtTiming.BusTurnAroundDuration = 15;
    ExtTiming.CLKDivision = 16;
    ExtTiming.DataLatency = 17;
    ExtTiming.AccessMode = FMC_ACCESS_MODE_A;
    if (HAL_SRAM_Init(&hsram1, &Timing, &ExtTiming) != HAL_OK)
    {
        return -RT_ERROR;
    }else{
        LOG_D("SRAM init success");
    }
    return RT_EOK;
}

static struct lcd_ops stm32_ops={
        .lcd_hal_init=lcd_hal_init,
        .lcd_wr_reg=lcd_wr_reg,
        .lcd_wr_data=lcd_wr_data,
        .lcd_rd_data=lcd_rd_data,
        .lcd_set_bk=lcd_set_bk,
        .lcd_delay_us=rt_hw_us_delay
};

int lcd_dev_init(void){
    return lcd_hw_init(&stm32_ops);
}
//INIT_DEVICE_EXPORT(lcd_dev_init);

//int lcd_dev_test(void){
//    rt_uint8_t bk_en=0;
//    rt_uint32_t color=0;
//    rt_device_t dev=rt_device_find("lcd");
//    if(dev == RT_NULL){
//        LOG_E("lcd device not found!");
//        return -RT_EEMPTY;
//    }
//    if (rt_device_open(dev, RT_NULL) != RT_EOK)
//    {
//        LOG_E("lcd device opened failed!");
//        return -RT_ERROR;
//    }
//    bk_en=1;
//    rt_device_control(dev, LCD_CTRL_SET_BK, &bk_en);
//    color=RED;
//    rt_device_control(dev, LCD_CTRL_CLEAR, &color);
//    return RT_EOK;
//}
//
//INIT_APP_EXPORT(lcd_dev_test);


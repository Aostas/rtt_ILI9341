#ifndef __ILI9341_H__
#define __ILI9341_H__

#include <rtthread.h>
#include <rtdevice.h>

#define LCD_CTRL_SET_BK         0
#define LCD_CTRL_CLEAR          1
#define LCD_CTRL_SET_CURSOR     2
#define LCD_CTRL_START_GRAM     3

/*ɨ�跽����*/
#define L2R_U2D  0      //������,���ϵ���
#define L2R_D2U  1      //������,���µ���
#define R2L_U2D  2      //���ҵ���,���ϵ���
#define R2L_D2U  3      //���ҵ���,���µ���

#define U2D_L2R  4      //���ϵ���,������
#define U2D_R2L  5      //���ϵ���,���ҵ���
#define D2U_L2R  6      //���µ���,������
#define D2U_R2L  7      //���µ���,���ҵ���

#define DFT_SCAN_DIR  L2R_U2D  //Ĭ�ϵ�ɨ�跽��

/*������ɫ*/
#define WHITE            0xFFFF
#define BLACK            0x0000
#define BLUE             0x001F
#define BRED             0XF81F
#define GRED             0XFFE0
#define GBLUE            0X07FF
#define RED              0xF800
#define MAGENTA          0xF81F
#define GREEN            0x07E0
#define CYAN             0x7FFF
#define YELLOW           0xFFE0
#define BROWN            0XBC40 //��ɫ
#define BRRED            0XFC07 //�غ�ɫ
#define GRAY             0X8430 //��ɫ
/*GUI��ɫ*/
#define DARKBLUE         0X01CF //����ɫ
#define LIGHTBLUE        0X7D7C //ǳ��ɫ
#define GRAYBLUE         0X5458 //����ɫ
/*������ɫΪPANEL����ɫ*/

#define LIGHTGREEN       0X841F //ǳ��ɫ
//#define LIGHTGRAY        0XEF5B //ǳ��ɫ(PANNEL)
#define LGRAY            0XC618 //ǳ��ɫ(PANNEL),���屳��ɫ

#define LGRAYBLUE        0XA651 //ǳ����ɫ(�м����ɫ)
#define LBBLUE           0X2B12 //ǳ����ɫ(ѡ����Ŀ�ķ�ɫ)

/*LCD�ֱ�������*/
#define SSD_HOR_RESOLUTION      800     //LCDˮƽ�ֱ���
#define SSD_VER_RESOLUTION      480     //LCD��ֱ�ֱ���
/*LCD������������*/
#define SSD_HOR_PULSE_WIDTH     1       //ˮƽ����
#define SSD_HOR_BACK_PORCH      46      //ˮƽǰ��
#define SSD_HOR_FRONT_PORCH     210     //ˮƽ����

#define SSD_VER_PULSE_WIDTH     1       //��ֱ����
#define SSD_VER_BACK_PORCH      23      //��ֱǰ��
#define SSD_VER_FRONT_PORCH     22      //��ֱǰ��
/*���¼����������Զ�����*/
#define SSD_HT  (SSD_HOR_RESOLUTION+SSD_HOR_BACK_PORCH+SSD_HOR_FRONT_PORCH)
#define SSD_HPS (SSD_HOR_BACK_PORCH)
#define SSD_VT  (SSD_VER_RESOLUTION+SSD_VER_BACK_PORCH+SSD_VER_FRONT_PORCH)
#define SSD_VPS (SSD_VER_BACK_PORCH)

struct lcd_ops{
    rt_err_t (*lcd_hal_init)(void);
    void (*lcd_set_bk)(rt_uint8_t val);
    void (*lcd_wr_reg)(rt_uint16_t val);
    void (*lcd_wr_data)(rt_uint16_t val);
    rt_uint16_t (*lcd_rd_data)(void);
    void (*lcd_delay_us)(rt_uint32_t us);
};

struct lcd_device
{
    struct rt_device parent;
    struct rt_semaphore lcd_lock;
    struct lcd_ops *ops;
    rt_uint16_t width;
    rt_uint16_t height;
    rt_uint16_t id;
    rt_uint8_t  dir;
    rt_uint16_t wramcmd;
    rt_uint16_t setxcmd;
    rt_uint16_t setycmd;
};
typedef struct lcd_device *lcd_device_t;
rt_err_t lcd_hw_init(struct lcd_ops *ops);

#endif/*__ILI9341_H__*/

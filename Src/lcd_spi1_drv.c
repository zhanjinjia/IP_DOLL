#include "lcd_spi1_drv.h"
#include "gpio.h"
#include "spi.h"
#include "bmp.h"

/**
 *@brief    LCD控制引脚和通信接口初始化
 *@param    none
 *@retval   none
*/
static void LCD_GPIO_Init(void)
{
    /* 初始化引脚 */
	MX_GPIO_Init();
		
	/* 复位LCD */
    LCD_PWR(0);
    LCD_RST(0);
    HAL_Delay(100);
    LCD_RST(1);

	/* 初始化SPI1接口 */
    MX_SPI1_Init();
}



/**
 * @brief   写命令到LCD
 * @param   cmd —— 需要发送的命令
 * @return  none
 */
static void LCD_Write_Cmd(uint8_t cmd)
{
    LCD_WR(0);
    SPI_WriteByte(&cmd, 1);
}


/**
 * @brief   写数据到LCD
 * @param   dat —— 需要发送的数据
 * @return  none
 */
static void LCD_Write_Data(uint8_t dat)
{
    LCD_WR(1);
    SPI_WriteByte(&dat, 1);
}

/**
 * @breif   打开LCD显示背光
 * @param   none
 * @return  none
 */
void LCD_DisplayOn(void)
{
    LCD_PWR(1);
}
/**
 * @brief   关闭LCD显示背光
 * @param   none
 * @return  none
 */
void LCD_DisplayOff(void)
{
    LCD_PWR(0);
}

/**
 * @brief   设置数据写入LCD显存区域
 * @param   x1,y1	—— 起点坐标
 * @param   x2,y2	—— 终点坐标
 * @return  none
 */
void LCD_Address_Set(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
    /* 指定X方向操作区域 */
    LCD_Write_Cmd(0x2a);
    LCD_Write_Data(x1 >> 8);
    LCD_Write_Data(x1);
    LCD_Write_Data(x2 >> 8);
    LCD_Write_Data(x2);

    /* 指定Y方向操作区域 */
    LCD_Write_Cmd(0x2b);
    LCD_Write_Data(y1 >> 8);
    LCD_Write_Data(y1);
    LCD_Write_Data(y2 >> 8);
    LCD_Write_Data(y2);

    /* 发送该命令，LCD开始等待接收显存数据 */
    LCD_Write_Cmd(0x2C);
}


/**
 * @brief   以一种颜色清空LCD屏
 * @param   color —— 清屏颜色(16bit)
 * @return  none
 */
void LCD_Clear(uint16_t color)
{
    uint16_t i, j;
    uint8_t data[2] = {0};  //color是16bit的，每个像素点需要两个字节的显存

    /* 将16bit的color值分开为两个单独的字节 */
    data[0] = color >> 8;
    data[1] = color;
    
    /* 显存的值需要逐字节写入 */
    for(j = 0; j < LCD_Buf_Size / 2; j++)
    {
        lcd_buf[j * 2] =  data[0];
        lcd_buf[j * 2 + 1] =  data[1];
    }
    /* 指定显存操作地址为全屏幕 */
    LCD_Address_Set(0, 0, LCD_Width - 1, LCD_Height - 1);
    /* 指定接下来的数据为数据 */
    LCD_WR(1);
    /* 将显存缓冲区的数据全部写入缓冲区 */
    for(i = 0; i < (LCD_TOTAL_BUF_SIZE / LCD_Buf_Size); i++)
    {
        SPI_WriteByte(lcd_buf, (uint16_t)LCD_Buf_Size);
    }
}

/**
 * @brief   LCD初始化
 * @param   none
 * @return  none
 */
void LCD_Init(void)
{
    /* 初始化和LCD通信的引脚 */
    LCD_GPIO_Init();
    HAL_Delay(120);
	
    /* 关闭睡眠模式 */
    LCD_Write_Cmd(0x11);
    HAL_Delay(120);

    /* 开始设置显存扫描模式，数据格式等 */
    LCD_Write_Cmd(0x36);
    LCD_Write_Data(0x00);
    /* RGB 5-6-5-bit格式  */
    LCD_Write_Cmd(0x3A);
    LCD_Write_Data(0x65);
    /* porch 设置 */
    LCD_Write_Cmd(0xB2);
    LCD_Write_Data(0x0C);
    LCD_Write_Data(0x0C);
    LCD_Write_Data(0x00);
    LCD_Write_Data(0x33);
    LCD_Write_Data(0x33);
    /* VGH设置 */
    LCD_Write_Cmd(0xB7);
    LCD_Write_Data(0x72);
    /* VCOM 设置 */
    LCD_Write_Cmd(0xBB);
    LCD_Write_Data(0x3D);
    /* LCM 设置 */
    LCD_Write_Cmd(0xC0);
    LCD_Write_Data(0x2C);
    /* VDV and VRH 设置 */
    LCD_Write_Cmd(0xC2);
    LCD_Write_Data(0x01);
    /* VRH 设置 */
    LCD_Write_Cmd(0xC3);
    LCD_Write_Data(0x19);
    /* VDV 设置 */
    LCD_Write_Cmd(0xC4);
    LCD_Write_Data(0x20);
    /* 普通模式下显存速率设置 60Mhz */
    LCD_Write_Cmd(0xC6);
    LCD_Write_Data(0x0F);
    /* 电源控制 */
    LCD_Write_Cmd(0xD0);
    LCD_Write_Data(0xA4);
    LCD_Write_Data(0xA1);
    /* 电压设置 */
    LCD_Write_Cmd(0xE0);
    LCD_Write_Data(0xD0);
    LCD_Write_Data(0x04);
    LCD_Write_Data(0x0D);
    LCD_Write_Data(0x11);
    LCD_Write_Data(0x13);
    LCD_Write_Data(0x2B);
    LCD_Write_Data(0x3F);
    LCD_Write_Data(0x54);
    LCD_Write_Data(0x4C);
    LCD_Write_Data(0x18);
    LCD_Write_Data(0x0D);
    LCD_Write_Data(0x0B);
    LCD_Write_Data(0x1F);
    LCD_Write_Data(0x23);
    /* 电压设置 */
    LCD_Write_Cmd(0xE1);
    LCD_Write_Data(0xD0);
    LCD_Write_Data(0x04);
    LCD_Write_Data(0x0C);
    LCD_Write_Data(0x11);
    LCD_Write_Data(0x13);
    LCD_Write_Data(0x2C);
    LCD_Write_Data(0x3F);
    LCD_Write_Data(0x44);
    LCD_Write_Data(0x51);
    LCD_Write_Data(0x2F);
    LCD_Write_Data(0x1F);
    LCD_Write_Data(0x1F);
    LCD_Write_Data(0x20);
    LCD_Write_Data(0x23);
    /* 显示开 */
    LCD_Write_Cmd(0x21);
    LCD_Write_Cmd(0x29);
    
    /* 清屏为白色 */
    LCD_Clear(WHITE);

    /*打开显示*/
    LCD_PWR(1);
}

/**
 * @brief		显示图片函数
 * @param   x,y		起点坐标
 * @param   width	图片宽度
 * @param   height	图片高度
 * @param   p —— 图片缓存数据起始地址
 * @note		Image2Lcd取模方式：C语言数据/水平扫描/16位真彩色(RGB565)/高位在前		其他的不要选
 * @return  none
 */
void LCD_Show_Image(uint16_t x, uint16_t y, uint16_t width, uint16_t height, const uint8_t *p)
{
		uint32_t img_size = width * height * 2;		//图片所占字节数
		uint32_t remain_size = img_size;									//图片每次发送后剩余的字节数
		uint8_t i = 0;
	
		/* 错误检测 */
    if(x + width > LCD_Width || y + height > LCD_Height)
    {
        return;
    }
				
    LCD_Address_Set(x, y, x + width - 1, y + height - 1);

    LCD_WR(1);

		/* SPI每次最大发送2^16 = 65536个数据,图片最大大小为240*240*2 = 115200，会超过此大小，所以设计循环发送算法 */
		for(i = 0;i <= img_size / 65536; i++)
		{
			if(remain_size / 65536 >= 1)
			{
				SPI_WriteByte((uint8_t *)p, 65535);
				p += 65535;
				remain_size -= 65535;
			}
			else
			{
				SPI_WriteByte((uint8_t *)p, remain_size % 65535);
			}		
		}  
}

void blink_expression(void)
{
	LCD_Show_Image(0,0,240,240,gImage_01);
	HAL_Delay(200);
	LCD_Show_Image(0,0,240,240,gImage_02);	
	HAL_Delay(25);
	LCD_Show_Image(0,0,240,240,gImage_03);	
	HAL_Delay(25);
	LCD_Show_Image(0,0,240,240,gImage_04);
	HAL_Delay(25);
	LCD_Show_Image(0,0,240,240,gImage_05);
	HAL_Delay(325);
	
	LCD_Show_Image(0,0,240,240,gImage_01);
	HAL_Delay(50);
	LCD_Show_Image(0,0,240,240,gImage_02);	
	HAL_Delay(25);
	LCD_Show_Image(0,0,240,240,gImage_05);
	HAL_Delay(25);
	
	LCD_Show_Image(0,0,240,240,gImage_01);
	HAL_Delay(50);
	LCD_Show_Image(0,0,240,240,gImage_02);	
	HAL_Delay(25);
	LCD_Show_Image(0,0,240,240,gImage_05);
	HAL_Delay(825);
		
	LCD_Show_Image(0,0,240,240,gImage_01);
	HAL_Delay(50);
	LCD_Show_Image(0,0,240,240,gImage_02);	
	HAL_Delay(25);
	LCD_Show_Image(0,0,240,240,gImage_05);
	HAL_Delay(825);
	
	LCD_Show_Image(0,0,240,240,gImage_01);
	HAL_Delay(50);
	LCD_Show_Image(0,0,240,240,gImage_02);	
	HAL_Delay(25);
	LCD_Show_Image(0,0,240,240,gImage_05);
	HAL_Delay(825);
	
	LCD_Show_Image(0,0,240,240,gImage_01);
	HAL_Delay(50);
	LCD_Show_Image(0,0,240,240,gImage_02);	
	HAL_Delay(25);
	LCD_Show_Image(0,0,240,240,gImage_05);
	HAL_Delay(25);
	
	LCD_Show_Image(0,0,240,240,gImage_01);
	HAL_Delay(50);
	LCD_Show_Image(0,0,240,240,gImage_02);	
	HAL_Delay(25);
	LCD_Show_Image(0,0,240,240,gImage_05);
	HAL_Delay(825);

	LCD_Show_Image(0,0,240,240,gImage_01);
	HAL_Delay(50);
	LCD_Show_Image(0,0,240,240,gImage_02);	
	HAL_Delay(25);
	LCD_Show_Image(0,0,240,240,gImage_05);
	HAL_Delay(550);
	
}








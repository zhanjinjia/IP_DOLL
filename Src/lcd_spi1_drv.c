#include "lcd_spi1_drv.h"
#include "gpio.h"
#include "spi.h"
#include "bmp.h"

/**
 *@brief    LCD�������ź�ͨ�Žӿڳ�ʼ��
 *@param    none
 *@retval   none
*/
static void LCD_GPIO_Init(void)
{
    /* ��ʼ������ */
	MX_GPIO_Init();
		
	/* ��λLCD */
    LCD_PWR(0);
    LCD_RST(0);
    HAL_Delay(100);
    LCD_RST(1);

	/* ��ʼ��SPI1�ӿ� */
    MX_SPI1_Init();
}



/**
 * @brief   д���LCD
 * @param   cmd ���� ��Ҫ���͵�����
 * @return  none
 */
static void LCD_Write_Cmd(uint8_t cmd)
{
    LCD_WR(0);
    SPI_WriteByte(&cmd, 1);
}


/**
 * @brief   д���ݵ�LCD
 * @param   dat ���� ��Ҫ���͵�����
 * @return  none
 */
static void LCD_Write_Data(uint8_t dat)
{
    LCD_WR(1);
    SPI_WriteByte(&dat, 1);
}

/**
 * @breif   ��LCD��ʾ����
 * @param   none
 * @return  none
 */
void LCD_DisplayOn(void)
{
    LCD_PWR(1);
}
/**
 * @brief   �ر�LCD��ʾ����
 * @param   none
 * @return  none
 */
void LCD_DisplayOff(void)
{
    LCD_PWR(0);
}

/**
 * @brief   ��������д��LCD�Դ�����
 * @param   x1,y1	���� �������
 * @param   x2,y2	���� �յ�����
 * @return  none
 */
void LCD_Address_Set(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
    /* ָ��X����������� */
    LCD_Write_Cmd(0x2a);
    LCD_Write_Data(x1 >> 8);
    LCD_Write_Data(x1);
    LCD_Write_Data(x2 >> 8);
    LCD_Write_Data(x2);

    /* ָ��Y����������� */
    LCD_Write_Cmd(0x2b);
    LCD_Write_Data(y1 >> 8);
    LCD_Write_Data(y1);
    LCD_Write_Data(y2 >> 8);
    LCD_Write_Data(y2);

    /* ���͸����LCD��ʼ�ȴ������Դ����� */
    LCD_Write_Cmd(0x2C);
}


/**
 * @brief   ��һ����ɫ���LCD��
 * @param   color ���� ������ɫ(16bit)
 * @return  none
 */
void LCD_Clear(uint16_t color)
{
    uint16_t i, j;
    uint8_t data[2] = {0};  //color��16bit�ģ�ÿ�����ص���Ҫ�����ֽڵ��Դ�

    /* ��16bit��colorֵ�ֿ�Ϊ�����������ֽ� */
    data[0] = color >> 8;
    data[1] = color;
    
    /* �Դ��ֵ��Ҫ���ֽ�д�� */
    for(j = 0; j < LCD_Buf_Size / 2; j++)
    {
        lcd_buf[j * 2] =  data[0];
        lcd_buf[j * 2 + 1] =  data[1];
    }
    /* ָ���Դ������ַΪȫ��Ļ */
    LCD_Address_Set(0, 0, LCD_Width - 1, LCD_Height - 1);
    /* ָ��������������Ϊ���� */
    LCD_WR(1);
    /* ���Դ滺����������ȫ��д�뻺���� */
    for(i = 0; i < (LCD_TOTAL_BUF_SIZE / LCD_Buf_Size); i++)
    {
        SPI_WriteByte(lcd_buf, (uint16_t)LCD_Buf_Size);
    }
}

/**
 * @brief   LCD��ʼ��
 * @param   none
 * @return  none
 */
void LCD_Init(void)
{
    /* ��ʼ����LCDͨ�ŵ����� */
    LCD_GPIO_Init();
    HAL_Delay(120);
	
    /* �ر�˯��ģʽ */
    LCD_Write_Cmd(0x11);
    HAL_Delay(120);

    /* ��ʼ�����Դ�ɨ��ģʽ�����ݸ�ʽ�� */
    LCD_Write_Cmd(0x36);
    LCD_Write_Data(0x00);
    /* RGB 5-6-5-bit��ʽ  */
    LCD_Write_Cmd(0x3A);
    LCD_Write_Data(0x65);
    /* porch ���� */
    LCD_Write_Cmd(0xB2);
    LCD_Write_Data(0x0C);
    LCD_Write_Data(0x0C);
    LCD_Write_Data(0x00);
    LCD_Write_Data(0x33);
    LCD_Write_Data(0x33);
    /* VGH���� */
    LCD_Write_Cmd(0xB7);
    LCD_Write_Data(0x72);
    /* VCOM ���� */
    LCD_Write_Cmd(0xBB);
    LCD_Write_Data(0x3D);
    /* LCM ���� */
    LCD_Write_Cmd(0xC0);
    LCD_Write_Data(0x2C);
    /* VDV and VRH ���� */
    LCD_Write_Cmd(0xC2);
    LCD_Write_Data(0x01);
    /* VRH ���� */
    LCD_Write_Cmd(0xC3);
    LCD_Write_Data(0x19);
    /* VDV ���� */
    LCD_Write_Cmd(0xC4);
    LCD_Write_Data(0x20);
    /* ��ͨģʽ���Դ��������� 60Mhz */
    LCD_Write_Cmd(0xC6);
    LCD_Write_Data(0x0F);
    /* ��Դ���� */
    LCD_Write_Cmd(0xD0);
    LCD_Write_Data(0xA4);
    LCD_Write_Data(0xA1);
    /* ��ѹ���� */
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
    /* ��ѹ���� */
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
    /* ��ʾ�� */
    LCD_Write_Cmd(0x21);
    LCD_Write_Cmd(0x29);
    
    /* ����Ϊ��ɫ */
    LCD_Clear(WHITE);

    /*����ʾ*/
    LCD_PWR(1);
}

/**
 * @brief		��ʾͼƬ����
 * @param   x,y		�������
 * @param   width	ͼƬ���
 * @param   height	ͼƬ�߶�
 * @param   p ���� ͼƬ����������ʼ��ַ
 * @note		Image2Lcdȡģ��ʽ��C��������/ˮƽɨ��/16λ���ɫ(RGB565)/��λ��ǰ		�����Ĳ�Ҫѡ
 * @return  none
 */
void LCD_Show_Image(uint16_t x, uint16_t y, uint16_t width, uint16_t height, const uint8_t *p)
{
		uint32_t img_size = width * height * 2;		//ͼƬ��ռ�ֽ���
		uint32_t remain_size = img_size;									//ͼƬÿ�η��ͺ�ʣ����ֽ���
		uint8_t i = 0;
	
		/* ������ */
    if(x + width > LCD_Width || y + height > LCD_Height)
    {
        return;
    }
				
    LCD_Address_Set(x, y, x + width - 1, y + height - 1);

    LCD_WR(1);

		/* SPIÿ�������2^16 = 65536������,ͼƬ����СΪ240*240*2 = 115200���ᳬ���˴�С���������ѭ�������㷨 */
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








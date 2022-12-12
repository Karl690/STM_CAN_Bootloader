#include "configure.h"
#ifdef USE_LCD
#include "gui.h"
#include "font.h"
uint16_t GUI_BUF[LCD_LINESIZE][MAX_LCD_SIZE] = {0};

void ResetGuiBuffer(uint16_t color)
{
	uint16_t i, j;
	for(i = 0; i < LCD_LINESIZE; i ++)
	{
		for(j = 0; j < LCD_WIDTH; j ++)
		{
			GUI_BUF[i][j] = color;
		}
	}
}
void DrawPixel(uint16_t X, uint16_t Y, uint16_t color)
{
	GUI_BUF[Y][X] = color;
}
void DrawHLine(uint16_t X1, uint16_t X2, uint16_t Y, uint16_t color)
{
	uint16_t i;
	if(Y >= LCD_HEIGHT) return;
	for(i = X1; i <= X2; i++)
	{
		if( i < LCD_WIDTH) GUI_BUF[Y][i] = color;
	}
}
void DrawVLine(uint16_t X, uint16_t Y1, uint16_t Y2, uint16_t color)
{
	uint16_t i;
	if(X >= LCD_WIDTH) return;
	for(i = Y1; i <= Y2; i++)
	{
		if( i < LCD_HEIGHT) GUI_BUF[i][X] = color;
	}
}
void DrawLine(int16_t X1, int16_t Y1, int16_t X2, int16_t Y2, uint16_t color)
{
	float m = 0;
	if((X2 - X1 == 0) && (Y2 - Y1 == 0)) {
		if(X2 >= 0 && X2 < LCD_WIDTH && Y2 >= 0 && Y2 < LCD_HEIGHT) {
			GUI_BUF[Y2][X2] = color;
		}
	}
	if(X2 - X1 == 0 ) {
		DrawVLine(X1, MIN(Y1, Y2), ABS(Y1 - Y2), color);
	}
	else if(Y2 - Y1 == 0) {
		//gui_draw_hline(gui_buf, _MIN(x1, x2), y1, _ABS((int32_t)(x1 - x2)), Color);
		//return ret;
	}
	//#ifndef _NN_
	m = (Y2- Y1) /(float)(X2 - X1);
	uint32_t xstart, xend;
	uint16_t y = 0;
	xstart = MIN(X1, X2);
	xend = MAX(X1, X2);

	for(uint32_t i = xstart; i < xend; i ++)
	{
		if(i < 0) break;
		if(i >= LCD_WIDTH) break;
		y = (uint16_t)((m * (i - X1) + Y1));
		if(y >= 0 && y < LCD_HEIGHT) {
			GUI_BUF[y][i] = color;
		}
	}
}

void DrawRect(uint16_t X1, uint16_t Y1, uint16_t X2, uint16_t Y2, uint16_t color)
{

	DrawVLine(X1, Y1, Y2, color);
	DrawVLine(X2, Y1, Y2, color);
	DrawHLine(X1, X2, Y1, color);
	DrawHLine(X1, X2, Y2, color);
}
void FillRect(uint16_t X1, uint16_t Y1, uint16_t X2, uint16_t Y2, uint16_t color)
{
	for(uint16_t i = Y1; i < Y2; i ++)
	{
		for(uint16_t j = X1; j < X2; j ++)
		{
			GUI_BUF[i][j] = color;
		}
	}
}
void DrawChar(uint16_t X, uint16_t Y, uint8_t chr, uint16_t color)
{
	//gui_draw_char(X, Y, chr, GUI_FONT_SIZE, color);

	uint8_t temp,t1,t;
	uint16_t y0=Y;
	uint16_t x0=X;
	uint32_t h,w;

	//uint16_t write[GUI_FONT_SIZE][GUI_FONT_SIZE==12?6:8];
	uint16_t count;

	w = LCD_WIDTH;
	h = LCD_LINESIZE;

	chr = chr - ' ';
	count = 0;
	extern const unsigned char asc2_1206[95][12];
	extern const unsigned char asc2_1608[95][16];
	for(t=0;t<GUI_FONT_SIZE;t++)
	{
		if(GUI_FONT_SIZE==12)temp=asc2_1206[chr][t];  //����1206����
		else temp=asc2_1608[chr][t];		 //����1608����

		for(t1=0;t1<8;t1++)
		{
			if(temp&0x80)
			{
				uint16_t yindex = (uint16_t)(y0+count);// * MAX_LCD_SIZE);
				uint16_t xindex = x0 + t/2;
				GUI_BUF[yindex][xindex] = color;
			}else {

			}
			count ++;
			if(count >= GUI_FONT_SIZE) count =0;

			temp<<=1;
			Y++;
			if(Y>=h)
			{
				return;
			}
			if((Y-y0)==GUI_FONT_SIZE)
			{
				Y=y0;
				X++;
				if(X>=w)
				{
					return;
				}
				break;
			}
		}
	}
}
void DrawString(uint16_t X, uint16_t Y, char *str, uint16_t color)
{
	uint16_t x0 = X;
	while((*str<='~')&&(*str>=' '))
	{
		if(X >= LCD_WIDTH){X = x0;Y += GUI_FONT_SIZE;}
		if(Y >= LCD_LINESIZE)break;//�˳�
		DrawChar(X, Y, *str, color);
		X+= GUI_FONT_SIZE/2;
		str++;
	}
}

#endif

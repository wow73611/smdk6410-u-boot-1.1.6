//
// Copyright (c) Microsoft Corporation.  All rights reserved.
//
//
// Use of this source code is subject to the terms of the Microsoft end-user
// license agreement (EULA) under which you licensed this SOFTWARE PRODUCT.
// If you did not accept the terms of the EULA, you are not authorized to use
// this source code. For a copy of the EULA, please see the LICENSE.RTF on your
// install media.
//
/*****************************************************************************
* FileName: lcddisp.C
*
* Copyright (c) 2007 DMATEK Corporation
* All rights reserved.
* This code may not be redistributed in source or linkable object form
* without the consent of its author.
*
* Description:
*
*       LCD display routines.
*
******************************************************************************/


/*
    Modifier      Sign       Date             Function Description           
------------------------------------------------------------------------------------------------------
      chris	 chris    2007/03/30
*/
#include <common.h>
#include <stdarg.h>
#include <linux/types.h>
#include <linux/string.h>
#include <linux/ctype.h>

#include <lcddisp.h>
#include <regs.h>
#include <asm/io.h>
#include <lcdlib.h>
#include "asc.h"


U16 (*frameBuffer16Bit480272Tft)[SCR_XSIZE_TFT_480272];
//U16 (*frameBuffer16Bit240X320Tft)[SCR_XSIZE_TFT_240320];
//U16 (*frameBuffer16Bit320240Tft)[SCR_XSIZE_TFT_320240];
//U16 (*frameBuffer16Bit640480Tft)[SCR_XSIZE_TFT_640480];
//U16 (*frameBuffer16Bit800480Tft)[SCR_XSIZE_TFT_800480];
dmatek_lcd_info mach_info;


/*ÆÁÄ»¼°×ÖÌå´óÐ¡²ÎÊý*/
#define LCD_X_SIZE 	480
#define LCD_Y_SIZE 	272
#define FONT_X_SIZE 8
#define FONT_Y_SIZE 16
#define FONT_X_NUM  28
#define FONT_Y_NUM  20

/*LCD Ç°¾°É«*/
#define FONT_FRONT_YELLOW 		0xff80

/*LCD ±³¾°É«*/
#define FONT_GND_BLUE 			0x001f

/*×Ö·ûÏÔÊ¾ÆðÊ¼Æ«ÒÆµØÖ·*/
// #define LCD_X_OFFSET 8
// #define LCD_Y_OFFSET 8
#define LCD_X_OFFSET 8
#define LCD_Y_OFFSET 0

// Be sure make the same in s2440.h
#define DMA_BUFFER_BASE			0x31000000


/* x : 3.5´çºáÆÁ·¶Î§0-319	3.5´çÊúÆÁ·¶Î§0-239	 */
/* y : 3.5´çºáÆÁ·¶Î§0-239	3.5´çÊúÆÁ·¶Î§0-319	 */

/*LCDÆÁ×ø±ê²ÎÊý*/
typedef struct  
{
	int x;
	int y;
}SCREEN_POSITION;

static SCREEN_POSITION Position;

/*×Ö·ûµãÕó²ÎÊý*/

typedef struct  
{
	unsigned char *name;
	unsigned char *bitmap;
}FONT_PARAM;

static FONT_PARAM FontBuffer[]=
{	
	{"",Bmp000},
	{"a",Bmp001},
	{"b",Bmp002},
	{"c",Bmp003},
	{"d",Bmp004},
	{"e",Bmp005},
	{"f",Bmp006},
	{"g",Bmp007},
	{"h",Bmp008},
	{"i",Bmp009},
	{"j",Bmp010},
	{"k",Bmp011},
	{"l",Bmp012},
	{"m",Bmp013},
	{"n",Bmp014},
	{"o",Bmp015},
	{"p",Bmp016},
	{"q",Bmp017},
	{"r",Bmp018},
	{"s",Bmp019},
	{"t",Bmp020},
	{"u",Bmp021},
	{"v",Bmp022},
	{"w",Bmp023},
	{"x",Bmp024},
	{"y",Bmp025},
	{"z",Bmp026},
	{"A",Bmp027},
	{"B",Bmp028},
	{"C",Bmp029},
	{"D",Bmp030},
	{"E",Bmp031},
	{"F",Bmp032},
	{"G",Bmp033},
	{"H",Bmp034},
	{"I",Bmp035},
	{"J",Bmp036},
	{"K",Bmp037},
	{"L",Bmp038},
	{"M",Bmp039},
	{"N",Bmp040},
	{"O",Bmp041},
	{"P",Bmp042},
	{"Q",Bmp043},
	{"R",Bmp044},
	{"S",Bmp045},
	{"T",Bmp046},
	{"U",Bmp047},
	{"V",Bmp048},
	{"W",Bmp049},
	{"X",Bmp050},
	{"Y",Bmp051},
	{"Z",Bmp052},
	{"0",Bmp053},
	{"1",Bmp054},
	{"2",Bmp055},
	{"3",Bmp056},
	{"4",Bmp057},
	{"5",Bmp058},
	{"6",Bmp059},
	{"7",Bmp060},
	{"8",Bmp061},
	{"9",Bmp062},
	{"!",Bmp063},
	{"@",Bmp064},
	{"#",Bmp065},
	{"$",Bmp066},
	{"%",Bmp067},
	{"*",Bmp068},
	{",",Bmp069},
	{".",Bmp070},
	{":",Bmp071},
	{";",Bmp072},
	{"³¤",Bmp073},
};


/*-------------------------------------*/
/*               ASCÂëË÷ÒýÖµ                       */
/*-------------------------------------*/
static int ascLibIndex =(sizeof(FontBuffer)/8);
#if 0
struct VirtualBuffer {
	unsigned short display[LCD_Y_SIZE][LCD_X_SIZE];
};
 struct VirtualBuffer *VBuf;
 #endif
/*FrameBuffer½á¹¹*/
struct FrameBuffer
{
	unsigned short pixel[LCD_Y_SIZE*2][LCD_X_SIZE*2];
};
 
struct FrameBuffer *FBuf;
/*--------------------------------------------------*/
/*		ÆÁÄ»³õÊ¼»¯º¯Êý                              */
/*--------------------------------------------------*/

void FbScreenInit(void)
{
	int x,y;
	// FBuf = (struct FrameBuffer *) (IMAGE_FRAMEBUFFER_UA_BASE);
	//FBuf = (struct FrameBuffer *)(frameBuffer16Bit480272Tft);
	//VBuf = (struct VirtualBuffer *) (IMAGE_FRAMEBUFFER_UA_BASE+0x100000);

	for (y=0; y<LCD_Y_SIZE; y++)
	{
		for (x=0; x<LCD_X_SIZE; x++)
		{
			//FBuf->pixel[y][x]=FONT_GND_BLUE;	
			frameBuffer16Bit480272Tft[y][x]=FONT_GND_BLUE;	

			
			//VBuf->display[y][x] =FONT_GND_BLUE;
		}
	}
	
	Position.x=LCD_X_OFFSET;
	Position.y=LCD_Y_OFFSET;
	printf("\ninit screen successful\n");
}


/*--------------------------------------------------*/
/*  			ÇåÆÁ 								*/
/*--------------------------------------------------*/

void FbScreenClear(void)
{
	int x,y;
	for(y=0;y<LCD_Y_SIZE;y++)
		for(x=0;x<LCD_X_SIZE;x++)		
		{
			//FBuf->pixel[y][x]=FONT_GND_BLUE;
			frameBuffer16Bit480272Tft[y][x]=FONT_GND_BLUE;
		}

	Position.x=LCD_X_OFFSET;
	Position.y=LCD_Y_OFFSET;

		
}

/*--------------------------------------------------*/
/*  			¹öÆÁ									*/
/*--------------------------------------------------*/

void FbScreenScroll(void)
{
	int x,y;
	for(y=0;y<(LCD_Y_SIZE-FONT_Y_SIZE-LCD_Y_OFFSET);y++)
		for(x=0;x<LCD_X_SIZE;x++)
		{
			//FBuf->pixel[LCD_Y_OFFSET+y][x] =FBuf->pixel[LCD_Y_OFFSET+FONT_Y_SIZE+y][x] ;
			frameBuffer16Bit480272Tft[LCD_Y_OFFSET+y][x] =frameBuffer16Bit480272Tft[LCD_Y_OFFSET+FONT_Y_SIZE+y][x] ;
		}

	for(y=(LCD_Y_SIZE-FONT_Y_SIZE-LCD_Y_OFFSET);y<(LCD_Y_SIZE);y++)
		for(x=0;x<LCD_X_SIZE;x++)
		{
			//if(FBuf->pixel[y][x]!=FONT_GND_BLUE)
			//	FBuf->pixel[y][x] = FONT_GND_BLUE;
			if(frameBuffer16Bit480272Tft[y][x]!=FONT_GND_BLUE)
				frameBuffer16Bit480272Tft[y][x] = FONT_GND_BLUE;
		}
}




/*--------------------------------------------------*/
/*  			×Ö·û²éÕÒº¯Êý						*/
/*--------------------------------------------------*/

static int FontAscFind(char  *asc)
{
	int i;
	int bFound = 0;
	//EdbgOutputDebugString("asc = %s\n",asc);
	for(i=0; i<ascLibIndex; i++)
	{
		if(strncmp(asc, FontBuffer[i].name,1)==0)
		{
			//EdbgOutputDebugString("FontBuffer[%d].name = %s\n",i,FontBuffer[i].name);
			return i;	
		}
	}
	return -1;
}


/*--------------------------------------------------*/
/*              LCD×Ö·ûÏÔÊ¾º¯Êý					*/
/* bx : XÖáÏÔÊ¾×ø±ê								*/
/* by : YÖáÏÔÊ¾×ø±ê								*/
/* index : ×Ö·ûË÷Òý								*/
/* color : ×Ö·ûÑÕÉ«								*/
/*--------------------------------------------------*/
static void LCDAscPut(int bx, int by, int index, int color)
{
	int x,y;
	unsigned char temp;
	for(y=0; y<16; y++)
	{	
		temp = FontBuffer[index].bitmap[y];
		for(x=0;x<8;x++)
		{
			if((temp<<x) & 0x80)
			{
				frameBuffer16Bit480272Tft[by+y][bx+x] = color;
				//FBuf->pixel[by+y][bx+x] = color;
				//VBuf->display[LCD_Y_OFFSET+by+i][LCD_X_OFFSET+bx+j] = color;
				//EdbgOutputDebugString("VBuf->display[%d][%d] =%x\n",LCD_Y_OFFSET+by+i,LCD_X_OFFSET+bx+j,VBuf->display[LCD_Y_OFFSET+by+i][LCD_X_OFFSET+bx+j]);

			}
		}
	}
} 

static void LCDAscfontPut(int bx, int by, int index, int color)
{
	int x,y;
	unsigned char temp;
	int count=0;
	
	for(y=0; y<128; y+=4)
	{	
		temp = FontBuffer[index].bitmap[y];
		count =y;
		for(x=0;x<32;x++)
		{
			if(x % 8 == 0)
				temp =FontBuffer[index].bitmap[count++];
			
			if((temp<<(x%8)) & 0x80)
			{

				frameBuffer16Bit480272Tft[by+y][bx+x] = color;

			}
		}
	}
} 


/*------------------------------------------------------*/
/* text : ÏÔÊ¾×Ö·û´®	 */
/* color : ×Ö·û´®ÑÕÉ«	 */
/*------------------------------------------------------*/
static void FbTextOut(char * text, int color)
{
	int font_index; 
	int index;
	unsigned  char *p;
	p = (unsigned char *)text;
	while(*p)
	{
		if(*p !='\n')
		{
			/*  ²éÕÒFONT RAM */
			font_index = FontAscFind(p);
			if( font_index >= 0 )
			{
				LCDAscPut(Position.x, Position.y, font_index, color);	
			
			}
			else
			{
				//EdbgOutputDebugString("ERROR: ASC[%02X] not Show\n", *p);
			}
			Position.x+=FONT_X_SIZE;
			if(Position.x>(LCD_X_SIZE-1))
			{
				//EdbgOutputDebugString("*p=%s ,X=%d,Y=%d\n",p,Position.x,Position.y);
				Position.x = 0;
				Position.y+=FONT_Y_SIZE;
			}
			if(Position.y>(LCD_Y_SIZE-1))
			{	
				Position.x = 0;
				Position.y = 0;
				FbScreenClear();
			}
			p++;

		}
		else
		{
			Position.x = 0;
			Position.y += FONT_Y_SIZE;
			p++;
		}
	}
}


/*------------------------------------------------------*/
/*	¡¡µ¥¸ö×Ö·ûÊä³öº¯Êý						*/
/* ¡¡text : ÏÔÊ¾×Ö·û	 								*/
/*------------------------------------------------------*/

static void FbCharOut(char  text)
{
	int color = FONT_FRONT_YELLOW;
	int font_index; 
	unsigned  char *p;
	p = &text;
	if(*p)
	{
		if(*p == '\n')
		{
	   		//EdbgOutputDebugString("nnn,X=%d,Y=%d\n",Position.x,Position.y);
			Position.x = LCD_X_OFFSET;
			if((Position.y+FONT_Y_SIZE)>(LCD_Y_SIZE-1-FONT_Y_SIZE))
			{			
	   			//EdbgOutputDebugString("FbScreenScroll,X=%d,Y=%d\n",Position.x,Position.y);
				FbScreenScroll();
			}
			else
			{
				Position.y += FONT_Y_SIZE;
			}

		}
		else if(*p=='\r')
		{
			// EdbgOutputDebugString("rrr ,X=%d,Y=%d\n",Position.x,Position.y);
			//Position.x+=FONT_X_SIZE;

		}
		else
		{
			/*  ²éÕÒFONT RAM */
			font_index = FontAscFind(p);
			if( font_index >= 0 )
			{
				//EdbgOutputDebugString("*p=%s ,X=%d,Y=%d\n",p,Position.x,Position.y);

				LCDAscPut(Position.x, Position.y, font_index, color);	
			
			}
			else
			{
				// EdbgOutputDebugString("ERROR: ASC[%s] not Show\n", p);
			}
			Position.x+=FONT_X_SIZE;

			if(Position.x>(LCD_X_SIZE-1))
			{
				//EdbgOutputDebugString("*p=%s ,X=%d,Y=%d\n",p,Position.x,Position.y);
				Position.x = LCD_X_OFFSET;
				if((Position.y+FONT_Y_SIZE)>(LCD_Y_SIZE-1-FONT_Y_SIZE))
				{	
					FbScreenScroll();
				}
				else
				{
				 Position.y+=FONT_Y_SIZE;
				}

			}
		

		}
		
	}
}

#if 1

//
//  For debugging purposes..
//

static void OutputString(const unsigned char *s)
{
    while (*s) 
    {
        if (*s == '\n') 
            FbCharOut('\r');
        
        FbCharOut(*s++);
    }
}


static void pOutputByte(unsigned char c)
{ 
    FbCharOut(c);
}


static void pOutputNumHex(unsigned long n, long depth)
{
    if (depth)    
        depth--;
    

    if ((n & ~0xf) || depth)
    {
        pOutputNumHex(n >> 4, depth);
        n &= 0xf;
    }

    if (n < 10)
    {
        pOutputByte((unsigned char)(n + '0'));
    }
    else
    {
        pOutputByte((unsigned char)(n - 10 + 'A'));
    }
}


static void pOutputNumDecimal(unsigned long n)
{
    if (n >= 10) 
    {
        pOutputNumDecimal(n / 10);
        n %= 10;
    }
    pOutputByte((unsigned char)(n + '0'));
}



////////////////////////////////////////////////////////////////////////////////
//  LCD_Printf()
//  Returns length of formatted string
//  Input:
//      Pointer to string to return formatted output.  
//      User must ensure that buffer is large enough.
//
//  const unsigned char * |   sz,... |
//  Format String:
//
//  @flag Format string | type
//  @flag u | unsigned
//  @flag d | int
//  @flag c | char
//  @flag s | string
//  @flag x | 4-bit hex number
//  @flag B | 8-bit hex number
//  @flag H | 16-bit hex number
//  @flag X | 32-bit hex number
//

void lcd_printf(const   unsigned char *sz, ...)
{


    unsigned char    c;
    va_list  vl;

   va_start(vl, sz);
    
    while (*sz) 
    {
        c = *sz++;
        switch (c) 
        {
            case (unsigned char)'%':
                c = *sz++;
                switch (c) 
                {
                    case 'x':
                        pOutputNumHex(va_arg(vl, unsigned long), 0);
                        break;
                    case 'B':
                        pOutputNumHex(va_arg(vl, unsigned long), 2);
                        break;
                    case 'H':
                        pOutputNumHex(va_arg(vl, unsigned long), 4);
                        break;
                    case 'X':
                        pOutputNumHex(va_arg(vl, unsigned long), 8);
                        break;
                    case 'd': 
                    {
                        long    l;
                        l = va_arg(vl, long);
                        if (l < 0) 
                        {
                            pOutputByte('-');
                            l = - l;
                        }
                        pOutputNumDecimal((unsigned long)l);
                    }
                        break;

                  case 'u':
                        pOutputNumDecimal(va_arg(vl, unsigned long));
                        break;

                    case 's':
                        OutputString(va_arg(vl, char *));
                        break;

                    case '%':
                        pOutputByte('%');
                        break;

                    case 'c':
                        c = va_arg(vl,  unsigned int);
                        pOutputByte(c);
                        break;

                    default:
                        pOutputByte(' ');
                        break;
                }
                break;
            case '\n':
                pOutputByte('\r');
                // fall through
            
            default:
                pOutputByte(c);
        }
    }

    pOutputByte(0);   
    
    va_end(vl);    

}   //  LCD_Printf()


#endif

/*--------------------------------------------------*/
/*              ²âÊÔ³ÌÐò								*/
/*--------------------------------------------------*/

void FbDemo(void)
{
	int i;
	// EdbgOutputDebugString("Clear Screen\n");
	FbScreenInit();
	
	//FbScreenClear();
	// EdbgOutputDebugString("Show Text\n");
#if 1
	//for(i=0;i<31;i++)
	{
	//LCD_Printf("abcdefghikjlmnopqrstuvwxyz0123456789!#@%*,.:\n", FONT_FRONT_YELLOW);
	

	lcd_printf ("0123456789012345678901234567890123456789\n");
	//LCD_Printf("ABCDEFGHIJKLMNOPQRSTUVWXYZ\n", FONT_FRONT_YELLOW);
	lcd_printf ("abcdABCD%d\n",i);
	lcd_printf("TEST Output string %d\n",i);
	lcd_printf("########################\n");

	}
#endif
	//FbScreenScroll();
}

void LcdWindowOnOff(int onoff)
{
    //S3C24X0_LCD *const lcdsn = S3C24X0_GetBase_LCD();
	
	if(onoff)
		__raw_writel(__raw_readl(S3C_WINCON0)|S3C_WINCONx_ENWIN_F_ENABLE, S3C_WINCON0);   // ON
	else
		__raw_writel(__raw_readl(S3C_WINCON0)&(~S3C_WINCONx_ENWIN_F_ENABLE), S3C_WINCON0);   // OFF
}

void LcdEnvidOnOff(int onoff)
{
	if(onoff)
		__raw_writel(__raw_readl(S3C_VIDCON0)|(3), 	S3C_VIDCON0);   // ON
	else 
		__raw_writel(__raw_readl(S3C_VIDCON0)&(~3), S3C_VIDCON0);   // OFF

}    


void Lcd_config(void) {

	mach_info.vidcon0= S3C_VIDCON0_INTERLACE_F_PROGRESSIVE | S3C_VIDCON0_VIDOUT_RGB_IF | S3C_VIDCON0_L1_DATA16_SUB_16_MODE | \
			S3C_VIDCON0_L0_DATA16_MAIN_16_MODE | S3C_VIDCON0_PNRMODE_RGB_P | \
			S3C_VIDCON0_CLKVALUP_ALWAYS | S3C_VIDCON0_CLKDIR_DIVIDED | S3C_VIDCON0_CLKSEL_F_HCLK | \
			S3C_VIDCON0_ENVID_DISABLE | S3C_VIDCON0_ENVID_F_DISABLE,
	mach_info.vidcon1= S3C_VIDCON1_IHSYNC_INVERT | S3C_VIDCON1_IVSYNC_INVERT | S3C_VIDCON1_IVDEN_NORMAL,
	mach_info.dithmode = (S3C_DITHMODE_RDITHPOS_5BIT | S3C_DITHMODE_GDITHPOS_6BIT | S3C_DITHMODE_BDITHPOS_5BIT ) & S3C_DITHMODE_DITHERING_DISABLE,
	
	mach_info.vidtcon0 = S3C_VIDTCON0_VBPD(V_BP - 1) | S3C_VIDTCON0_VFPD(V_FP - 1) | S3C_VIDTCON0_VSPW(V_SW - 1);
	mach_info.vidtcon1 = S3C_VIDTCON1_HBPD(H_BP - 1) | S3C_VIDTCON1_HFPD(H_FP - 1) | S3C_VIDTCON1_HSPW(H_SW - 1);
	mach_info.vidtcon2 = S3C_VIDTCON2_LINEVAL(V_RESOLUTION - 1) | S3C_VIDTCON2_HOZVAL(H_RESOLUTION - 1);

	mach_info.wincon0=   S3C_WINCONx_HAWSWP_ENABLE | S3C_WINCONx_BURSTLEN_16WORD | S3C_WINCONx_BPPMODE_F_16BPP_565;// 4word burst, 16bpp,


	mach_info.vidosd0a = S3C_VIDOSDxA_OSD_LTX_F(0) | S3C_VIDOSDxA_OSD_LTY_F(0);
	mach_info.vidosd0b = S3C_VIDOSDxB_OSD_RBX_F(H_RESOLUTION - 1) | S3C_VIDOSDxB_OSD_RBY_F(V_RESOLUTION - 1);

	mach_info.vidosd1a = S3C_VIDOSDxA_OSD_LTX_F(0) | S3C_VIDOSDxA_OSD_LTY_F(0);
	mach_info.vidosd1b = S3C_VIDOSDxB_OSD_RBX_F(H_RESOLUTION_OSD - 1) | S3C_VIDOSDxB_OSD_RBY_F(V_RESOLUTION_OSD - 1);

	mach_info.vidintcon0 = S3C_VIDINTCON0_FRAMESEL0_VSYNC | S3C_VIDINTCON0_FRAMESEL1_NONE | S3C_VIDINTCON0_INTFRMEN_DISABLE | \
			S3C_VIDINTCON0_FIFOSEL_WIN0 | S3C_VIDINTCON0_FIFOLEVEL_25 | S3C_VIDINTCON0_INTFIFOEN_DISABLE | S3C_VIDINTCON0_INTEN_ENABLE,
	mach_info.vidintcon1 = 0,

	mach_info.width=	H_RESOLUTION;
	mach_info.height=	V_RESOLUTION;
	mach_info.xres=	H_RESOLUTION;
	mach_info.yres=	V_RESOLUTION;

	mach_info.xoffset=	0;
	mach_info.yoffset=	0;


	mach_info.xres_virtual =	H_RESOLUTION;
	mach_info.yres_virtual =	V_RESOLUTION;

	mach_info.osd_width=	H_RESOLUTION_OSD;
	mach_info.osd_height=	V_RESOLUTION_OSD;
	mach_info.osd_xres=	H_RESOLUTION_OSD;
	mach_info.osd_yres=	V_RESOLUTION_OSD;

	mach_info.osd_xres_virtual=	H_RESOLUTION_OSD;
	mach_info.osd_yres_virtual=	V_RESOLUTION_OSD;


	mach_info.bpp=		S3C_FB_PIXEL_BPP_16;
	mach_info.wpalcon =   S3C_WPALCON_W0PAL_16BIT,
      mach_info	.pixclock=	PIXEL_CLOCK;
	mach_info.bytes_per_pixel = 2;

      mach_info	.hsync_len= 	H_SW;
	mach_info.vsync_len=	V_SW;

      mach_info	.left_margin= 	H_FP;
	mach_info.upper_margin=	V_FP;
      mach_info	.right_margin= 	H_BP;
	mach_info.lower_margin=	V_BP;

      mach_info	.sync= 		0;
	mach_info.cmap_static=	1;
};


void lcd480272_init(void)
{   
	int x;
	int y;
	u_long flags = 0;
	u_long PageWidth = 0, Offset = 0;
	U32 addr;

	LcdWindowOnOff(0);
	LcdEnvidOnOff(0);
	
	__raw_writel(__raw_readl(S3C_WINCON1)&(~S3C_WINCONx_ENWIN_F_ENABLE), S3C_WINCON1);   // OFF
      frameBuffer16Bit480272Tft=(U16 (*)[SCR_XSIZE_TFT_480272])LCDFRAMEBUFFER;


	/* Initialise LCD with values from hare */
      printf("Initialise LCD with values\r\n");

		mach_info.vidcon0 = mach_info.vidcon0 & ~(S3C_VIDCON0_ENVID_ENABLE | S3C_VIDCON0_ENVID_F_ENABLE );
		__raw_writel(mach_info.vidcon0, S3C_VIDCON0);
		mach_info.vidcon0 |=  S3C_VIDCON0_CLKVAL_F((int)(((166.5*1000000) / LCD_PIXEL_CLOCK) - 1));
		//mach_info.vidcon0 |=  3 << 6;
        /* For buffer start address */
	__raw_writel((U32)frameBuffer16Bit480272Tft, S3C_VIDW00ADD0B0);
	
	PageWidth = mach_info.xres *  mach_info.bpp / 8;
	Offset = (mach_info.xres_virtual - mach_info.xres) *  mach_info.bpp / 8;
	
	/* End address */

	__raw_writel(LCDBASEL((U32) LCDFRAMEBUFFER + (H_RESOLUTION*V_RESOLUTION*2)), S3C_VIDW00ADD1B0);
	/* size of buffer */
	__raw_writel(OFFSIZE(Offset) | (PAGEWIDTH(PageWidth)), S3C_VIDW00ADD2B0);
	__raw_writel((U32)frameBuffer16Bit480272Tft, S3C_VIDW00ADD0B1);
	__raw_writel(S3C_VIDWxxADD1_VBASEL_F((U32) LCDFRAMEBUFFER + (H_RESOLUTION*V_RESOLUTION*2)), S3C_VIDW00ADD1B1 );

		__raw_writel(mach_info.wincon0, S3C_WINCON0);
		__raw_writel(mach_info.vidcon0, S3C_VIDCON0);
		__raw_writel(mach_info.vidcon1, S3C_VIDCON1);
		__raw_writel(mach_info.vidtcon0, S3C_VIDTCON0);
		__raw_writel(mach_info.vidtcon1, S3C_VIDTCON1);
		__raw_writel(mach_info.vidtcon2, S3C_VIDTCON2);

		__raw_writel(mach_info.dithmode, S3C_DITHMODE);
		__raw_writel(mach_info.vidintcon0, S3C_VIDINTCON0);
		__raw_writel(mach_info.vidintcon1, S3C_VIDINTCON1);
		__raw_writel(mach_info.vidosd0a, S3C_VIDOSD0A);
		__raw_writel(mach_info.vidosd0b, S3C_VIDOSD0B);
		__raw_writel(mach_info.vidosd0c, S3C_VIDOSD0C);
		__raw_writel(mach_info.wpalcon, S3C_WPALCON);


		LcdWindowOnOff(1);
		LcdEnvidOnOff(1);

	for(y=0; y<mach_info.yres; y++)
		for(x=0; x<mach_info.xres; x++)
			frameBuffer16Bit480272Tft[y][x] = 0;
	for(y=10; y<mach_info.yres-10; y++)
		for(x=10; x<mach_info.xres-10; x++)
			frameBuffer16Bit480272Tft[y][x] = 0;
	for(y=20; y<mach_info.yres-20; y++)
		for(x=20; x<mach_info.xres-20; x++)
			frameBuffer16Bit480272Tft[y][x] = 0;

	for(y=0; y<mach_info.yres; y++)
		for(x=0; x<mach_info.xres; x++)
			frameBuffer16Bit480272Tft[y][x] = COLOR_RED_TFT16;
	for(y=10; y<mach_info.yres-10; y++)
		for(x=10; x<mach_info.xres-10; x++)
			frameBuffer16Bit480272Tft[y][x] = COLOR_GREEN_TFT16;
	for(y=20; y<mach_info.yres-20; y++)
		for(x=20; x<mach_info.xres-20; x++)
			frameBuffer16Bit480272Tft[y][x] = COLOR_BLUE_TFT16;
#if 0
Position.x=140;
Position.y=80;
lcd_printf("DMATEK6410  PLATFORM  FOR DMATEK.CO:\n\n");
Position.x=150;
lcd_printf("      U-boot  1.1.6\n");
Position.x=150;
lcd_printf("      Linux  2.6.27\n");
Position.x=150;
lcd_printf("      ANDROID GUI\n");
#endif
//FbTextOut("DMATEK",COLOR_RED_TFT16);

}




void Lcd_Init(void)
{


		lcd480272_init();
	
}

#if 1//def CONFIG_LCD
static void SetLcdPort(void)
{
      int i;
	ulong gpdat;
	ulong gpcon;
	ulong hclkcon; //, sclkcon;

	writel(0x0, S3C_HOSTIFB_MIFPCON);
	/* Enable clock to LCD */
	hclkcon = readl(HCLK_GATE);
	hclkcon |= (1<<3);
	writel(hclkcon, HCLK_GATE);

#if 1
	// To select TFT LCD type
	gpdat = __raw_readl(S3C_SPCON);
	gpdat  = (gpdat & ~0x3) | (1 << 0);
	__raw_writel(gpdat, S3C_SPCON);
#endif
	writel(0xaaaaaaaa, GPICON);  // CTRL,  VD[7:0]
	writel(0xaaaaaaaa, GPJCON);  // VD[23:8]

	/* LCD Backlight Enable control */
	gpcon = __raw_readl(GPFCON);
	gpcon  = (gpcon & ~(3<<30)) | (1<<30);
	__raw_writel(gpcon, GPFCON);

	/*  GPB3 for Backlight PWM */
	gpdat |= __raw_readl(GPFDAT);
	 gpdat  &= ~(1<<15);
	__raw_writel(gpdat, GPFDAT);

	gpdat |= __raw_readl(GPMCON);
	 gpdat  &= ~(0xF << 8) | (1 << 8);
	__raw_writel(gpdat, GPMCON);

	gpcon = __raw_readl(GPMDAT);
	gpcon  |=  (1<<2);
	__raw_writel(gpcon, GPMDAT);


	//lcd power enable,gpg4 set high  ZHB
	gpcon = __raw_readl(GPMCON);
	gpcon  = (gpcon & ~(0xF<<12)) | (1<<12);
	__raw_writel(gpcon, GPMCON);
	udelay (100000);
	
	gpdat = __raw_readl(GPMDAT);
	gpdat  |= (1<<3);
	__raw_writel(gpdat, GPMDAT);
	 udelay (10000);

	gpdat = __raw_readl(GPMDAT);
	gpdat  &= ~(1<<3);   // goes to LOW
	__raw_writel(gpdat, GPMDAT);
	udelay (10000);

	gpdat = __raw_readl(GPMDAT);
	gpdat  |= (1<<3);   // goes to HIGH
	__raw_writel(gpdat, GPMDAT);

}

#endif

void LcdDisplay(void)
{
    Lcd_config();
    SetLcdPort();
    Lcd_Init();	//LCD³õÊ¼»¯
//    FbDemo();
    LcdWindowOnOff(1);
    LcdEnvidOnOff(1);
 }



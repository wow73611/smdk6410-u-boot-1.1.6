#ifndef __LCDLIB_H__
#define __LCDLIB_H__

#include <asm/hardware.h>

#define	COLOR_RED_TFT16		0xf800
#define	COLOR_GREEN_TFT16	0x07e0
#define	COLOR_BLUE_TFT16	0x001f

#define LCDFRAMEBUFFER 0x57800000

/* VIDCON0 */
#define fVIDOUT  Fld(2,22)
#define VIDCON0_RGB_IF  FInsrt(0x0, fVIDOUT)
#define fPNRMODE  Fld(2,13)
#define VIDCON0_RGB_PAR  FInsrt(0x0, fPNRMODE)
#define VIDCON0_VCLK_GATING_OFF  (1<<5)
#define VIDCON0_VCLK_GATING_ON   (0<<5)
#define VIDCON0_CLKDIR_DIV   (1<<4)
#define VIDCON0_CLKDIR_DIR   (0<<4)
#define fCLKSEL   Fld(2,2)
#define VIDCON0_CLKSEL_HCLK	  FInsrt(0x0, fCLKSEL)
#define fCLKVAL     Fld(6,6)
#define CLKVALMSK   FMsk(fCLKVAL)   /* clk value bit clear */
#define CLKVAL(x)   FInsrt((x), fCLKVAL)
#define ENVID       (1 << 1)    /* 0:Disable 1:Enable LCD video output and logic immediatly */
#define ENVID_F     (1 << 0)    /* 0:Dis 1:Ena wait until Current frame end. */
#define ENWIN_F     (1 << 0)    /* 0:Dis 1:Ena wait until Current frame end. */
#define BUFAUTOEN   (1 << 22)   /* Double buffer auto enable bit */
#define LCDBASEL(x) FInsrt((x), Fld(24,0))  /* the end address of the LCD frame buffer. */
#define OFFSIZE(x)  FInsrt((x), Fld(13,13)) /* Virtual screen offset size (the number of byte). */
#define PAGEWIDTH(x)    FInsrt((x), Fld(13, 0)) /* Virtual screen page width (the number of byte). */


#define M5D(n) ((n) & 0x1fffff)	// To get lower 21bits

	
//TFT 240320
#define LCD_XSIZE_TFT_240320 	(240)	
#define LCD_YSIZE_TFT_240320 	(320)
//TFT 320X240
#define LCD_XSIZE_TFT_320240 	(320)
#define LCD_YSIZE_TFT_320240 	(240)
//TFT 480X272
#define LCD_XSIZE_TFT_480272 	(480)
#define LCD_YSIZE_TFT_480272 	(272)

//TFT 640480
#define LCD_XSIZE_TFT_640480 	(640)	
#define LCD_YSIZE_TFT_640480 	(480)
//TFT 800600
#define LCD_XSIZE_TFT_800480 	(800)	
#define LCD_YSIZE_TFT_800480 	(480)

//TFT 240320
#define SCR_XSIZE_TFT_240320 	(LCD_XSIZE_TFT_240320*2)
#define SCR_YSIZE_TFT_240320 	(LCD_YSIZE_TFT_240320*2)
//TFT 320240
#define SCR_XSIZE_TFT_320240 	(LCD_XSIZE_TFT_320240*2)
#define SCR_YSIZE_TFT_320240 	(LCD_YSIZE_TFT_320240*2)
//TFT 480272
#define SCR_XSIZE_TFT_480272 	(LCD_XSIZE_TFT_480272)
#define SCR_YSIZE_TFT_480272 	(LCD_YSIZE_TFT_480272*2)
//TFT 640480
#define SCR_XSIZE_TFT_640480 	(LCD_XSIZE_TFT_640480*2)
#define SCR_YSIZE_TFT_640480 	(LCD_YSIZE_TFT_640480*2)
//TFT 800600
#define SCR_XSIZE_TFT_800480 	(LCD_XSIZE_TFT_800480*2)
#define SCR_YSIZE_TFT_800480 	(LCD_YSIZE_TFT_800480*2)
/********************************************/

#define ON		1
#define OFF 		0

#define DEFAULT_BACKLIGHT_LEVEL		2

#define H_FP	  5		/* front porch */
#define H_SW  10		/* Hsync width */
#define H_BP	  4		/* Back porch */

#define V_FP	  11		/* front porch */
#define V_SW  5		/* Vsync width */
#define V_BP	  4		/* Back porch */

/*
typedef struct {
	int Bpp;
	int LeftTop_x;
	int LeftTop_y;
	int Width;
	int Height;
}  s3c_win_info_t;

s3c_win_info_t window_info;
*/
//------------------ Virtual Screen -----------------------

//------------------ OSD (On Screen Display) -----------------------
#define START_OSD		1
#define STOP_OSD		0

// QCIF OSD image
#define H_RESOLUTION_OSD	480	/* horizon pixel  x resolition */
#define V_RESOLUTION_OSD	272	/* line cnt       y resolution */


#define ALPHA_UP		3
#define ALPHA_DOWN		4
#define MOVE_LEFT		5
#define MOVE_RIGHT		6
#define MOVE_UP			7
#define MOVE_DOWN		8

#define MAX_ALPHA_LEVEL		0x0f

//int osd_alpha_level = MAX_ALPHA_LEVEL;
//int osd_left_top_x = 0;
//int osd_left_top_y = 0;
//int osd_right_bottom_x = H_RESOLUTION_OSD-1;
//int osd_left_bottom_y = V_RESOLUTION_OSD -1;
//------------------------------------------------------------------------

#define H_RESOLUTION	480	/* horizon pixel  x resolition */
#define V_RESOLUTION	272	/* line cnt       y resolution */

#define H_RESOLUTION_VIRTUAL	800	/* horizon pixel  x resolition */
#define V_RESOLUTION_VIRTUAL	600	/* line cnt       y resolution */


#define VFRAME_FREQ     65	/* frame rate freq */


#define PIXEL_CLOCK	VFRAME_FREQ * LCD_PIXEL_CLOCK	/*  vclk = frame * pixel_count */
#define PIXEL_BPP8	8
#define PIXEL_BPP16	16	/*  RGB 5-6-5 format for SMDK EVAL BOARD */
#define PIXEL_BPP24	32	/*  RGB 8-8-8 format for SMDK EVAL BOARD */

#define LCD_PIXEL_CLOCK (VFRAME_FREQ *(H_FP+H_SW+H_BP+H_RESOLUTION) * (V_FP+V_SW+V_BP+V_RESOLUTION))

#define MAX_DISPLAY_BRIGHTNESS		100
#define DEF_DISPLAY_BRIGHTNESS		50

//int display_brightness = DEF_DISPLAY_BRIGHTNESS;
/********************************************/




#define LCD_240X320 1

#define LCD_320X240 2
#define LCD_480x272 3
#define LCD_640480  3

typedef struct  {

	int width;
	int height;

	/* Screen info */
	int xres;
	int yres;

	/* Virtual Screen info */
	int xres_virtual;
	int yres_virtual;
	int xoffset;
	int yoffset;

	/* OSD Screen size */
	int osd_width;
	int osd_height;

	/* OSD Screen info */
	int osd_xres;
	int osd_yres;

	/* OSD Screen info */
	int osd_xres_virtual;
	int osd_yres_virtual;

	int bpp;
	int bytes_per_pixel;
	unsigned long pixclock;
	unsigned long lcd_pixclock;

	int hsync_len;
	int left_margin;
	int right_margin;
	int vsync_len;
	int upper_margin;
	int lower_margin;
	int sync;

	int cmap_grayscale:1;
	int cmap_inverse:1;
	int cmap_static:1;
	int unused:29;

	/* lcd configuration registers */
	unsigned long lcdcon1;
	unsigned long lcdcon2;

        unsigned long lcdcon3;
	unsigned long lcdcon4;
	unsigned long lcdcon5;

	/* GPIOs */
	unsigned long gpcup;
	unsigned long gpcup_mask;
	unsigned long gpccon;
	unsigned long gpccon_mask;
	unsigned long gpdup;
	unsigned long gpdup_mask;
	unsigned long gpdcon;
	unsigned long gpdcon_mask;

	/* lpc3600 control register */
	unsigned long lpcsel;
	unsigned long lcdtcon1;
	unsigned long lcdtcon2;
	unsigned long lcdtcon3;
	unsigned long lcdosd1;
	unsigned long lcdosd2;
	unsigned long lcdosd3;
	unsigned long lcdsaddrb1;
	unsigned long lcdsaddrb2;
	unsigned long lcdsaddrf1;
	unsigned long lcdsaddrf2;
	unsigned long lcdeaddrb1;
	unsigned long lcdeaddrb2;
	unsigned long lcdeaddrf1;
	unsigned long lcdeaddrf2;
	unsigned long lcdvscrb1;
	unsigned long lcdvscrb2;
	unsigned long lcdvscrf1;
	unsigned long lcdvscrf2;
	unsigned long lcdintcon;
	unsigned long lcdkeycon;
	unsigned long lcdkeyval;
	unsigned long lcdbgcon;
	unsigned long lcdfgcon;
	unsigned long lcddithcon;

	unsigned long vidcon0;
	unsigned long vidcon1;
	unsigned long vidtcon0;
	unsigned long vidtcon1;
	unsigned long vidtcon2;
	unsigned long vidtcon3;
	unsigned long wincon0;
	unsigned long wincon2;
	unsigned long wincon1;
	unsigned long wincon3;
	unsigned long wincon4;

	unsigned long vidosd0a;
	unsigned long vidosd0b;
	unsigned long vidosd0c;
	unsigned long vidosd1a;
	unsigned long vidosd1b;
	unsigned long vidosd1c;
	unsigned long vidosd1d;
	unsigned long vidosd2a;
	unsigned long vidosd2b;
	unsigned long vidosd2c;
	unsigned long vidosd2d;
	unsigned long vidosd3a;
	unsigned long vidosd3b;
	unsigned long vidosd3c;
	unsigned long vidosd4a;
	unsigned long vidosd4b;
	unsigned long vidosd4c;

	unsigned long vidw00add0b0;
	unsigned long vidw00add0b1;
	unsigned long vidw01add0;
	unsigned long vidw01add0b0;
	unsigned long vidw01add0b1;

	unsigned long vidw00add1b0;
	unsigned long vidw00add1b1;
	unsigned long vidw01add1;
	unsigned long vidw01add1b0;
	unsigned long vidw01add1b1;

	unsigned long vidw00add2b0;
	unsigned long vidw00add2b1;

	unsigned long vidw02add0;
	unsigned long vidw03add0;
	unsigned long vidw04add0;

	unsigned long vidw02add1;
	unsigned long vidw03add1;
	unsigned long vidw04add1;
	unsigned long vidw00add2;
	unsigned long vidw01add2;
	unsigned long vidw02add2;
	unsigned long vidw03add2;
	unsigned long vidw04add2;

	unsigned long vidintcon;
	unsigned long vidintcon0;
	unsigned long vidintcon1;
	unsigned long w1keycon0;
	unsigned long w1keycon1;
	unsigned long w2keycon0;
	unsigned long w2keycon1;
	unsigned long w3keycon0;
	unsigned long w3keycon1;
	unsigned long w4keycon0;
	unsigned long w4keycon1;

	unsigned long win0map;
	unsigned long win1map;
	unsigned long win2map;
	unsigned long win3map;
	unsigned long win4map;

	unsigned long wpalcon;
	unsigned long dithmode;
	unsigned long intclr0;
	unsigned long intclr1;
	unsigned long intclr2;

	unsigned long win0pal;
	unsigned long win1pal;

       /* backlight info */
	int backlight_min;
	int backlight_max;
	int backlight_default;

	/* utility functions */
	void (*set_backlight_power)(int);
	void (*set_lcd_power)(int);
	void (*set_brightness)(int);

}dmatek_lcd_info;

	
/*type*/
#define U32 unsigned int
#define U16 unsigned short
#define S32 int
#define S16 short int
#define U8  unsigned char
#define	S8  char

#define TRUE 	1   
#define FALSE 	0
#define OK		1
#define FAIL	0

extern void LcdDisplay(void);
extern void Lcd_Init(void);
//U16 (*frameBuffer16Bit480272Tft)[SCR_XSIZE_TFT_480272];
#endif

/**
 * \file lcdas.h
 */


#ifndef __GLCD_H 
#define __GLCD_H


#include "LPC17xx.h"

/**
 * \defgroup LCDPins LCD pins and connections
 * Defines specific pins used to communicate with the LCD module, should not be changed until the board or schematic
 * is changed.
 * @{
 */
/** Group documentation here: \ref LCDPins See datasheet for pin descriptions */
#define PIN_EN		(1 << 19)
/** Group documentation here: \ref LCDPins See datasheet for pin descriptions */
#define PIN_LE		(1 << 20)
/** Group documentation here: \ref LCDPins See datasheet for pin descriptions */
#define PIN_DIR		(1 << 21)
/** Group documentation here: \ref LCDPins See datasheet for pin descriptions */
#define PIN_CS      (1 << 22)
/** Group documentation here: \ref LCDPins See datasheet for pin descriptions */
#define PIN_RS		(1 << 23)
/** Group documentation here: \ref LCDPins See datasheet for pin descriptions */
#define PIN_WR		(1 << 24)
/** Group documentation here: \ref LCDPins See datasheet for pin descriptions */
#define PIN_RD		(1 << 25)   
/** @} */

/**
 * \defgroup LCDPinSetMacros LCD pin setting macros
 * Defines macros used to set specific values to corresponding pins.
 * @{
 */
/** Group documentation here: \ref LCDPinSetMacros See datasheet for pin descriptions */
#define LCD_EN(x)   ((x) ? (LPC_GPIO0->FIOSET = PIN_EN) : (LPC_GPIO0->FIOCLR = PIN_EN));
/** Group documentation here: \ref LCDPinSetMacros See datasheet for pin descriptions */
#define LCD_LE(x)   ((x) ? (LPC_GPIO0->FIOSET = PIN_LE) : (LPC_GPIO0->FIOCLR = PIN_LE));
/** Group documentation here: \ref LCDPinSetMacros See datasheet for pin descriptions */
#define LCD_DIR(x)  ((x) ? (LPC_GPIO0->FIOSET = PIN_DIR) : (LPC_GPIO0->FIOCLR = PIN_DIR));
/** Group documentation here: \ref LCDPinSetMacros See datasheet for pin descriptions */
#define LCD_CS(x)   ((x) ? (LPC_GPIO0->FIOSET = PIN_CS) : (LPC_GPIO0->FIOCLR = PIN_CS));
/** Group documentation here: \ref LCDPinSetMacros See datasheet for pin descriptions */
#define LCD_RS(x)   ((x) ? (LPC_GPIO0->FIOSET = PIN_RS) : (LPC_GPIO0->FIOCLR = PIN_RS));
/** Group documentation here: \ref LCDPinSetMacros See datasheet for pin descriptions */
#define LCD_WR(x)   ((x) ? (LPC_GPIO0->FIOSET = PIN_WR) : (LPC_GPIO0->FIOCLR = PIN_WR));
/** Group documentation here: \ref LCDPinSetMacros See datasheet for pin descriptions */
#define LCD_RD(x)   ((x) ? (LPC_GPIO0->FIOSET = PIN_RD) : (LPC_GPIO0->FIOCLR = PIN_RD));
/** @} */
#define OSCIL_ON 0x0000		//Oscillator
#define POW_CTRL1 0x0003	//Power control 1
#define POW_CTRL2 0x000C	//Power control 2
#define POW_CTRL3 0x000D	//Power control 3
#define POW_CTRL4 0x000E	//Power control 4
#define POW_CTRL5 0x001E	//Power control 5
#define DRV_OUT_CTRL 0x0001	//Driver Output Control
#define DRV_WAVE_CTRL 0x0002	//LCD-Driving-Waveform Control
#define SLEEPM 0x0010 	//Sleep mode
#define ENTRYM 0x0011 	//Entry Mode
#define CMP_R1 0x0005 	//Compare register 1
#define CMP_R2 0x0006 	//Compare register 2
#define HORIZ_P 0x0016 	//Horizontal Porch
#define VERT_P 0x0017 	//Vertical Porch
#define DISP_CTRL 0x0007 	//Display Control
#define FRAME_CYC 0x000B 	//Frame Cycle Control
#define GATE_SPOS 0x000F 	//Gate Scan Position
#define VERT_SCROLL1 0x0041 	//Vertical Scroll Control 1-th Screen
#define VERT_SCROLL2 0x0042 	//Vertical Scroll Control 2-nd Screen
#define DRVPOS_START1 0x0048 	//First Screen driving position Start
#define DRVPOS_END1 0x0049 	//First Screen driving position End
#define DRVPOS_START2 0x004A 	//Second Screen driving position Start
#define DRVPOS_END2 0x004B 	//Second Screen driving position End
#define HADRPOS_RAM 0x0044 	//Horizontal RAM address position
#define VADRPOS_RAM_START 0x0045 	//Vertical RAM address position Start
#define VADRPOS_RAM_END 0x0046 	//Vertical RAM address position End
#define GAMMA_CTRL0 0x0030 	//Gamma Control 0
#define GAMMA_CTRL1 0x0031 	//Gamma Control 1
#define GAMMA_CTRL2 0x0032 	//Gamma Control 2
#define GAMMA_CTRL3 0x0033 	//Gamma Control 3
#define GAMMA_CTRL4 0x0034 	//Gamma Control 4
#define GAMMA_CTRL5 0x0035 	//Gamma Control 5
#define GAMMA_CTRL6 0x0036 	//Gamma Control 6
#define GAMMA_CTRL7 0x0037 	//Gamma Control 7
#define GAMMA_CTRL8 0x003A 	//Gamma Control 8
#define GAMMA_CTRL9 0x003B 	//Gamma Control 9
#define DMASK_WRAM1 0x0023 	//RAM write data mask 1
#define DMASK_WRAM2 0x0024 	//RAM write data mask 2
#define FRAME_FREQ 0x0025 	//Frame Frequency Control
#define ADRY_RAM 0x004f 	//RAM address set Y
#define ADRX_RAM 0x004e 	//RAM address set X
#define DATA_RAM 0x0022		//RAM data 

/**
 * Defines initial rotation of the screen.
 */
#define DISP_ORIENTATION  0  /* angle 0 90 */

#if  ( DISP_ORIENTATION == 90 ) || ( DISP_ORIENTATION == 270 )

/**
 * Screen width
 */
#define  LCD_MAX_X  320
/**
 * Screen height
 */
#define  LCD_MAX_Y  240   

#elif  ( DISP_ORIENTATION == 0 ) || ( DISP_ORIENTATION == 180 )

/**
 * Screen width
 */
#define  LCD_MAX_X  240
/**
 * Screen height
 */
#define  LCD_MAX_Y  320   

#endif


/**
 * \defgroup LCDColors Predefined colors for the LCD
 *
 * Colors in rgb565 format defined as 16-bit unsigned numbers ready to use by the programmer
 * @{
 */
/** See \ref LCDColors */
#define LCDWhite         	0xFFFF
/** See \ref LCDColors */
#define LCDBlack         	0x0000
/** See \ref LCDColors */
#define LCDGrey				0xa534
/** See \ref LCDColors */
#define LCDBlue				0x001F
/** See \ref LCDColors */
#define LCDBlueSea	        0x05BF
/** See \ref LCDColors */
#define LCDPastelBlue       0x051F
/** See \ref LCDColors */
#define LCDViolet       	0xB81F
/** See \ref LCDColors */
#define LCDMagenta	       	0xF81F
/** See \ref LCDColors */
#define LCDRed          	0xF800
/** See \ref LCDColors */
#define LCDGinger		 	0xFAE0
/** See \ref LCDColors */
#define LCDGreen        	0x07E0
/** See \ref LCDColors */
#define LCDCyan         	0x7FFF
/** See \ref LCDColors */
#define LCDYellow       	0xFFE0
/**
 * @}
 */


/* Display define ------------------------------------------------------------*/
#define  ILI9320    0  /* 0x9320 */
#define  ILI9325    1  /* 0x9325 */
#define  ILI9328    2  /* 0x9328 */
#define  ILI9331    3  /* 0x9331 */
#define  SSD1298    4  /* 0x8999 */
#define  SSD1289    5  /* 0x8989 */
#define  ST7781     6  /* 0x7783 */
#define  LGDP4531   7  /* 0x4531 */
#define  SPFD5408B  8  /* 0x5408 */
#define  R61505U    9  /* 0x1505 0x0505 */
#define  HX8346A	10 /* 0x0046 */  
#define  HX8347D    11 /* 0x0047 */
#define  HX8347A    12 /* 0x0047 */	
#define  LGDP4535   13 /* 0x4535 */  
#define  SSD2119    14 /* 3.5 LCD 0x9919 */

void lcdConfiguration(void);
void lcdSend (uint16_t byte);
void wait_delay(int count);
uint16_t lcdRead (void);
void lcdWriteIndex(uint16_t index);
void lcdWriteData(uint16_t index);
uint16_t lcdReadData(void);
void lcdWriteReg(uint16_t LCD_Reg,uint16_t LCD_RegValue);
uint16_t lcdReadReg(uint16_t LCD_Reg);
void lcdSetCursor(uint16_t Xpos, uint16_t Ypos);
void delay_ms(uint16_t ms);

typedef struct ClippingRegion {
	uint16_t x0;
	uint16_t y0;
	uint16_t width;
	uint16_t height;
} ClippingRegion;
#endif

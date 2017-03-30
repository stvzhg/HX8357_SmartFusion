#ifndef HX8357_H
#define HX8357_H

#include <inttypes.h>
#include "drivers/mss_spi/mss_spi.h"
#include "drivers/mss_gpio/mss_gpio.h"
#include "gfxfont.h"

#if defined (__arm__) || defined(ARDUINO_STM32_FEATHER)
#if defined(TEENSYDUINO)
  typedef volatile uint8_t RwReg;
  #define USE_FAST_PINIO
 #elif defined(NRF52)
  typedef volatile uint32_t RwReg;
  #define USE_FAST_PINIO
 #else
  typedef volatile uint32_t RwReg;
  #define USE_FAST_PINIO
 #endif
#elif defined (__AVR__) || defined(TEENSYDUINO)
typedef volatile uint8_t RwReg;
  #define USE_FAST_PINIO
#elif defined (ESP8266) || defined (ESP32)
typedef volatile uint32_t RwReg;
#elif defined (__ARDUINO_ARC__)
typedef volatile uint32_t RwReg;
#endif


#define HX8357D 0xD
#define HX8357B 0xB

#define HX8357_TFTWIDTH  320
#define HX8357_TFTHEIGHT 480

#define HX8357_NOP     0x00
#define HX8357_SWRESET 0x01
#define HX8357_RDDID   0x04
#define HX8357_RDDST   0x09

#define HX8357_RDPOWMODE  0x0A
#define HX8357_RDMADCTL  0x0B
#define HX8357_RDCOLMOD  0x0C
#define HX8357_RDDIM  0x0D
#define HX8357_RDDSDR  0x0F

#define HX8357_SLPIN   0x10
#define HX8357_SLPOUT  0x11
#define HX8357B_PTLON   0x12
#define HX8357B_NORON   0x13

#define HX8357_INVOFF  0x20
#define HX8357_INVON   0x21
#define HX8357_DISPOFF 0x28
#define HX8357_DISPON  0x29

#define HX8357_CASET   0x2A
#define HX8357_PASET   0x2B
#define HX8357_RAMWR   0x2C
#define HX8357_RAMRD   0x2E

#define HX8357B_PTLAR   0x30
#define HX8357_TEON  0x35
#define HX8357_TEARLINE  0x44
#define HX8357_MADCTL  0x36
#define HX8357_COLMOD  0x3A

#define HX8357_SETOSC 0xB0
#define HX8357_SETPWR1 0xB1
#define HX8357B_SETDISPLAY 0xB2
#define HX8357_SETRGB 0xB3
#define HX8357D_SETCOM  0xB6

#define HX8357B_SETDISPMODE  0xB4
#define HX8357D_SETCYC  0xB4
#define HX8357B_SETOTP 0xB7
#define HX8357D_SETC 0xB9

#define HX8357B_SET_PANEL_DRIVING 0xC0
#define HX8357D_SETSTBA 0xC0
#define HX8357B_SETDGC  0xC1
#define HX8357B_SETID  0xC3
#define HX8357B_SETDDB  0xC4
#define HX8357B_SETDISPLAYFRAME 0xC5
#define HX8357B_GAMMASET 0xC8
#define HX8357B_SETCABC  0xC9
#define HX8357_SETPANEL  0xCC


#define HX8357B_SETPOWER 0xD0
#define HX8357B_SETVCOM 0xD1
#define HX8357B_SETPWRNORMAL 0xD2

#define HX8357B_RDID1   0xDA
#define HX8357B_RDID2   0xDB
#define HX8357B_RDID3   0xDC
#define HX8357B_RDID4   0xDD

#define HX8357D_SETGAMMA 0xE0

#define HX8357B_SETGAMMA 0xC8
#define HX8357B_SETPANELRELATED  0xE9
#define FRAME_SIZE 8
#define UNIT_TIME 8000

#define HIGH 1
#define LOW 0
extern mss_gpio_id_t _dc;



// Color definitions
#define	HX8357_BLACK   0x0000
#define	HX8357_BLUE    0x001F
#define	HX8357_RED     0xF800
#define	HX8357_GREEN   0x07E0
#define HX8357_CYAN    0x07FF
#define HX8357_MAGENTA 0xF81F
#define HX8357_YELLOW  0xFFE0
#define HX8357_WHITE   0xFFFF

void     begin(uint8_t);
void     setAddrWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
void     pushColor(uint16_t color);
void     fillScreen(uint16_t color);
void     drawPixel(int16_t x, int16_t y, uint16_t color);
void     drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color);
void     drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color);
void     fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
void     setRotation(uint8_t r);
void     invertDisplay(uint8_t i);
uint16_t color565(uint8_t r, uint8_t g, uint8_t b);

void     writecommand(uint8_t c);
void     writedata(uint8_t d);
void     commandList(uint8_t *addr);
void     delay(uint16_t time);

uint8_t  tabcolor;
uint8_t rotation;
uint16_t _width, _height;

void	 digitalWrite(mss_gpio_id_t port, uint8_t value);

//start from here is GFX
void     startWrite(void);
void     endWrite(void);

<<<<<<< HEAD
void writePixel(int16_t x, int16_t y, uint16_t color);

void     drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);
=======
void     drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color),
>>>>>>> 9c79c2342a68a936ef2bb8d46bbb0e804ebf5b5d
void     drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);

void
        drawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color),
        drawCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername,
                         uint16_t color),
        fillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color),
        fillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername,
                         int16_t delta, uint16_t color),
        drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1,
                     int16_t x2, int16_t y2, uint16_t color),
        fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1,
                     int16_t x2, int16_t y2, uint16_t color),
        drawRoundRect(int16_t x0, int16_t y0, int16_t w, int16_t h,
                      int16_t radius, uint16_t color),
        fillRoundRect(int16_t x0, int16_t y0, int16_t w, int16_t h,
                      int16_t radius, uint16_t color),
        drawBitmap(int16_t x, int16_t y, uint8_t *bitmap,
                   int16_t w, int16_t h, uint16_t color, int32_t bg),
        drawXBitmap(int16_t x, int16_t y, const uint8_t *bitmap,
                    int16_t w, int16_t h, uint16_t color),
        drawChar(int16_t x, int16_t y, unsigned char c, uint16_t color,
                 uint16_t bg, uint8_t size),
        setCursor(int16_t x, int16_t y),
        setTextColor(uint16_t c, int32_t bg),
        setTextSize(uint8_t s),
        setTextWrap(uint8_t w),
        cp437(uint8_t x),
        setFont(const GFXfont *f),
        getTextBounds(char *string, int16_t x, int16_t y,
                      int16_t *x1, int16_t *y1, uint16_t *w, uint16_t *h);

void   write(uint8_t);
<<<<<<< HEAD
void   printStr(char* str, int16_t x, int16_t y);

int16_t height(void);
int16_t width(void);

uint8_t getRotation(void);

// get current cursor position (get rotation safe maximum values, using: width() for x, height() for y)
int16_t getCursorX(void);
int16_t getCursorY(void);
=======

int16_t height(void) const;
int16_t width(void) const;

uint8_t getRotation(void) const;

// get current cursor position (get rotation safe maximum values, using: width() for x, height() for y)
int16_t getCursorX(void) const;
int16_t getCursorY(void) const;
>>>>>>> 9c79c2342a68a936ef2bb8d46bbb0e804ebf5b5d

//may be const?
int16_t
        WIDTH, HEIGHT;   // This is the 'raw' display w/h - never changes
int16_t
        cursor_x, cursor_y;
uint16_t
        textcolor, textbgcolor;
uint8_t
        textsize;
uint8_t
        wrap,   // If set, 'wrap' text at right edge of display
        _cp437; // If set, use correct CP437 charset (default is off)
GFXfont
        *gfxFont;

<<<<<<< HEAD
#endif
=======
>>>>>>> 9c79c2342a68a936ef2bb8d46bbb0e804ebf5b5d

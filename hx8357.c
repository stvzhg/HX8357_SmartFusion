#include "hx8357.h"
#include <inttypes.h>
#include "drivers/mss_spi/mss_spi.h"
#include "drivers/mss_gpio/mss_gpio.h"
#include "gfxfont.h"
#include "glcdfont.c"

#ifndef pgm_read_byte
#define pgm_read_byte(addr) (*(const unsigned char *)(addr))
#endif
#ifndef pgm_read_word
#define pgm_read_word(addr) (*(const unsigned short *)(addr))
#endif
#ifndef pgm_read_dword
#define pgm_read_dword(addr) (*(const unsigned long *)(addr))
#endif

#if !defined(__INT_MAX__) || (__INT_MAX__ > 0xFFFF)
#define pgm_read_pointer(addr) ((void *)pgm_read_dword(addr))
#else
#define pgm_read_pointer(addr) ((void *)pgm_read_word(addr))
#endif

#ifndef min
#define min(a,b) (((a) < (b)) ? (a) : (b))
#endif

#ifndef max
#define max(a,b) (((a) > (b)) ? (a) : (b))
#endif

#ifndef abs
#define abs(a) (((a) > (0)) ? (a) : (0 - a))
#endif

#ifndef _swap_int16_t
#define _swap_int16_t(a, b) { int16_t t = a; a = b; b = t; }
#endif

void	 digitalWrite(mss_gpio_id_t port, uint8_t value){
	MSS_GPIO_set_output(port, value);
}

void writecommand(uint8_t c) {
    MSS_GPIO_set_output(_dc, 0);
    MSS_SPI_init(&g_mss_spi1 );
    MSS_SPI_configure_master_mode
    (
            &g_mss_spi1,
            MSS_SPI_SLAVE_0,
            MSS_SPI_MODE0,
            MSS_SPI_PCLK_DIV_4,
            FRAME_SIZE
    );
    MSS_SPI_set_slave_select(&g_mss_spi1, MSS_SPI_SLAVE_0);
    MSS_SPI_transfer_frame(&g_mss_spi1, c);
    MSS_SPI_clear_slave_select(&g_mss_spi1, MSS_SPI_SLAVE_0);
    MSS_GPIO_set_output(_dc, 1);
}


void writedata(uint8_t c) {
    MSS_GPIO_set_output(_dc, 1);
    MSS_SPI_init(&g_mss_spi1 );
    MSS_SPI_configure_master_mode
            (
                    &g_mss_spi1,
                    MSS_SPI_SLAVE_0,
                    MSS_SPI_MODE0,
                    MSS_SPI_PCLK_DIV_4,
                    FRAME_SIZE
            );
    MSS_SPI_set_slave_select(&g_mss_spi1, MSS_SPI_SLAVE_0);
    MSS_SPI_transfer_frame(&g_mss_spi1, c);
    MSS_SPI_clear_slave_select(&g_mss_spi1, MSS_SPI_SLAVE_0);
    MSS_GPIO_set_output(_dc, 1);
}



void begin(uint8_t type) {
    MSS_GPIO_init();
    MSS_GPIO_config(MSS_GPIO_0, MSS_GPIO_OUTPUT_MODE);
    WIDTH = HX8357_TFTWIDTH;
    HEIGHT = HX8357_TFTHEIGHT;
    _width    = WIDTH;
    _height   = HEIGHT;
    rotation  = 0;
    cursor_y  = cursor_x    = 0;
    textsize  = 1;
    textcolor = textbgcolor = 0xFFFF;
<<<<<<< HEAD
    wrap      = HIGH;
    _cp437    = LOW;
    gfxFont   = (void *)0;
=======
    wrap      = true;
    _cp437    = false;
    gfxFont   = NULL;
>>>>>>> 9c79c2342a68a936ef2bb8d46bbb0e804ebf5b5d

    if (type == HX8357B) {
        //Serial.println("linux HX8357B");
        // seqpower
        writecommand(HX8357B_SETPOWER);
        writedata(0x44);
        writedata(0x41);
        writedata(0x06);
        // seq_vcom
        writecommand(HX8357B_SETVCOM);
        writedata(0x40);
        writedata(0x10);
        // seq_power_normal
        writecommand(HX8357B_SETPWRNORMAL);
        writedata(0x05);
        writedata(0x12);
        // seq_panel_driving
        writecommand(HX8357B_SET_PANEL_DRIVING);
        writedata(0x14);
        writedata(0x3b);
        writedata(0x00);
        writedata(0x02);
        writedata(0x11);
        // seq_display_frame
        writecommand(HX8357B_SETDISPLAYFRAME);
        writedata(0x0c);  // 6.8mhz
        // seq_panel_related
        writecommand(HX8357B_SETPANELRELATED);
        writedata(0x01);  // BGR
        // seq_undefined1
        writecommand(0xEA);
        writedata(0x03);
        writedata(0x00);
        writedata(0x00);
        // undef2
        writecommand(0xEB);
        writedata(0x40);
        writedata(0x54);
        writedata(0x26);
        writedata(0xdb);
        // seq_gamma
        writecommand(HX8357B_SETGAMMA); // 0xC8
        writedata(0x00);
        writedata(0x15);
        writedata(0x00);
        writedata(0x22);
        writedata(0x00);
        writedata(0x08);
        writedata(0x77);
        writedata(0x26);
        writedata(0x66);
        writedata(0x22);
        writedata(0x04);
        writedata(0x00);

        // seq_addr mode
        writecommand(HX8357_MADCTL);
        writedata(0xC0);
        // pixel format
        writecommand(HX8357_COLMOD);
        writedata(0x55);

        // set up whole address box
        // paddr
        writecommand(HX8357_PASET);
        writedata(0x00);
        writedata(0x00);
        writedata(0x01);
        writedata(0xDF);
        // caddr
        writecommand(HX8357_CASET);
        writedata(0x00);
        writedata(0x00);
        writedata(0x01);
        writedata(0x3F);

        // display mode
        writecommand(HX8357B_SETDISPMODE);
        writedata(0x00); // CPU (DBI) and internal oscillation ??
        // exit sleep
        writecommand(HX8357_SLPOUT);

        delay(120);
        // main screen turn on
        writecommand(HX8357_DISPON);
        delay(10);
    } else if (type == HX8357D) {
        writecommand(HX8357_SWRESET);
        delay(10);

        // setextc
        writecommand(HX8357D_SETC);
        writedata(0xFF);
        writedata(0x83);
        writedata(0x57);
        delay(300);

        // setRGB which also enables SDO
        writecommand(HX8357_SETRGB);
        writedata(0x80);  //enable SDO pin!
//    writedata(0x00);  //disable SDO pin!
        writedata(0x0);
        writedata(0x06);
        writedata(0x06);

        writecommand(HX8357D_SETCOM);
        writedata(0x25);  // -1.52V

        writecommand(HX8357_SETOSC);
        writedata(0x68);  // Normal mode 70Hz, Idle mode 55 Hz

        writecommand(HX8357_SETPANEL); //Set Panel
        writedata(0x05);  // BGR, Gate direction swapped

        writecommand(HX8357_SETPWR1);
        writedata(0x00);  // Not deep standby
        writedata(0x15);  //BT
        writedata(0x1C);  //VSPR
        writedata(0x1C);  //VSNR
        writedata(0x83);  //AP
        writedata(0xAA);  //FS

        writecommand(HX8357D_SETSTBA);
        writedata(0x50);  //OPON normal
        writedata(0x50);  //OPON idle
        writedata(0x01);  //STBA
        writedata(0x3C);  //STBA
        writedata(0x1E);  //STBA
        writedata(0x08);  //GEN

        writecommand(HX8357D_SETCYC);
        writedata(0x02);  //NW 0x02
        writedata(0x40);  //RTN
        writedata(0x00);  //DIV
        writedata(0x2A);  //DUM
        writedata(0x2A);  //DUM
        writedata(0x0D);  //GDON
        writedata(0x78);  //GDOFF

        writecommand(HX8357D_SETGAMMA);
        writedata(0x02);
        writedata(0x0A);
        writedata(0x11);
        writedata(0x1d);
        writedata(0x23);
        writedata(0x35);
        writedata(0x41);
        writedata(0x4b);
        writedata(0x4b);
        writedata(0x42);
        writedata(0x3A);
        writedata(0x27);
        writedata(0x1B);
        writedata(0x08);
        writedata(0x09);
        writedata(0x03);
        writedata(0x02);
        writedata(0x0A);
        writedata(0x11);
        writedata(0x1d);
        writedata(0x23);
        writedata(0x35);
        writedata(0x41);
        writedata(0x4b);
        writedata(0x4b);
        writedata(0x42);
        writedata(0x3A);
        writedata(0x27);
        writedata(0x1B);
        writedata(0x08);
        writedata(0x09);
        writedata(0x03);
        writedata(0x00);
        writedata(0x01);

        writecommand(HX8357_COLMOD);
        writedata(0x55);  // 16 bit

        writecommand(HX8357_MADCTL);
        writedata(0xC0);

        writecommand(HX8357_TEON);  // TE off
        writedata(0x00);

        writecommand(HX8357_TEARLINE);  // tear line
        writedata(0x00);
        writedata(0x02);

        writecommand(HX8357_SLPOUT); //Exit Sleep
        delay(150);

        writecommand(HX8357_DISPON);  // display on
        delay(50);
    } else {
        //Serial.println("unknown type");
    }
}


void setAddrWindow(uint16_t x0, uint16_t y0, uint16_t x1,
                                    uint16_t y1) {

    writecommand(HX8357_CASET); // Column addr set
    writedata(x0 >> 8);
    writedata(x0 & 0xFF);     // XSTART
    writedata(x1 >> 8);
    writedata(x1 & 0xFF);     // XEND

    writecommand(HX8357_PASET); // Row addr set
    writedata(y0>>8);
    writedata(y0);     // YSTART
    writedata(y1>>8);
    writedata(y1);     // YEND

    writecommand(HX8357_RAMWR); // write to RAM
}


void pushColor(uint16_t color) {
    digitalWrite(_dc, HIGH);
    MSS_SPI_init(&g_mss_spi1 );
	MSS_SPI_configure_master_mode
	(
			&g_mss_spi1,
			MSS_SPI_SLAVE_0,
			MSS_SPI_MODE0,
			MSS_SPI_PCLK_DIV_4,
			FRAME_SIZE
	);
	MSS_SPI_set_slave_select(&g_mss_spi1, MSS_SPI_SLAVE_0);
	MSS_SPI_transfer_frame(&g_mss_spi1, color >> 8);
	MSS_SPI_transfer_frame(&g_mss_spi1, color);
	MSS_SPI_clear_slave_select(&g_mss_spi1, MSS_SPI_SLAVE_0);
    /*
    if (_sclk == -1)
        SPI.beginTransaction(spi_settings);

    spiwrite(color >> 8);
    spiwrite(color);

    if (_sclk == -1)
        SPI.endTransaction();
	*/
}

void drawPixel(int16_t x, int16_t y, uint16_t color) {

    if((x < 0) ||(x >= _width) || (y < 0) || (y >= _height)) return;


    setAddrWindow(x,y,x,y);

    digitalWrite(_dc, HIGH);

    /* 18 bit hack for testing */
    /*
    uint8_t r = (color >> 10) & 0x1F, g = (color >> 5) & 0x3F, b = color & 0x1F;
    r <<= 3;
    g <<= 2;
    b <<= 3;

    spiwrite(r);
    spiwrite(g);
    spiwrite(b);
    */
    /*
    spiwrite(color >> 8);
    spiwrite(color);
	*/
    MSS_SPI_init(&g_mss_spi1 );
	MSS_SPI_configure_master_mode
	(
			&g_mss_spi1,
			MSS_SPI_SLAVE_0,
			MSS_SPI_MODE0,
			MSS_SPI_PCLK_DIV_4,
			FRAME_SIZE
	);
	MSS_SPI_set_slave_select(&g_mss_spi1, MSS_SPI_SLAVE_0);
	MSS_SPI_transfer_frame(&g_mss_spi1, color >> 8);
	MSS_SPI_transfer_frame(&g_mss_spi1, color);
	MSS_SPI_clear_slave_select(&g_mss_spi1, MSS_SPI_SLAVE_0);
}


void drawFastVLine(int16_t x, int16_t y, int16_t h,
                                    uint16_t color) {

    // Rudimentary clipping
    if((x >= _width) || (y >= _height)) return;

    if((y+h-1) >= _height)
        h = _height-y;

    setAddrWindow(x, y, x, y+h-1);

    uint8_t hi = color >> 8, lo = color;


    digitalWrite(_dc, HIGH);


    /*
    if (_sclk == -1)
        SPI.beginTransaction(spi_settings);

    while (h--) {
        spiwrite(hi);
        spiwrite(lo);
    }

    if (_sclk == -1)
        SPI.endTransaction();
	*/
    MSS_SPI_init(&g_mss_spi1 );
	MSS_SPI_configure_master_mode
	(
			&g_mss_spi1,
			MSS_SPI_SLAVE_0,
			MSS_SPI_MODE0,
			MSS_SPI_PCLK_DIV_4,
			FRAME_SIZE
	);
	MSS_SPI_set_slave_select(&g_mss_spi1, MSS_SPI_SLAVE_0);
    while(h--){
		MSS_SPI_transfer_frame(&g_mss_spi1, hi);
		MSS_SPI_transfer_frame(&g_mss_spi1, lo);
    }
    MSS_SPI_clear_slave_select(&g_mss_spi1, MSS_SPI_SLAVE_0);
}


void drawFastHLine(int16_t x, int16_t y, int16_t w,
                                    uint16_t color) {

    // Rudimentary clipping
    if((x >= _width) || (y >= _height)) return;
    if((x+w-1) >= _width)  w = _width-x;
    setAddrWindow(x, y, x+w-1, y);

    uint8_t hi = color >> 8, lo = color;


    digitalWrite(_dc, HIGH);

    /*
    if (_sclk == -1)
        SPI.beginTransaction(spi_settings);

    while (w--) {
        spiwrite(hi);
        spiwrite(lo);
    }

    if (_sclk == -1)
        SPI.endTransaction();
	*/
    MSS_SPI_init(&g_mss_spi1 );
	MSS_SPI_configure_master_mode
	(
			&g_mss_spi1,
			MSS_SPI_SLAVE_0,
			MSS_SPI_MODE0,
			MSS_SPI_PCLK_DIV_4,
			FRAME_SIZE
	);
	MSS_SPI_set_slave_select(&g_mss_spi1, MSS_SPI_SLAVE_0);
    while(w--){
    	MSS_SPI_transfer_frame(&g_mss_spi1, hi);
		MSS_SPI_transfer_frame(&g_mss_spi1, lo);
    }
    MSS_SPI_clear_slave_select(&g_mss_spi1, MSS_SPI_SLAVE_0);

}

void fillScreen(uint16_t color) {
#if defined(ESP8266)
    ESP.wdtDisable();
#endif
    fillRect(0, 0, _width, _height, color);
#if defined(ESP8266)
    ESP.wdtEnable(WDTO_4S);
#endif
}

// fill a rectangle
void fillRect(int16_t x, int16_t y, int16_t w, int16_t h,
                               uint16_t color) {

    // rudimentary clipping (drawChar w/big text requires this)
    if((x >= _width) || (y >= _height)) return;
    if((x + w - 1) >= _width)  w = _width  - x;
    if((y + h - 1) >= _height) h = _height - y;

    setAddrWindow(x, y, x+w-1, y+h-1);

    uint8_t hi = color >> 8, lo = color;

    /* 18 bit hack for testing */
    /*
    uint8_t r = (color >> 10) & 0x1F, g = (color >> 5) & 0x3F, b = color & 0x1F;
    r <<= 3;
    g <<= 2;
    b <<= 3;
    */

    digitalWrite(_dc, HIGH);
    /*
    if (_sclk == -1)
        SPI.beginTransaction(spi_settings);

    for(y=h; y>0; y--) {
        for(x=w; x>0; x--) {
            spiwrite(hi);
            spiwrite(lo);
        }
    }

    if (_sclk == -1)
        SPI.endTransaction();
    */
    MSS_SPI_init(&g_mss_spi1 );
	MSS_SPI_configure_master_mode
	(
			&g_mss_spi1,
			MSS_SPI_SLAVE_0,
			MSS_SPI_MODE0,
			MSS_SPI_PCLK_DIV_4,
			FRAME_SIZE
	);
	MSS_SPI_set_slave_select(&g_mss_spi1, MSS_SPI_SLAVE_0);
	for(y=h; y>0; y--){
		for(x=w; x>0; x--){
			MSS_SPI_transfer_frame(&g_mss_spi1, hi);
			MSS_SPI_transfer_frame(&g_mss_spi1, lo);
		}
	}
	MSS_SPI_clear_slave_select(&g_mss_spi1, MSS_SPI_SLAVE_0);

}


// Pass 8-bit (each) R,G,B, get back 16-bit packed color
uint16_t color565(uint8_t r, uint8_t g, uint8_t b) {
    return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}


#define MADCTL_MY  0x80
#define MADCTL_MX  0x40
#define MADCTL_MV  0x20
#define MADCTL_ML  0x10
#define MADCTL_RGB 0x00
#define MADCTL_BGR 0x08
#define MADCTL_MH  0x04

void setRotation(uint8_t m) {

    writecommand(HX8357_MADCTL);
    rotation = m % 4; // can't be higher than 3
    switch (rotation) {
        case 0:
            writedata(MADCTL_MX | MADCTL_MY | MADCTL_RGB);
            _width  = HX8357_TFTWIDTH;
            _height = HX8357_TFTHEIGHT;
            break;
        case 1:
            writedata(MADCTL_MV | MADCTL_MY | MADCTL_RGB);
            _width  = HX8357_TFTHEIGHT;
            _height = HX8357_TFTWIDTH;
            break;
        case 2:
            writedata( MADCTL_RGB);
            _width  = HX8357_TFTWIDTH;
            _height = HX8357_TFTHEIGHT;
            break;
        case 3:
            writedata(MADCTL_MX | MADCTL_MV | MADCTL_RGB);
            _width  = HX8357_TFTHEIGHT;
            _height = HX8357_TFTWIDTH;
            break;
    }
}


void invertDisplay(uint8_t i) {
    writecommand(i ? HX8357_INVON : HX8357_INVOFF);
}

void delay(uint16_t time) {
    uint32_t i = 0;
    uint32_t target = time * UNIT_TIME;
    for (i = 0; i < target; ++i) {

    }
    return;
}

//start from here is GFX
void writeLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1,
                             uint16_t color) {
    int16_t steep = abs(y1 - y0) > abs(x1 - x0);
    if (steep) {
        _swap_int16_t(x0, y0);
        _swap_int16_t(x1, y1);
    }

    if (x0 > x1) {
        _swap_int16_t(x0, x1);
        _swap_int16_t(y0, y1);
    }

    int16_t dx, dy;
    dx = x1 - x0;
    dy = abs(y1 - y0);

    int16_t err = dx / 2;
    int16_t ystep;

    if (y0 < y1) {
        ystep = 1;
    } else {
        ystep = -1;
    }

    for (; x0<=x1; x0++) {
        if (steep) {
            writePixel(y0, x0, color);
        } else {
            writePixel(x0, y0, color);
        }
        err -= dy;
        if (err < 0) {
            y0 += ystep;
            err += dx;
        }
    }
}

void startWrite(){
    // Overwrite in subclasses if desired!
}

void writePixel(int16_t x, int16_t y, uint16_t color){
    // Overwrite in subclasses if startWrite is defined!
    drawPixel(x, y, color);
}

void writeFastVLine(int16_t x, int16_t y,
                                  int16_t h, uint16_t color) {
    // Overwrite in subclasses if startWrite is defined!
    // Can be just writeLine(x, y, x, y+h-1, color);
    // or writeFillRect(x, y, 1, h, color);
    drawFastVLine(x, y, h, color);
}

void writeFastHLine(int16_t x, int16_t y,
                                  int16_t w, uint16_t color) {
    // Overwrite in subclasses if startWrite is defined!
    // Example: writeLine(x, y, x+w-1, y, color);
    // or writeFillRect(x, y, w, 1, color);
    drawFastHLine(x, y, w, color);
}

void writeFillRect(int16_t x, int16_t y, int16_t w, int16_t h,
                                 uint16_t color) {
    // Overwrite in subclasses if desired!
    fillRect(x,y,w,h,color);
}

void endWrite(){
    // Overwrite in subclasses if startWrite is defined!
}


#define distDiff(a,b) ((max(a,b) - min(a,b))+1)

void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1,
                            uint16_t color) {
    // Update in subclasses if desired!
    if(x0 == x1){
        drawFastVLine(x0, y0, distDiff(y0,y1), color);
    } else if(y0 == y1){
        drawFastHLine(x0, y0, distDiff(x0,x1), color);
    } else {
        startWrite();
        writeLine(x0, y0, x1, y1, color);
        endWrite();
    }
}


// Draw a circle outline
void drawCircle(int16_t x0, int16_t y0, int16_t r,
                              uint16_t color) {
    int16_t f = 1 - r;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * r;
    int16_t x = 0;
    int16_t y = r;

    startWrite();
    writePixel(x0  , y0+r, color);
    writePixel(x0  , y0-r, color);
    writePixel(x0+r, y0  , color);
    writePixel(x0-r, y0  , color);

    while (x<y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;

        writePixel(x0 + x, y0 + y, color);
        writePixel(x0 - x, y0 + y, color);
        writePixel(x0 + x, y0 - y, color);
        writePixel(x0 - x, y0 - y, color);
        writePixel(x0 + y, y0 + x, color);
        writePixel(x0 - y, y0 + x, color);
        writePixel(x0 + y, y0 - x, color);
        writePixel(x0 - y, y0 - x, color);
    }
    endWrite();
}

void drawCircleHelper( int16_t x0, int16_t y0,
                                     int16_t r, uint8_t cornername, uint16_t color) {
    int16_t f     = 1 - r;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * r;
    int16_t x     = 0;
    int16_t y     = r;

    while (x<y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f     += ddF_y;
        }
        x++;
        ddF_x += 2;
        f     += ddF_x;
        if (cornername & 0x4) {
            writePixel(x0 + x, y0 + y, color);
            writePixel(x0 + y, y0 + x, color);
        }
        if (cornername & 0x2) {
            writePixel(x0 + x, y0 - y, color);
            writePixel(x0 + y, y0 - x, color);
        }
        if (cornername & 0x8) {
            writePixel(x0 - y, y0 + x, color);
            writePixel(x0 - x, y0 + y, color);
        }
        if (cornername & 0x1) {
            writePixel(x0 - y, y0 - x, color);
            writePixel(x0 - x, y0 - y, color);
        }
    }
}

void fillCircle(int16_t x0, int16_t y0, int16_t r,
                              uint16_t color) {
    startWrite();
    writeFastVLine(x0, y0-r, 2*r+1, color);
    fillCircleHelper(x0, y0, r, 3, 0, color);
    endWrite();
}

// Used to do circles and roundrects
void fillCircleHelper(int16_t x0, int16_t y0, int16_t r,
                                    uint8_t cornername, int16_t delta, uint16_t color) {

    int16_t f     = 1 - r;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * r;
    int16_t x     = 0;
    int16_t y     = r;

    while (x<y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f     += ddF_y;
        }
        x++;
        ddF_x += 2;
        f     += ddF_x;

        if (cornername & 0x1) {
            writeFastVLine(x0+x, y0-y, 2*y+1+delta, color);
            writeFastVLine(x0+y, y0-x, 2*x+1+delta, color);
        }
        if (cornername & 0x2) {
            writeFastVLine(x0-x, y0-y, 2*y+1+delta, color);
            writeFastVLine(x0-y, y0-x, 2*x+1+delta, color);
        }
    }
}

// Draw a rectangle
void drawRect(int16_t x, int16_t y, int16_t w, int16_t h,
                            uint16_t color) {
    startWrite();
    writeFastHLine(x, y, w, color);
    writeFastHLine(x, y+h-1, w, color);
    writeFastVLine(x, y, h, color);
    writeFastVLine(x+w-1, y, h, color);
    endWrite();
}

// Draw a rounded rectangle
void drawRoundRect(int16_t x, int16_t y, int16_t w,
                                 int16_t h, int16_t r, uint16_t color) {
    // smarter version
    startWrite();
    writeFastHLine(x+r  , y    , w-2*r, color); // Top
    writeFastHLine(x+r  , y+h-1, w-2*r, color); // Bottom
    writeFastVLine(x    , y+r  , h-2*r, color); // Left
    writeFastVLine(x+w-1, y+r  , h-2*r, color); // Right
    // draw four corners
    drawCircleHelper(x+r    , y+r    , r, 1, color);
    drawCircleHelper(x+w-r-1, y+r    , r, 2, color);
    drawCircleHelper(x+w-r-1, y+h-r-1, r, 4, color);
    drawCircleHelper(x+r    , y+h-r-1, r, 8, color);
    endWrite();
}

// Fill a rounded rectangle
void fillRoundRect(int16_t x, int16_t y, int16_t w,
                                 int16_t h, int16_t r, uint16_t color) {
    // smarter version
    startWrite();
    writeFillRect(x+r, y, w-2*r, h, color);

    // draw four corners
    fillCircleHelper(x+w-r-1, y+r, r, 1, h-2*r-1, color);
    fillCircleHelper(x+r    , y+r, r, 2, h-2*r-1, color);
    endWrite();
}

// Draw a triangle
void drawTriangle(int16_t x0, int16_t y0,
                                int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color) {
    drawLine(x0, y0, x1, y1, color);
    drawLine(x1, y1, x2, y2, color);
    drawLine(x2, y2, x0, y0, color);
}

// Fill a triangle
void fillTriangle(int16_t x0, int16_t y0,
                                int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color) {

    int16_t a, b, y, last;

    // Sort coordinates by Y order (y2 >= y1 >= y0)
    if (y0 > y1) {
        _swap_int16_t(y0, y1); _swap_int16_t(x0, x1);
    }
    if (y1 > y2) {
        _swap_int16_t(y2, y1); _swap_int16_t(x2, x1);
    }
    if (y0 > y1) {
        _swap_int16_t(y0, y1); _swap_int16_t(x0, x1);
    }

    startWrite();
    if(y0 == y2) { // Handle awkward all-on-same-line case as its own thing
        a = b = x0;
        if(x1 < a)      a = x1;
        else if(x1 > b) b = x1;
        if(x2 < a)      a = x2;
        else if(x2 > b) b = x2;
        writeFastHLine(a, y0, b-a+1, color);
        endWrite();
        return;
    }

    int16_t
            dx01 = x1 - x0,
            dy01 = y1 - y0,
            dx02 = x2 - x0,
            dy02 = y2 - y0,
            dx12 = x2 - x1,
            dy12 = y2 - y1;
    int32_t
            sa   = 0,
            sb   = 0;

    // For upper part of triangle, find scanline crossings for segments
    // 0-1 and 0-2.  If y1=y2 (flat-bottomed triangle), the scanline y1
    // is included here (and second loop will be skipped, avoiding a /0
    // error there), otherwise scanline y1 is skipped here and handled
    // in the second loop...which also avoids a /0 error here if y0=y1
    // (flat-topped triangle).
    if(y1 == y2) last = y1;   // Include y1 scanline
    else         last = y1-1; // Skip it

    for(y=y0; y<=last; y++) {
        a   = x0 + sa / dy01;
        b   = x0 + sb / dy02;
        sa += dx01;
        sb += dx02;
        /* longhand:
    a = x0 + (x1 - x0) * (y - y0) / (y1 - y0);
    b = x0 + (x2 - x0) * (y - y0) / (y2 - y0);
         */
        if(a > b) _swap_int16_t(a,b);
        writeFastHLine(a, y, b-a+1, color);
    }

    // For lower part of triangle, find scanline crossings for segments
    // 0-2 and 1-2.  This loop is skipped if y1=y2.
    sa = dx12 * (y - y1);
    sb = dx02 * (y - y0);
    for(; y<=y2; y++) {
        a   = x1 + sa / dy12;
        b   = x0 + sb / dy02;
        sa += dx12;
        sb += dx02;
        /* longhand:
    a = x1 + (x2 - x1) * (y - y1) / (y2 - y1);
    b = x0 + (x2 - x0) * (y - y0) / (y2 - y0);
         */
        if(a > b) _swap_int16_t(a,b);
        writeFastHLine(a, y, b-a+1, color);
    }
    endWrite();
}

// Draw a 1-bit image (bitmap) at the specified (x,y) position from the
// provided bitmap buffer (must be PROGMEM memory) using the specified
// foreground color (unset bits are transparent).

// drawBitmap() variant w/background for RAM-resident (not PROGMEM) bitmaps.
void drawBitmap(int16_t x, int16_t y,
                              uint8_t *bitmap, int16_t w, int16_t h, uint16_t color, int32_t bg) {

    int16_t i, j, byteWidth = (w + 7) / 8;
    uint8_t byte = 0;

    startWrite();
    for(j=0; j<h; j++) {
        for(i=0; i<w; i++ ) {
            if(i & 7) byte <<= 1;
            else      byte   = bitmap[j * byteWidth + i / 8];
            if(byte & 0x80) writePixel(x+i, y+j, color);
            else if(bg >= 0)            writePixel(x+i, y+j, bg);
        }
    }
    endWrite();
}

//Draw XBitMap Files (*.xbm), exported from GIMP,
//Usage: Export from GIMP to *.xbm, rename *.xbm to *.c and open in editor.
//C Array can be directly used with this function
void drawXBitmap(int16_t x, int16_t y,
                               const uint8_t *bitmap, int16_t w, int16_t h, uint16_t color) {

    int16_t i, j, byteWidth = (w + 7) / 8;
    uint8_t byte = 0;

    startWrite();
    for(j=0; j<h; j++) {
        for(i=0; i<w; i++ ) {
            if(i & 7) byte >>= 1;
            else      byte   = pgm_read_byte(bitmap + j * byteWidth + i / 8);
            if(byte & 0x01) writePixel(x+i, y+j, color);
        }
    }
    endWrite();
}

// Draw a character
void drawChar(int16_t x, int16_t y, unsigned char c,
                            uint16_t color, uint16_t bg, uint8_t size) {

    if(!gfxFont) { // 'Classic' built-in font

        if((x >= _width)            || // Clip right
           (y >= _height)           || // Clip bottom
           ((x + 6 * size - 1) < 0) || // Clip left
           ((y + 8 * size - 1) < 0))   // Clip top
            return;

        if(!_cp437 && (c >= 176)) c++; // Handle 'classic' charset behavior

        startWrite();
<<<<<<< HEAD
        int8_t i = 0;
        int8_t j = 0;
        for(i=0; i<6; i++ ) {
            uint8_t line;
            if(i < 5) line = pgm_read_byte(font+(c*5)+i);
            else      line = 0x0;
            for(j=0; j<8; j++, line >>= 1) {
=======
        for(int8_t i=0; i<6; i++ ) {
            uint8_t line;
            if(i < 5) line = pgm_read_byte(font+(c*5)+i);
            else      line = 0x0;
            for(int8_t j=0; j<8; j++, line >>= 1) {
>>>>>>> 9c79c2342a68a936ef2bb8d46bbb0e804ebf5b5d
                if(line & 0x1) {
                    if(size == 1) writePixel(x+i, y+j, color);
                    else          writeFillRect(x+(i*size), y+(j*size), size, size, color);
                } else if(bg != color) {
                    if(size == 1) writePixel(x+i, y+j, bg);
                    else          writeFillRect(x+i*size, y+j*size, size, size, bg);
                }
            }
        }
        endWrite();

    } else { // Custom font

        // Character is assumed previously filtered by write() to eliminate
        // newlines, returns, non-printable characters, etc.  Calling drawChar()
        // directly with 'bad' characters of font may cause mayhem!

        c -= pgm_read_byte(&gfxFont->first);
        GFXglyph *glyph  = &(((GFXglyph *)pgm_read_pointer(&gfxFont->glyph))[c]);
        uint8_t  *bitmap = (uint8_t *)pgm_read_pointer(&gfxFont->bitmap);

        uint16_t bo = pgm_read_word(&glyph->bitmapOffset);
        uint8_t  w  = pgm_read_byte(&glyph->width),
                h  = pgm_read_byte(&glyph->height);
        int8_t   xo = pgm_read_byte(&glyph->xOffset),
                yo = pgm_read_byte(&glyph->yOffset);
        uint8_t  xx, yy, bits = 0, bit = 0;
        int16_t  xo16 = 0, yo16 = 0;

        if(size > 1) {
            xo16 = xo;
            yo16 = yo;
        }

        // Todo: Add character clipping here

        // NOTE: THERE IS NO 'BACKGROUND' COLOR OPTION ON CUSTOM FONTS.
        // THIS IS ON PURPOSE AND BY DESIGN.  The background color feature
        // has typically been used with the 'classic' font to overwrite old
        // screen contents with new data.  This ONLY works because the
        // characters are a uniform size; it's not a sensible thing to do with
        // proportionally-spaced fonts with glyphs of varying sizes (and that
        // may overlap).  To replace previously-drawn text when using a custom
        // font, use the getTextBounds() function to determine the smallest
        // rectangle encompassing a string, erase the area with fillRect(),
        // then draw new text.  This WILL infortunately 'blink' the text, but
        // is unavoidable.  Drawing 'background' pixels will NOT fix this,
        // only creates a new set of problems.  Have an idea to work around
        // this (a canvas object type for MCUs that can afford the RAM and
        // displays supporting setAddrWindow() and pushColors()), but haven't
        // implemented this yet.

        startWrite();
        for(yy=0; yy<h; yy++) {
            for(xx=0; xx<w; xx++) {
                if(!(bit++ & 7)) {
                    bits = pgm_read_byte(&bitmap[bo++]);
                }
                if(bits & 0x80) {
                    if(size == 1) {
                        writePixel(x+xo+xx, y+yo+yy, color);
                    } else {
                        writeFillRect(x+(xo16+xx)*size, y+(yo16+yy)*size, size, size, color);
                    }
                }
                bits <<= 1;
            }
        }
        endWrite();

    } // End classic vs custom font
}


void write(uint8_t c) {
    if(!gfxFont) { // 'Classic' built-in font

        if(c == '\n') {
            cursor_y += textsize*8;
            cursor_x  = 0;
        } else if(c == '\r') {
            // skip em
        } else {
            if(wrap && ((cursor_x + textsize * 6) >= _width)) { // Heading off edge?
                cursor_x  = 0;            // Reset x to zero
                cursor_y += textsize * 8; // Advance y one line
            }
            drawChar(cursor_x, cursor_y, c, textcolor, textbgcolor, textsize);
            cursor_x += textsize * 6;
        }

    } else { // Custom font

        if(c == '\n') {
            cursor_x  = 0;
            cursor_y += (int16_t)textsize *
                        (uint8_t)pgm_read_byte(&gfxFont->yAdvance);
        } else if(c != '\r') {
            uint8_t first = pgm_read_byte(&gfxFont->first);
            if((c >= first) && (c <= (uint8_t)pgm_read_byte(&gfxFont->last))) {
                uint8_t   c2    = c - pgm_read_byte(&gfxFont->first);
                GFXglyph *glyph = &(((GFXglyph *)pgm_read_pointer(&gfxFont->glyph))[c2]);
                uint8_t   w     = pgm_read_byte(&glyph->width),
                        h     = pgm_read_byte(&glyph->height);
                if((w > 0) && (h > 0)) { // Is there an associated bitmap?
                    int16_t xo = (int8_t)pgm_read_byte(&glyph->xOffset); // sic
                    if(wrap && ((cursor_x + textsize * (xo + w)) >= _width)) {
                        // Drawing character would go off right edge; wrap to new line
                        cursor_x  = 0;
                        cursor_y += (int16_t)textsize *
                                    (uint8_t)pgm_read_byte(&gfxFont->yAdvance);
                    }
                    drawChar(cursor_x, cursor_y, c, textcolor, textbgcolor, textsize);
                }
                cursor_x += pgm_read_byte(&glyph->xAdvance) * (int16_t)textsize;
            }
        }

    }
}

void setCursor(int16_t x, int16_t y) {
    cursor_x = x;
    cursor_y = y;
}

<<<<<<< HEAD
int16_t getCursorX(void) {
    return cursor_x;
}

int16_t getCursorY(void) {
=======
int16_t getCursorX(void) const {
    return cursor_x;
}

int16_t getCursorY(void) const {
>>>>>>> 9c79c2342a68a936ef2bb8d46bbb0e804ebf5b5d
    return cursor_y;
}

void setTextSize(uint8_t s) {
    textsize = (s > 0) ? s : 1;
}

void setTextColor(uint16_t c, int32_t b) {
    textcolor   = c;
    if(b >= 0)    textbgcolor = b;
    else          textbgcolor = c;
}

void setTextWrap(uint8_t w) {
    wrap = w;
}

<<<<<<< HEAD
uint8_t getRotation(void) {
    return rotation;
}

void printStr(char* str, int16_t x, int16_t y){
	int i = 0;
	setCursor(x, y);
	while(str[i] != '\0'){
		write(str[i]);
		i++;
	}
}

=======
uint8_t getRotation(void) const {
    return rotation;
}

>>>>>>> 9c79c2342a68a936ef2bb8d46bbb0e804ebf5b5d
// Enable (or disable) Code Page 437-compatible charset.
// There was an error in glcdfont.c for the longest time -- one character
// (#176, the 'light shade' block) was missing -- this threw off the index
// of every character that followed it.  But a TON of code has been written
// with the erroneous character indices.  By default, the library uses the
// original 'wrong' behavior and old sketches will still work.  Pass 'true'
// to this function to use correct CP437 character values in your code.
void cp437(uint8_t x) {
    _cp437 = x;
}

void setFont(const GFXfont *f) {
    if(f) {          // Font struct pointer passed in?
        if(!gfxFont) { // And no current font struct?
            // Switching from classic to new font behavior.
            // Move cursor pos down 6 pixels so it's on baseline.
            cursor_y += 6;
        }
    } else if(gfxFont) { // NULL passed.  Current font struct defined?
        // Switching from new to classic font behavior.
        // Move cursor pos up 6 pixels so it's at top-left of char.
        cursor_y -= 6;
    }
    gfxFont = (GFXfont *)f;
}

// Pass string and a cursor position, returns UL corner and W,H.
void getTextBounds(char *str, int16_t x, int16_t y,
                                 int16_t *x1, int16_t *y1, uint16_t *w, uint16_t *h) {
    uint8_t c; // Current character

    *x1 = x;
    *y1 = y;
    *w  = *h = 0;

    if(gfxFont) {

        GFXglyph *glyph;
        uint8_t   first = pgm_read_byte(&gfxFont->first),
                last  = pgm_read_byte(&gfxFont->last),
                gw, gh, xa;
        int8_t    xo, yo;
        int16_t   minx = _width, miny = _height, maxx = -1, maxy = -1,
                gx1, gy1, gx2, gy2, ts = (int16_t)textsize,
                ya = ts * (uint8_t)pgm_read_byte(&gfxFont->yAdvance);

        while((c = *str++)) {
            if(c != '\n') { // Not a newline
                if(c != '\r') { // Not a carriage return, is normal char
                    if((c >= first) && (c <= last)) { // Char present in current font
                        c    -= first;
                        glyph = &(((GFXglyph *)pgm_read_pointer(&gfxFont->glyph))[c]);
                        gw    = pgm_read_byte(&glyph->width);
                        gh    = pgm_read_byte(&glyph->height);
                        xa    = pgm_read_byte(&glyph->xAdvance);
                        xo    = pgm_read_byte(&glyph->xOffset);
                        yo    = pgm_read_byte(&glyph->yOffset);
                        if(wrap && ((x + (((int16_t)xo + gw) * ts)) >= _width)) {
                            // Line wrap
                            x  = 0;  // Reset x to 0
                            y += ya; // Advance y by 1 line
                        }
                        gx1 = x   + xo * ts;
                        gy1 = y   + yo * ts;
                        gx2 = gx1 + gw * ts - 1;
                        gy2 = gy1 + gh * ts - 1;
                        if(gx1 < minx) minx = gx1;
                        if(gy1 < miny) miny = gy1;
                        if(gx2 > maxx) maxx = gx2;
                        if(gy2 > maxy) maxy = gy2;
                        x += xa * ts;
                    }
                } // Carriage return = do nothing
            } else { // Newline
                x  = 0;  // Reset x
                y += ya; // Advance y by 1 line
            }
        }
        // End of string
        *x1 = minx;
        *y1 = miny;
        if(maxx >= minx) *w  = maxx - minx + 1;
        if(maxy >= miny) *h  = maxy - miny + 1;

    } else { // Default font

        uint16_t lineWidth = 0, maxWidth = 0; // Width of current, all lines

        while((c = *str++)) {
            if(c != '\n') { // Not a newline
                if(c != '\r') { // Not a carriage return, is normal char
                    if(wrap && ((x + textsize * 6) >= _width)) {
                        x  = 0;            // Reset x to 0
                        y += textsize * 8; // Advance y by 1 line
                        if(lineWidth > maxWidth) maxWidth = lineWidth; // Save widest line
                        lineWidth  = textsize * 6; // First char on new line
                    } else { // No line wrap, just keep incrementing X
                        lineWidth += textsize * 6; // Includes interchar x gap
                    }
                } // Carriage return = do nothing
            } else { // Newline
                x  = 0;            // Reset x to 0
                y += textsize * 8; // Advance y by 1 line
                if(lineWidth > maxWidth) maxWidth = lineWidth; // Save widest line
                lineWidth = 0;     // Reset lineWidth for new line
            }
        }
        // End of string
        if(lineWidth) y += textsize * 8; // Add height of last (or only) line
        if(lineWidth > maxWidth) maxWidth = lineWidth; // Is the last or only line the widest?
        *w = maxWidth - 1;               // Don't include last interchar x gap
        *h = y - *y1;

    } // End classic vs custom font
}


// Return the size of the display (per current rotation)
<<<<<<< HEAD
int16_t width(void)  {
    return _width;
}

int16_t height(void)  {
=======
int16_t width(void) const {
    return _width;
}

int16_t height(void) const {
>>>>>>> 9c79c2342a68a936ef2bb8d46bbb0e804ebf5b5d
    return _height;
}


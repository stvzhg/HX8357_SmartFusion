#include "hx8537.h"
#include <inttypes.h>
#include "drivers/mss_spi/mss_spi.h"
#include "drivers/mss_gpio/mss_gpio.h"

void writecommand(uint8_t c) {
    MSS_GPIO_set_output(_dc, 0);
    MSS_SPI_init(&g_mss_spi1 );
    MSS_SPI_configure_master_mode
    (
            &g_mss_spi1,
            MSS_SPI_SLAVE_0,
            MSS_SPI_MODE_0,
            MSS_SPI_PCLK_DIV_2,
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
                    MSS_SPI_MODE_0,
                    MSS_SPI_PCLK_DIV_2,
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
#if defined (USE_FAST_PINIO)
    *dcport |=  dcpinmask;
  *csport &= ~cspinmask;
#else
    digitalWrite(_dc, HIGH);
    digitalWrite(_cs, LOW);
#endif

    if (_sclk == -1)
        SPI.beginTransaction(spi_settings);

    spiwrite(color >> 8);
    spiwrite(color);

    if (_sclk == -1)
        SPI.endTransaction();

#if defined (USE_FAST_PINIO)
    *csport |= cspinmask;
#else
    digitalWrite(_cs, HIGH);
#endif
}

void drawPixel(int16_t x, int16_t y, uint16_t color) {

    if((x < 0) ||(x >= _width) || (y < 0) || (y >= _height)) return;


    setAddrWindow(x,y,x,y);

#if defined (USE_FAST_PINIO)
    *dcport |=  dcpinmask;
  *csport &= ~cspinmask;
#else
    digitalWrite(_dc, HIGH);
    digitalWrite(_cs, LOW);
#endif

    if (_sclk == -1)
        SPI.beginTransaction(spi_settings);

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

    spiwrite(color >> 8);
    spiwrite(color);

    if (_sclk == -1)
        SPI.endTransaction();

#if defined (USE_FAST_PINIO)
    *csport |= cspinmask;
#else
    digitalWrite(_cs, HIGH);
#endif
}


void drawFastVLine(int16_t x, int16_t y, int16_t h,
                                    uint16_t color) {

    // Rudimentary clipping
    if((x >= _width) || (y >= _height)) return;

    if((y+h-1) >= _height)
        h = _height-y;

    setAddrWindow(x, y, x, y+h-1);

    uint8_t hi = color >> 8, lo = color;

#if defined (USE_FAST_PINIO)
    *dcport |=  dcpinmask;
  *csport &= ~cspinmask;
#else
    digitalWrite(_dc, HIGH);
    digitalWrite(_cs, LOW);
#endif


    if (_sclk == -1)
        SPI.beginTransaction(spi_settings);

    while (h--) {
        spiwrite(hi);
        spiwrite(lo);
    }

    if (_sclk == -1)
        SPI.endTransaction();

#if defined (USE_FAST_PINIO)
    *csport |= cspinmask;
#else
    digitalWrite(_cs, HIGH);
#endif
}


void drawFastHLine(int16_t x, int16_t y, int16_t w,
                                    uint16_t color) {

    // Rudimentary clipping
    if((x >= _width) || (y >= _height)) return;
    if((x+w-1) >= _width)  w = _width-x;
    setAddrWindow(x, y, x+w-1, y);

    uint8_t hi = color >> 8, lo = color;

#if defined (USE_FAST_PINIO)
    *dcport |=  dcpinmask;
  *csport &= ~cspinmask;
#else
    digitalWrite(_dc, HIGH);
    digitalWrite(_cs, LOW);
#endif

    if (_sclk == -1)
        SPI.beginTransaction(spi_settings);

    while (w--) {
        spiwrite(hi);
        spiwrite(lo);
    }

    if (_sclk == -1)
        SPI.endTransaction();

#if defined (USE_FAST_PINIO)
    *csport |= cspinmask;
#else
    digitalWrite(_cs, HIGH);
#endif
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

#if defined (USE_FAST_PINIO)
    *dcport |=  dcpinmask;
  *csport &= ~cspinmask;
#else
    digitalWrite(_dc, HIGH);
    digitalWrite(_cs, LOW);
#endif

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

#if defined (USE_FAST_PINIO)
    *csport |= cspinmask;
#else
    digitalWrite(_cs, HIGH);
#endif
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
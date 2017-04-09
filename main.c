#include <stdio.h>
#include <stdlib.h>
#include "drivers/mss_uart/mss_uart.h"
#include "drivers/mss_gpio/mss_gpio.h"
#include "HX8357_SmartFusion/hx8357.h"
#include "HX8357_SmartFusion/Fonts/FreeSerifBold24pt7b.h"
#include "HX8357_SmartFusion/Fonts/FreeSerifItalic24pt7b.h"
#include "mytimer.h"

#define RX_BUFF_SIZE 64
mss_gpio_id_t _dc = MSS_GPIO_0;
mss_gpio_id_t _rst = MSS_GPIO_1;
uint8_t second = 0;
uint8_t minute = 0;
uint8_t hour = 0;

__volatile__ uint32_t count;
__attribute__ ((interrupt)) void Fabric_IRQHandler( void )
{
	update_timers();
    NVIC_ClearPendingIRQ( Fabric_IRQn );
}

uint8_t rx_buff[RX_BUFF_SIZE] = "";
int i = 0;

void set_uart_1(){
	MSS_UART_init
		 (
			 &g_mss_uart1,
			 MSS_UART_115200_BAUD,
			 MSS_UART_DATA_8_BITS | MSS_UART_NO_PARITY | MSS_UART_ONE_STOP_BIT
		 );
	printf("AT+Project373\n\r");
	MSS_UART_polled_tx_string(&g_mss_uart1, (uint8_t *)"$$$");
	while ( ! (MSS_UART_TEMT && MSS_UART_get_tx_status( &g_mss_uart1 ) ) )
	{
		;
	}
}

void update_time(){
	setCursor(20, 120);
	setTextColor(color565(0x66, 0xcc, 0xff), color565(0, 0, 0));
	if (hour < 10) {
		write('0');
		write((hour + '0'));
	} else {
		write((hour / 10 + '0'));
		write((hour % 10 + '0'));
	}
	write(':');
	if (minute < 10) {
		write('0');
		write((minute + '0'));
	} else {
		write((minute / 10 + '0'));
		write((minute % 10 + '0'));
	}
	write(':');
	if (second < 10) {
		write('0');
		write((second + '0'));
	} else {
		write((second / 10 + '0'));
		write((second % 10 + '0'));
	}
	second++;
	if (second == 60) {
		second = 0;
		minute ++;
		if (minute == 60) {
			minute = 0;
			hour++;
			if (hour == 24) {
				hour = 0;
			}
		}
	}
	setTextColor(color565(0xFF, 0xFF, 0xFF), color565(0, 0, 0));
	setCursor(20, 120);
	if (hour < 10) {
		write('0');
		write((hour + '0'));
	} else {
		write((hour / 10 + '0'));
		write((hour % 10 + '0'));
	}
	write(':');
	if (minute < 10) {
		write('0');
		write((minute + '0'));
	} else {
		write((minute / 10 + '0'));
		write((minute % 10 + '0'));
	}
	write(':');
	if (second < 10) {
		write('0');
		write((second + '0'));
	} else {
		write((second / 10 + '0'));
		write((second % 10 + '0'));
	}
}

void fetchbluetooth() {
	int rx_size = 0;
	rx_size = MSS_UART_get_rx( &g_mss_uart1, rx_buff, sizeof(rx_buff) );
	  if (rx_size > 0)
	  {
		 int i;
		 for (i = 0; i < rx_size; ++i) {
			 printf("%d %c\n\r", rx_size, rx_buff[i]);
		 }
	  }
}

int main(){
	set_uart_1();
	//startTimerContinuous(&update_time, 0x4c4b400, 0);
	//startTimerContinuous(&fetchbluetooth, 0x4c4b4, 0);
	//start_hardware_timer();

	GFXfont myFont = FreeSerifItalic24pt7b;
	begin(HX8357D);
	//invertDisplay((1 == 1));
	setRotation(0);
	delay(100);
	fillScreen(color565(220, 255, 191));
	delay(10);
	fillScreen(color565(0, 0, 0));
	delay(10);
	fillScreen(color565(0x66, 0xcc, 0xff));
	delay(10);
	drawRect(110,  200,  100,  100, color565(0, 0, 0));
	delay(10);
	setFont(&myFont);
	drawChar(150,  250, 'a', color565(0, 0, 0), color565(0xFF, 0xFF, 0xFF), 3);
	delay(10);
	fillScreen(color565(0x66, 0xcc, 0xff));
	delay(10);
	setTextColor(color565(0xFF, 0xFF, 0xFF), color565(0, 0, 0));
	setCursor(20, 240);
	write('H');
	write('e');
	write('l');
	write('l');
	write('o');
	setCursor(getCursorX() + 10, getCursorY());
	write('W');
	write('o');
	write('r');
	write('l');
	write('d');
	setTextSize(1.2);
	printStr("DEBUG", 20, 300);
	setTextSize(1);
	printStr("Info list", 20, 360);

	while(1);

	return 0;
}

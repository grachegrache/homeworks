#include <wiringPi.h>           //WiringPi headers
#include <lcd.h>                //LCD headers from WiringPi
#include <stdio.h>              //Needed for the printf function below


//Pin numbers below are the WiringPi pin numbers

#define LCD_RS  11               //Register select pin
#define LCD_E   10               //Enable Pin
#define LCD_D4  6               //Data pin 4
#define LCD_D5  5               //Data pin 5
#define LCD_D6  4               //Data pin 6
#define LCD_D7  1               //Data pin 7

//prior.. wiringPiSetup() needed.
int lcd_start() {
	int lcd;                //Handle for LCD

	wiringPiSetup();
	//Initialise LCD(int rows, int cols, int bits, int rs, 
	//int enable, int d0, int d1, int d2, int d3, int d4, int d5, int d6, int d7)
	if (
			lcd =
			lcdInit (2, 16,4, LCD_RS, LCD_E ,LCD_D4 , 
				LCD_D5, LCD_D6,LCD_D7,0,0,0,0)
	   ){
		printf ("lcd init failed! \n");
		return -1 ;
	}
	lcdPosition(lcd,0,0);  //Position cursor on the first line in the first column
	lcdPuts(lcd, "*^^* HELLO THIS IS JOSS TAB CTRL");  
	/*
	char s[100];
	int i;
	   for(i=0; i<4; i++) {
	   int ret = get_relay_value(i);
	   printf("ret:%d\n",ret);
	   sprintf(s,"[%d]tab status :%d\n",i,ret);
	   lcdPosition(lcd,0,0); 
	   lcdPuts(lcd,s);
	   delay(2000);
	   }
	 */
	return lcd;
}



#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include "multitab_client.h"

//wiringPi num..
#define LSW 0 //17
#define RSW 2 //27
#define TGSW 3 //22
#define EXIT_SUCC 0
#define EXIT_FAIL 1

#define MAXTAB 4

int main(void) {
	int lcd;
	//if(wiringPiSetupGpio() == -1) return EXIT_FAIL;
	lcd = lcd_start(); 
	setbuf(stdout,NULL);
	pinMode(LSW, INPUT);
	pinMode(RSW, INPUT);
	pinMode(TGSW, INPUT);
	pullUpDnControl(LSW, PUD_UP);
	pullUpDnControl(RSW, PUD_UP);
	pullUpDnControl(TGSW, PUD_UP);

	int curTab=0;
	int lsw,rsw,tgsw;
	int tabStat[4]={0};
	int occupied=0;
	char s[100];
	// ex) lsw == 0  => switch pushed..
	while(1) {
		delay(100);
		lsw = digitalRead(LSW);
		rsw = digitalRead(RSW);
		tgsw = digitalRead(TGSW);
		printf("lsw, rsw tgsw :%d %d %d\n",lsw,rsw,tgsw);
		if(!occupied) {
			if(lsw == 0) occupied = 1;
			else if(rsw == 0) occupied = 2;
			else if(tgsw == 0) occupied = 3;
		}
		else {
			if(lsw== 1 && occupied == 1) occupied = 0;
			else if(rsw == 1 && occupied == 2) occupied = 0;
			else if(tgsw == 1 && occupied == 3) occupied = 0;
		}

		if(occupied == 1 || occupied == 2) {
			if(occupied == 1) curTab = (curTab-1+MAXTAB)%MAXTAB;
			if(occupied == 2) curTab = (curTab+1)%MAXTAB;
			int ret = get_relay_value(curTab);
			tabStat[curTab] = ret;
			printf("ret:%d\n",ret);
			printf("lcd :%d\n",lcd);
			sprintf(s,"%dth tab is %3s%18s",curTab,ret?"ON":"OFF","");
			lcdPosition(lcd,0,0); 
			lcdPuts(lcd,s);
			delay(200);
		}
		else if(occupied==3) {
			set_relay_value(curTab,!tabStat[curTab]); 
			tabStat[curTab]= !tabStat[curTab];
			sprintf(s,"%dth tab is %3s%18s",curTab,
				tabStat[curTab]?"ON":"OFF","");
			lcdPosition(lcd,0,0); 
			lcdPuts(lcd,s);
			delay(200);
		}
	}
	return EXIT_SUCC;
}


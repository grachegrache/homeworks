#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <stdint.h>

#define SDATA 6
#define STR_CLK 13
#define SHR_CLK 19
#define SHR_CLEAR 26

void allclear(void);

void init(void){
	if(wiringPiSetupGpio() == -1){
		fprintf(stderr, "wiringPiSetupGpio() error\n");
		exit(1);
	}
	pinMode(SDATA, OUTPUT);
	pinMode(STR_CLK, OUTPUT);
	pinMode(SHR_CLK, OUTPUT);
	pinMode(SHR_CLEAR, OUTPUT);
	allclear();
}

void allclear(void){
	digitalWrite(SHR_CLEAR, 0);
	digitalWrite(SHR_CLEAR, 1);
	digitalWrite(STR_CLK, 0);
	digitalWrite(STR_CLK, 1);
}

void set8(uint8_t value){
	int i;
	for(i=0;i<8;i++){
		int mask = 0b1 << i;
		if((value & mask) == ))
			digitalWrite(SDATA, 0);
		else
			digitalWrite(SDATA, 1);
		digitalWrite(SHR_CLK, 0);
		digitalWrite(SHR_CLK, 1);
	}
	digitalWrite(STR_CLK, 0);
	digitalWrite(STR_CLK, 1);
}

int main(void){
	int i;
	uint8_t arr[] = {	~0b10000000,
						~0b01000000,
						~0b00100000,
						~0b00010000,
						~0b00001000,
						~0b00000100,
						~0b00000010,
						~0b00000001};
	init();

	for(i=0;i<8;i++){
		set8(arr[i]);
		delay(1000);
	}
	return 1;
}

#include <stdio.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <wiringPi.h>
#include <string.h>
#include <stdlib.h>
#include "multitab_client.h"

#define ultrasonic_log(x,y,z) syscall(391,x,y,z)

// wiringPi number.. not bcm
#define trigPin1 28
#define echoPin1 29

#define SETDISTANCE 20
#define TABNUM 3

//vcc 5v

//network
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>

#define OUTPUT_DIST_LEN 12

int getDistance(int num);
void decToStr(int dec, char* str);

//network

int main(void)
{
	printf("start..\n");
	if(wiringPiSetup () == -1) return 1;
	pinMode (trigPin1, OUTPUT);
	pinMode (echoPin1, INPUT);
	char output[12];

	ultrasonic_log(1,TABNUM,1);

	while(1){
		int d;
		d = getDistance(0);
		printf("d:%d\n",d);
		decToStr(d,output);
		output[4] = 0;
		printf("check...%s\n",output);
		usleep(400000);
	}
}
int getDistance(int num) {
	int trigPin,echoPin;
	int distance=0;
	int pulse = 0;
	if(num == 0) { 
		trigPin = trigPin1;
		echoPin = echoPin1;
	}
	else {
		printf("thread_sonic init error\n");
		return 0;
	}
	digitalWrite (trigPin, LOW);
	usleep(2);
	digitalWrite (trigPin, HIGH);
	usleep(20);
	digitalWrite (trigPin, LOW);

	while(digitalRead(echoPin) == LOW);

	long startTime = micros();
	while(digitalRead(echoPin) == HIGH);
	long travelTime = micros() - startTime;

	distance = travelTime / 58;
	int setval = -1;
	if(distance <= SETDISTANCE) setval=1;
	else setval = 0;
	set_relay_value(TABNUM,setnum);
	ultrasonic_log(distance,TABNUM,setnum);

	printf("%d sonic, Distance: %dcm\n", num,distance);
	return distance;
}
void decToStr(int dec, char* str) {
	int i;
	for(i=3; i>=0; i--) {
		str[i] = dec % 10 + '0';
		dec/=10;
	}
}

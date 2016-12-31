// index  value 
//

#include <stdio.h>
#include <errno.h>
#include <wiringPi.h>

#define SW0	0
#define SW1	2
#define SW2	3
#define SW3	4

#define SwitchCount 4

int Switchs[SwitchCount] = {SW0, SW1, SW2, SW3};
int SwitchStatus[SwitchCount] = {0,};


int getStatus(int index);
int init(void);
void initDevice(void);
void SwitchON(int nIDX);
void SwitchOFF(int nIDX);


int main(int argc, char * argv[])
{
	if(argc != 2)
	{
		printf("you put wrong argument!\n");
		return -1;
	}

	int status = -1;
	int input = atoi(argv[1]);

	if(  input >4 || input <0)
	{
		printf("error: Switch index must be 0~3 \n");
		return -1;
	}	


	
	init();
	initDevice();
	status = getStatus(input);
	printf("status is %d\n", status);

	if( status ==0)
		return 0;
	else if( status == 1)
		return 1;
	else
		return -1;



	
}

int getStatus(int index)
{
	if(digitalRead(Switchs[index])==LOW)
		return 0;
	else 
		return 1;
	
}



//////////////////
///wiringPi setup
///////

int init(void)
{
	printf("Start init Device\n");
	
	if(wiringPiSetup() == -1){
		printf("unable to setupwiringPi:%s\n", strerror(errno));
		return -1;
	}

	initDevice();

	printf("End init Device\n");
	return 0;
}

////////
//Relay GPIO 할당 
///////
void initDevice(void)
{
	int i;
	for(i=0; i<SwitchCount; i++)
	{
		pinMode(Switchs[i], OUTPUT);
//		SwitchOFF(i);
	}
}

////Relay on
void SwitchON(int nIDX)
{
	digitalWrite(Switchs[nIDX], 0); // ON
	SwitchStatus[nIDX]=1;
	printf("Turn On Switch %d\n", nIDX);
}

////Relay off
void SwitchOFF(int nIDX)
{
	digitalWrite(Switchs[nIDX], 1);
	SwitchStatus[nIDX]=0;
	printf("Turn OFF Switch %d\n", nIDX);
}







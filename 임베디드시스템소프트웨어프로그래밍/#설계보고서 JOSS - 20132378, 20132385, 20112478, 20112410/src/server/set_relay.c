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
	if(argc != 3)
	{
		printf("you put wrong argument!\n");
		return -1;
	}

//	int status = -1;
	int index = atoi(argv[1]);
	int value = atoi(argv[2]);
	printf("index = %d , value = %d\n", index, value);

	if(  index >4 || index <0)
	{
		printf("error: Switch index must be 0~3 \n");
		return -1;
	}	
	if(!(value ==0 |value ==1))
	{
		printf("error : you put wrong  value.\n");
		return -1;
	}


	init();


	int i;
	printf("***************before*********************\n");
	for(i=0; i<SwitchCount; i++)
	{
		SwitchStatus[i] = getStatus(i);
		printf("SW%d's status  = %d\n",i,SwitchStatus[i]);
	}	

	if(value==1)
	{
		SwitchON(index);
	}
	else if(value == 0)
	{
		SwitchOFF(index);
	}



   printf("***************after*********************\n");
        for(i=0; i<SwitchCount; i++)
        {
                SwitchStatus[i] = getStatus(i);
                printf("SW%d's status  = %d\n",i,SwitchStatus[i]);
        }    

	for(i  = 0 ; i< 4; i++)
	{
		printf("%d \n", Switchs[i]);
	}

	return 0;
}


int getStatus(int index)
{
//	pinMode(Switchs[index], INPUT);
	if(digitalRead(Switchs[index])==HIGH)
		return 1;
	else 
		return 0;
	
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
	digitalWrite(Switchs[nIDX], HIGH); // ON
	SwitchStatus[nIDX]=1;
	printf("Turn On Switch %d\n", nIDX);
}

////Relay off
void SwitchOFF(int nIDX)
{
	digitalWrite(Switchs[nIDX], LOW);
	SwitchStatus[nIDX]=0;
	printf("Turn OFF Switch %d\n", nIDX);
}







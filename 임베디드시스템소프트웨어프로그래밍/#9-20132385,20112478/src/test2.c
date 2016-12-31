#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define MAX_BUFF 32
#define LINE_BUFF 16
#define FPGA_TEXT_LCD_DEVICE "/dev/fpga_text_lcd"
#define MAX_ARGV 3

int main(int argc, char** argv){
	int i;
	int dev;
	int sizes[MAX_ARGV];
	unsigned char string[32];
	unsigned char spaces[32];
	memset(spaces,' ',MAX_BUFF);
	memset(string,0,sizeof(string));

	if(argc<2||argc>3){
		printf("invalid value arguments!\n");
		return -1;
	}

	sizes[1] = strlen(argv[1]);
	sizes[2] = strlen(argv[2]);

	if(sizes[1] > LINE_BUFF || sizes[2] > LINE_BUFF){
		printf("16 alphanumeric characters on a line!\n");
		return -1;
	}

	dev = open(FPGA_TEXT_LCD_DEVICE, O_WRONLY);

	if(dev<0){
		printf("device open error: %s\n",FPGA_TEXT_LCD_DEVICE);
		return -1;
	}

	for(i=0; i<LINE_BUFF; i++) {
		if( !(i< sizes[1] || i< sizes[2]) ) break;
		if(i<sizes[1]) string[i] = argv[1][i]; 
		if(i<sizes[2]) string[i+LINE_BUFF] = argv[2][i];
		write(dev,string,MAX_BUFF);
		sleep(1);
	}
	for(i=0; i<5; i++) {
		write(dev,spaces,MAX_BUFF);
		sleep(1);
		write(dev,string,MAX_BUFF);
	}

	close(dev);
	return(0);
}

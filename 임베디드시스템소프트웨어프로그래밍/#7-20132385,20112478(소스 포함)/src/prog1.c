#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define LED_DEVICE "/dev/fpga_led"

int main(int argc, char *argv[]){
	int dev;
	unsigned char data;
	unsigned char retval;

	dev = open(LED_DEVICE, O_RDWR);
	if(dev < 0){
		printf("device open error : %s\n", LED_DEVICE);
		exit(1);
	}

	for(data=1<<7; data; data>>=1) {
		write(dev, &data, 1);
		sleep(1);
		retval = read(dev, &data, 1);
		printf("current LED value : %d\n", data);
	}
	//led off
	data = 0;
	write(dev, &data, 1);

	close(dev);
	return(0);
}

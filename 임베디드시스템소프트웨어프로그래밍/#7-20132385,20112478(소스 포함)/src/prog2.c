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

	if(argc < 2 ){
		printf("please input the parameter! \n");
		printf("ex)./test_led a1\n");
		return -1;
	}
	if(argc > 9) {
		printf("too many arg\n");
		return -1;
	}
	dev = open(LED_DEVICE, O_RDWR);
	if(dev < 0){
		printf("device open error : %s\n", LED_DEVICE);
		exit(1);
	}
	int i;
	unsigned char cur=0;
	for(i=1; i<argc; i++) {
		data = atoi(argv[i]);
		cur += (1<<8) >> data;
		write(dev, &cur, 1);
		retval = read(dev, &cur, 1);
		if(retval < 0){
			printf("read error!\n");
			return -1;
		}
		printf("current LED value : %d\n", cur);
		sleep(1);
	}
	//모든 LED 소등
	data = 0 ;
	write(dev, &data, 1); 
	close(dev);
	return(0);
}

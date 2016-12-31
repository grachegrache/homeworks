#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#define MAX_DIGIT 4
#define FND_DEVICE "/dev/fpga_fnd"

int main(int argc, char* argv[]){
	int dev;
	int i;
	int str_size;
	unsigned char data[4];
	unsigned char retval;
	memset(data, 0, sizeof(data));

	if(argc != 2){
		printf("please input the parameter!\n");
		printf("ex)%s 1234\n", argv[0]);
		return -1;
	}
	str_size = strlen(argv[1]);

	if(str_size > MAX_DIGIT){
		printf("warning! 4 digit number only!\n");
		str_size = MAX_DIGIT;
	}

	str_size = strlen(argv[1]);

	if(str_size > MAX_DIGIT){
		printf("warning! 4 digit number only!\n");
		str_size = MAX_DIGIT;
	}

	for(i=0;i<str_size;i++){
		if((argv[1][i] < 0x30) || (argv[1][i] > 0x39)){
			printf("error! invalid value!\n");
			return -1;
		}

		data[i] = argv[1][i] - 0x30;
	}

	dev = open(FND_DEVICE, O_RDWR);

	if(dev < 0){
		printf("device open error: %s\n", FND_DEVICE);
		exit(1);
	}

	retval = write(dev, &data, 4);

	if(retval < 0){
		printf("write error!\n");
		return -1;
	}

	memset(data, 0, sizeof(data));
	sleep(1);
	retval = read(dev, &data, 4);

	if(retval < 0){
		printf("read error!\n");
		return -1;
	}

	printf("current FND value: ");

	for(i=0;i<str_size;i++)
		printf("%d", data[i]);

	printf("\n");
	close(dev);
	return 0;
}

#include <stdio.h>
#include <math.h>
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
	int a_size,b_size;
	unsigned char data[4];
	unsigned char retval;
	const int MAX_NUM = pow(10,MAX_DIGIT) -1; // 7seg max int..

	memset(data, 0, sizeof(data));

	if(argc != 4){
		printf("please input two parameters!\n");
		printf("ex)%s 34 * 12( You must input first value is better than sec\n", argv[0]);
		return -1;
	}
	int a,b,c;
	char op;
	a_size = strlen(argv[1]);
	b_size = strlen(argv[3]);
	op = argv[2][0];
	char ops[] = {'*','+','-','/'};
	for(i=0; i<4; i++)
		if(op == ops[i]) break;
	if(i==4) {
		printf("operator error... '%c'\n",op);
		return -1;
	}

	//argv check... is not number... error
	for(i=0;i<a_size;i++){
		if( ( (argv[1][i] < 0x30) || (argv[1][i] > 0x39))) {
			printf("error! invalid value!\n");
			return -1;
		}
	}
	for(i=0;i<b_size;i++){
		if( ( (argv[3][i] < 0x30) || (argv[3][i] > 0x39))) {
			printf("error! invalid value!\n");
			return -1;
		}
	}
	// ------ check end!!

	// calc start
	a= atoi(argv[1]);
	b= atoi(argv[3]);
	switch(op) {
		case '+':
			c = a+b;
			break;
		case '-':
			c = a-b;
			break;
		case '*':
			c = a*b;
			break;
		case '/':
			c = a/b;
			break;
	}
	if( c< 0) {
		printf("can't express minus\n");
		return -1;
	}
	for(i=3;i>=0; i--) {
		data[i] = c%10;
		c/=10;
	}

	dev = open(FND_DEVICE, O_RDWR); if(dev < 0){
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


	for(i=0;i<4;i++)
		printf("data[i] : %d",i,data[i]);

	printf("\n");
	close(dev);
	return 0;
}

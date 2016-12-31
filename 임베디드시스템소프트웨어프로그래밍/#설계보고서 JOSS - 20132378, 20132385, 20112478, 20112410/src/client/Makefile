CC = gcc
OBJS = multitab_client.o gpioSwitch.o ultrasonic.o lcd.o
TARGET = gpioSwitch ultrasonic

all : $(TARGET)

multitab_client.o : multitab_client.h multitab_client.c
	$(CC) -c multitab_client.c -L/usr/lib/arm-linux-gnueabihf -lcurl
gpioSwitch.o : gpioSwitch.c multitab_client.o
	$(CC) -c gpioSwitch.c -lwiringPi

lcd.o : lcd.c 
	$(CC) -c lcd.c -lwiringPi -lwiringPiDev

gpioSwitch : multitab_client.o gpioSwitch.o lcd.o
	$(CC) -o gpioSwitch gpioSwitch.o multitab_client.o lcd.o -lwiringPi -lwiringPiDev -L/usr/lib/arm-linux-gnueabihf -lcurl
ultrasonic : multitab_client.o ultrasonic.c
	$(CC) -o ultrasonic multitab_client.o ultrasonic.c -lwiringPi -lwiringPiDev -L/usr/lib/arm-linux-gnueabihf -lcurl

clean :
	rm -f $(OBJS) $(TARGET)

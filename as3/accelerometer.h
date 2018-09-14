#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <stdint.h>



int initI2cBus(char* bus, int address);
int16_t* readI2cReg(int i2cFileDesc, unsigned char regAddr);
void writeI2cReg(int i2cFileDesc, unsigned char regAddr,unsigned char value);
int echo_to_file(char* file, char* buff);
int initAccelerometer();
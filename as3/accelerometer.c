#include "accelerometer.h"
#include <string.h>
#include "general.h"
#define I2CDRV_LINUX_BUS0 "/dev/i2c-0"
#define I2CDRV_LINUX_BUS1 "/dev/i2c-1"
#define I2CDRV_LINUX_BUS2 "/dev/i2c-2"
#define SLOTS "/sys/devices/platform/bone_capemgr/slots"
#define I2C_DEVICE_ADDRESS 0x1C
#define I2C_ACTIVE_MODE 0x2A
#define NUM_OF_BYTE 7

int initI2cBus(char* bus, int address)
{
int i2cFileDesc = open(bus, O_RDWR);
if (i2cFileDesc < 0) {
printf("I2C: Unable to open bus for read/write (%s)\n", bus);
perror("Error is:");
exit(1);
}
int result = ioctl(i2cFileDesc, I2C_SLAVE, address);
if (result < 0) {
perror("I2C: Unable to set I2C device to slave address.");
exit(1);
}
return i2cFileDesc;
}

int16_t* readI2cReg(int i2cFileDesc, unsigned char regAddr)
{
// To read a register, must first write the address
int res = write(i2cFileDesc, &regAddr, NUM_OF_BYTE);
if (res != NUM_OF_BYTE) {
perror("I2C: Unable to write to i2c register.");
exit(1);
}
// Now read the value and return it
unsigned char value[NUM_OF_BYTE];
int16_t* result = malloc(3*sizeof(int16_t));
res = read(i2cFileDesc, value, NUM_OF_BYTE * sizeof(char));
if (res != NUM_OF_BYTE) {
    printf("res is %d\n",res);
perror("I2C: Unable to read from i2c register");
exit(1);
}
result[0] = ((int)value[1] << 8) | ((int)value[2]);
result[1] = ((int)value[3] << 8) | ((int)value[4]);
result[2] = ((int)value[5] << 8) | ((int)value[6]);

return result;
}

void writeI2cReg(int i2cFileDesc, unsigned char regAddr,unsigned char value)
{
    unsigned char buff[2];
    buff[0] = regAddr;
    buff[1] = value;
    int res = write(i2cFileDesc, buff, 2);
    if (res != 2) {
        perror("I2C: Unable to write i2c register.");
        exit(1);
    }
}

int echo_to_file(char* file, char* buff)
{
	// Open direction file
	int file_desc = open(file, O_WRONLY);
	if (file_desc < 0) {
		printf("FILEIODRV ERROR: Unable to open file for write: %s\n", file);
		perror("Error is:");
		exit(-1);
	}

	int bytes_written = write(file_desc, buff, strlen(buff));
	die_on_failed("Unable to write to file for echo.");

	// Close
	close(file_desc);

	return bytes_written;
}

int initAccelerometer()
{
    int result = echo_to_file(SLOTS, "BB-I2C1");
		if (result <= 0) {
			printf("WARNING: Unable to write to cape manager for I2C; may mean it's already loaded...\n");
		}
    int i2cFileDesc = initI2cBus(I2CDRV_LINUX_BUS1, I2C_DEVICE_ADDRESS);
    writeI2cReg(i2cFileDesc,I2C_ACTIVE_MODE,0x01);
    return i2cFileDesc;
}
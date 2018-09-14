#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#define JoyStickUpFile "/sys/class/gpio/gpio26/value"
#define JoyStickRightFile "/sys/class/gpio/gpio47/value"
#define JoyStickDownFile "/sys/class/gpio/gpio46/value"
#define JoyStickLeftFile "/sys/class/gpio/gpio65/value"
#define JoyStickStraightDownFile "/sys/class/gpio/gpio27/value"
#define JoyStickExportFile "/sys/class/gpio/export"
#define JoyStickUpDirectionFile "/sys/class/gpio/gpio26/direction"
#define JoyStickRightDirectionFile "/sys/class/gpio/gpio47/direction"
#define JoyStickDownDirectionFile "/sys/class/gpio/gpio46/direction"
#define JoyStickLeftDirectionFile "/sys/class/gpio/gpio65/direction"
#define JoyStickStraightDownDirectionFile "/sys/class/gpio/gpio27/direction"




int checkJoyStick();
int setExportFile();
int setDirectionFile();
void* backgroundFunction(void* arg);
int getBpm();
void setBpm(int newValue);
int getVol();
void setVol(int newValue);
int getModel();
void setModel(int newValue);
void initJoyStickThread();
void JoyStickThreadCleanUp();
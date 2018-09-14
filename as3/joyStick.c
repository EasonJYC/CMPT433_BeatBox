#include "joyStick.h"
#include "accelerometer.h"
#include "audioMixer_template.h"
#include <math.h>
#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3
#define STRAIGHTDOWN 4
#define X 5
#define Y 6
#define Z 7
#define INITIAL_ADDRESS 0X00
#define X_BOUNDARY 10000
#define Y_BOUNDARY 10000
#define Z_BOUNDARY 30000
static int bpm = 120;
static int volume = 80;
static int running = 1;
static int model = 0;
static int filedesc;
static pthread_t threadId;
int16_t* test;
int setExportFile()
{
	FILE* file = fopen(JoyStickExportFile, "w");
	if(file == 0)
	{
		printf("error opening %s\n",JoyStickExportFile);
	}
	// setvbuf ( file , NULL , _IOFBF , 1024 );
	fprintf(file,"%d",26);
	fclose(file);
	file = fopen(JoyStickExportFile, "w");
	fprintf(file,"%d",27);
	fclose(file);
	file = fopen(JoyStickExportFile, "w");
	fprintf(file,"%d",46);
	fclose(file);
	file = fopen(JoyStickExportFile, "w");
	fprintf(file,"%d",47);
	fclose(file);
	file = fopen(JoyStickExportFile, "w");
	fprintf(file,"%d",65);
	fclose(file);
	return 0;
}


int setDirectionFile()
{
	FILE* JS_UpDirection = fopen(JoyStickUpDirectionFile,"w");
	FILE* JS_DownDirection = fopen(JoyStickDownDirectionFile,"w");
	FILE* JS_RightDirection = fopen(JoyStickRightDirectionFile,"w");
	FILE* JS_LeftDirection = fopen(JoyStickLeftDirectionFile,"w");
	FILE* JS_StraightDownDirection = fopen(JoyStickDownDirectionFile,"w");
	if(JS_UpDirection == NULL)
	{
		printf("error opening %s\n",JoyStickUpFile);
	}
	if(JS_DownDirection == NULL)
	{
		printf("error opening %s\n",JoyStickDownFile);
	}
	if(JS_RightDirection == NULL)
	{
		printf("error opening %s\n",JoyStickRightFile);
	}
	if(JS_LeftDirection == NULL)
	{
		printf("error opening %s\n",JoyStickLeftFile);
	}
    if(JS_StraightDownDirection == NULL)
    {
        printf("error opening %s\n",JoyStickDownDirectionFile);
    }


	fprintf(JS_UpDirection,"in");
	fprintf(JS_DownDirection,"in");
	fprintf(JS_RightDirection,"in");
	fprintf(JS_LeftDirection,"in");
    fprintf(JS_StraightDownDirection,"in");
	fclose(JS_UpDirection);
	fclose(JS_DownDirection);
	fclose(JS_RightDirection);
	fclose(JS_LeftDirection);
    fclose(JS_StraightDownDirection);
	return 0;
}

int checkJoyStick()
{
	const int max_length = 8;
	int found = 1;
	int result;
	FILE* JS_Up = fopen(JoyStickUpFile,"r");
	FILE* JS_Down = fopen(JoyStickDownFile,"r");
	FILE* JS_Right = fopen(JoyStickRightFile,"r");
	FILE* JS_Left = fopen(JoyStickLeftFile,"r");
    FILE* JS_StraightDown = fopen(JoyStickStraightDownFile,"r");
	
	if(JS_Up == NULL)
	{
		printf("error opening %s\n",JoyStickUpFile);
	}
	if(JS_Down == NULL)
	{
		printf("error opening %s\n",JoyStickDownFile);
	}
	if(JS_Right == NULL)
	{
		printf("error opening %s\n",JoyStickRightFile);
	}
	if(JS_Left == NULL)
	{
		printf("error opening %s\n",JoyStickLeftFile);
	}
    if(JS_StraightDown == NULL)
    {
        printf("error opening %s\n",JoyStickDownDirectionFile);
    }
	char* buffer = malloc(max_length*sizeof(char));
	while(found && running)
	{
		test = readI2cReg(filedesc,INITIAL_ADDRESS);
		if((test[0] > X_BOUNDARY) || (test[0] < -X_BOUNDARY))
			{
				found = 0;
				fclose(JS_Up);
				fclose(JS_Down);
				fclose(JS_Left);
				fclose(JS_Right);
            	fclose(JS_StraightDown);
				free(buffer);
				free(test);
				return X;
			}
		if((test[1] > Y_BOUNDARY) || (test[1] < -Y_BOUNDARY))
			{
				found = 0;
				fclose(JS_Up);
				fclose(JS_Down);
				fclose(JS_Left);
				fclose(JS_Right);
            	fclose(JS_StraightDown);
				free(buffer);
				free(test);
				return Y;
			}
		if((test[2] > Z_BOUNDARY) || (test[2] < -Z_BOUNDARY))
			{
				found = 0;
				fclose(JS_Up);
				fclose(JS_Down);
				fclose(JS_Left);
				fclose(JS_Right);
            	fclose(JS_StraightDown);
				free(buffer);
				free(test);
				return Z;
			}
		fgets(buffer,max_length,JS_Up);
		result = atoi(buffer);
		if(result == 0)
		{
			found = 0;
			
			fclose(JS_Up);
			fclose(JS_Down);
			fclose(JS_Left);
			fclose(JS_Right);
            fclose(JS_StraightDown);
			free(buffer);
			free(test);
			return UP;
		}
		fgets(buffer,max_length,JS_Down);
		result = atoi(buffer);
		if(result == 0)
		{
			found = 0;
			fclose(JS_Up);
			fclose(JS_Down);
			fclose(JS_Left);
			fclose(JS_Right);
            fclose(JS_StraightDown);
			free(buffer);
			free(test);
			return DOWN;
		}
		fgets(buffer,max_length,JS_Left);
		result = atoi(buffer);
		if(result == 0)
		{
			found = 0;
			fclose(JS_Up);
			fclose(JS_Down);
			fclose(JS_Left);
			fclose(JS_Right);
            fclose(JS_StraightDown);
			free(buffer);
			free(test);
			return LEFT; 
		}
		fgets(buffer,max_length,JS_Right);
		result = atoi(buffer);
		if(result == 0)
		{
			found = 0;
			fclose(JS_Up);
			fclose(JS_Down);
			fclose(JS_Left);
			fclose(JS_Right);
            fclose(JS_StraightDown);
			free(buffer);
			free(test);
			return RIGHT;
		}
        fgets(buffer,max_length,JS_StraightDown);
		result = atoi(buffer);
		if(result == 0)
		{
			found = 0;
			fclose(JS_Up);
			fclose(JS_Down);
			fclose(JS_Left);
			fclose(JS_Right);
            fclose(JS_StraightDown);
			free(buffer);
			free(test);
			return STRAIGHTDOWN;
		}


		fclose(JS_Up);
		fclose(JS_Down);
		fclose(JS_Left);
		fclose(JS_Right);
        fclose(JS_StraightDown);
		JS_Up = fopen(JoyStickUpFile,"r");
		JS_Down = fopen(JoyStickDownFile,"r");
		JS_Right = fopen(JoyStickRightFile,"r");
		JS_Left = fopen(JoyStickLeftFile,"r");
        JS_StraightDown = fopen(JoyStickStraightDownFile,"r");
	}

	fclose(JS_Up);
	fclose(JS_Down);
	fclose(JS_Left);
	fclose(JS_Right);
    fclose(JS_StraightDown);
	free(buffer);
	free(test);
	return -1;
}

void* backgroundFunction(void* args)
{
	long seconds = 0;
	long nanoseconds = 200000000;
	struct timespec reqDelay = {seconds, nanoseconds};
	setExportFile();
	setDirectionFile();
    while(running)
    {
        int result = checkJoyStick();
        if(result == UP)
        {
            if(volume <= 95){
                volume += 5;
            }
        }
        else if(result == DOWN)
        {
            if(volume >= 5){
                volume -= 5;
            }
        }
        else if(result == LEFT)
        {
            if(bpm >= 45){
                bpm -= 5;
            }
        }
        else if(result == RIGHT)
        {
            if(bpm <= 295){
                bpm += 5;
            }
        }
        else if(result == STRAIGHTDOWN)
        {
            if(model < 2){
				model++;
			}
			else{
				model = 0;
			}
        }
		else if(result == X)
		{
			model = 6;
			
		}
		else if(result == Y)
		{
			model = 7;
		}
		else if(result == Z)
		{
			model = 8;
		}
		nanosleep(&reqDelay, (struct timespec *) NULL);
    }
	return NULL;
}

int getBpm()
{
    return bpm;
}
void setBpm(int newValue)
{
	bpm = newValue;
}
int getVol()
{
    return volume;
}

void setVol(int newValue)
{
	volume = newValue;
}

int getModel()
{
    return model;
}

void setModel(int newValue)
{
	model = newValue;
}

void initJoyStickThread()
{
	filedesc = initAccelerometer();
    //launch background thread
    pthread_create(&threadId,NULL, backgroundFunction,NULL);
}
void JoyStickThreadCleanUp()
{
	printf("cleaning up joystick thread\n");
    running = 0;
    pthread_join(threadId, NULL);
	printf("done cleaning up joystick thread\n");
}
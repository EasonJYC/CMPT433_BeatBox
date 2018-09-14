#include "udp.h"
#include "joyStick.h"
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>			// for strncmp()
#include <unistd.h>			// for close()
#include <pthread.h>
#include <stdbool.h>
#include "general.h"

#define UDP_PORT 12345
#define MAX_RECEIVE_MESSAGE_LENGTH 1024
#define REPLY_BUFFER_SIZE (1500)
#define VALUES_PER_LINE 4

#define COMMAND_HELP        "help"
#define COMMAND_STOP        "stop"
#define COMMAND_VOL_INC        "increase volume"
#define COMMAND_VOL_DEC       "decrease volume"
#define COMMAND_BPM_FAST  "bpm faster"
#define COMMAND_BPM_SLOW  "bpm slower"
#define COMMAND_MODEL_0         "model 0"
#define COMMAND_MODEL_0         "model 0"
#define COMMAND_MODEL_1        "model 1"
#define COMMAND_MODEL_2         "model 2"
#define COMMAND_BASE			"base"
#define COMMAND_HIHAT			"hihat"
#define COMMAND_SNARE			"snare"
#define COMMAND_UPDATE			"update"
#define TIME_PATH				"/proc/uptime"
static pthread_t s_threadId;
static char replyBuffer[REPLY_BUFFER_SIZE];

// Header
static void *udpListeningThread(void *args);
static void processUDPCommand(char* udpCommand, int socketDescriptor, struct sockaddr_in *pSin);

void UdpListener_startListening(void)
{
	pthread_create(&s_threadId, NULL, &udpListeningThread, NULL);
}

void UdpListener_cleanup(void)
{
	pthread_join(s_threadId, NULL);
}

/*
 *	On the host:
 *		> netcat -u 192.168.0.171 12345
 *		(Change the IP address to your board)
 *
 *	On the host, type in a command and press enter:
 *		help<Enter>
 *
 */



static void *udpListeningThread(void *args)
{
	// Buffer to hold packet data:
	char message[MAX_RECEIVE_MESSAGE_LENGTH];

	// Address
	struct sockaddr_in sin;
	unsigned int sin_len;						// May change.
	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = htonl(INADDR_ANY);	// Host to Network long
	sin.sin_port = htons(UDP_PORT);				// Host to Network short

	// Create the socket for UDP
	int socket_descriptor = socket(PF_INET, SOCK_DGRAM, 0);

	// Bind the socket to the port that we specify
	bind(socket_descriptor, (struct sockaddr*) &sin, sizeof(sin));

	while (!General_isShuttingDown()) {
		// Get the data (blocking)
		// Will change sin (the address) to be the address of the client.
		sin_len = sizeof(sin);
		int bytesRx = recvfrom(socket_descriptor, message, MAX_RECEIVE_MESSAGE_LENGTH, 0,
				(struct sockaddr *) &sin, &sin_len);

		// Make it null terminated (so string functions work):
		message[bytesRx] = 0;
		//printf("Message received (%d bytes): \n\n'%s'\n", bytesRx, message);

		processUDPCommand(message, socket_descriptor, &sin);

		// Transmit a reply (if desired):
		if (strnlen(replyBuffer, REPLY_BUFFER_SIZE) > 0) {
			sin_len = sizeof(sin);
			sendto( socket_descriptor,
				replyBuffer, strnlen(replyBuffer, REPLY_BUFFER_SIZE),
				0,
				(struct sockaddr *) &sin, sin_len);
		}
	}

	// Close socket on shut-down
	close(socket_descriptor);
	return NULL;
}

_Bool isUdpThisCommand(char* udpMessage, const char* command)
{
	return strncmp(udpMessage, command, strlen(command)) == 0;
}


static void processUDPCommand(char* udpCommand, int socketDescriptor, struct sockaddr_in *pSin)
{
	replyBuffer[0] = 0;

	// Extract the value from the message:
    if (isUdpThisCommand(udpCommand, COMMAND_HELP)) {
		sprintf(replyBuffer, "Accepted command examples:\n");
		strcat(replyBuffer, "increase volume      -- increase the volume.\n");
		strcat(replyBuffer, "decrease volume     -- decrease the volume.\n");
		strcat(replyBuffer, "bpm faster  -- increase bpm.\n");
		strcat(replyBuffer, "bpm slower     -- decrease bpm.\n");
		strcat(replyBuffer, "stop       -- cause the server program to end.\n");
        strcat(replyBuffer, "model 0     -- change to model 0.\n");
        strcat(replyBuffer, "model 1     -- change to model 1.\n");
        strcat(replyBuffer, "model 2     -- change to model 2.\n");
		strcat(replyBuffer, "base     -- change to base.\n");
		strcat(replyBuffer, "hihat     -- change to hihat.\n");
		strcat(replyBuffer, "snare     -- change to snare.\n");
		strcat(replyBuffer, "update     -- update the index.\n");
	}



	else if (isUdpThisCommand(udpCommand, COMMAND_VOL_INC)) {
		int currentVol = getVol();
        if(currentVol <= 95){
            currentVol += 5;
        }
        else{
            currentVol = 100;
        }
        setVol(currentVol);
        sprintf(replyBuffer,"increase volume\n");
	}

	else if (isUdpThisCommand(udpCommand, COMMAND_VOL_DEC)) {
        int currentVol = getVol();
        if(currentVol >= 5)
        {
            currentVol -= 5;
        }
        else{
            currentVol = 0;
        }
        setVol(currentVol);
        sprintf(replyBuffer,"decrease volume\n");
    	}

	else if (isUdpThisCommand(udpCommand, COMMAND_BPM_FAST)) {
		int currentBpm = getBpm();
        if(currentBpm <= 295)
        {
            currentBpm += 5;
        }
        else{
            currentBpm = 300;
        }
        setBpm(currentBpm);
        sprintf(replyBuffer,"increase bpm\n");
	}
	else if (isUdpThisCommand(udpCommand, COMMAND_BPM_SLOW)) {
		int currentBpm = getBpm();
        if(currentBpm >= 45)
        {
            currentBpm -= 5;
        }
        else{
            currentBpm = 40;
        }
        setBpm(currentBpm);
        sprintf(replyBuffer,"decrease bpm\n");
	}

	else if (isUdpThisCommand(udpCommand, COMMAND_MODEL_0)) {
		setModel(0);
        sprintf(replyBuffer,"set model 0\n");
	}

	else if (isUdpThisCommand(udpCommand, COMMAND_MODEL_1)) {
		setModel(1);
        sprintf(replyBuffer,"set model 1\n");
	}

    else if (isUdpThisCommand(udpCommand, COMMAND_MODEL_2)) {
		setModel(2);
        sprintf(replyBuffer,"set model 2\n");
	}
	else if (isUdpThisCommand(udpCommand, COMMAND_BASE)) {
		setModel(3);
		sprintf(replyBuffer,"set base\n");
	}
	else if (isUdpThisCommand(udpCommand, COMMAND_HIHAT)) {
		setModel(4);
		sprintf(replyBuffer,"set hihat\n");
	}
	else if (isUdpThisCommand(udpCommand, COMMAND_SNARE)) {
		setModel(5);
		sprintf(replyBuffer,"set snare\n");
	}
	else if (isUdpThisCommand(udpCommand, COMMAND_STOP)) {
		sprintf(replyBuffer, "Program terminating.\n");
		General_shutdown();
	}
	else if(isUdpThisCommand(udpCommand,COMMAND_UPDATE)) {
		int bpm = getBpm();
		int vol = getVol();
		FILE* file = fopen(TIME_PATH,"r");
		if(!file)
		{
			printf("error opening uptime file!\n");
			exit(-1);
		}
		int currenttime;
		fscanf(file,"%d",&currenttime);
		fclose(file);
		sprintf(replyBuffer,"updating: bpm: %d, vol: %d\n, time:%d", bpm, vol,currenttime);
	}

	else {
		sprintf(replyBuffer, "Unknown command. Type 'help' for command list.\n");
	}
}


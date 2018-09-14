#include "audioMixer_template.h"
#include "joyStick.h"
#include "udp.h"
#include "general.h"
#include "accelerometer.h"
#include <inttypes.h>
#define _DEFAULT_SOURCE
#include <unistd.h>
#define BASE_FILE "wave-files/100051__menegass__gui-drum-bd-hard.wav"
#define HIHAT_FILE "wave-files/100053__menegass__gui-drum-cc.wav"
#define SNARE_FILE "wave-files/100059__menegass__gui-drum-snare-soft.wav"
#define CUSTOM_FILE_1 "wave-files/100061__menegass__gui-drum-splash-soft.wav"
#define CUSTOM_FILE_2 "wave-files/100063__menegass__gui-drum-tom-hi-soft.wav"
#define _XOPEN_SOURCE 500

int model;
int volume;
int bpm;
struct timespec reqDelay;
int main()
{
    wavedata_t sample1, sample2, sample3, sample4, sample5;
    long seconds = 0;
	long long nanoseconds = 250000000;
    AudioMixer_init();
    initJoyStickThread();
    UdpListener_startListening();
    AudioMixer_readWaveFileIntoMemory(BASE_FILE, &sample1);
    AudioMixer_readWaveFileIntoMemory(HIHAT_FILE, &sample2);
    AudioMixer_readWaveFileIntoMemory(SNARE_FILE, &sample3);
    AudioMixer_readWaveFileIntoMemory(CUSTOM_FILE_1,&sample4);
    AudioMixer_readWaveFileIntoMemory(CUSTOM_FILE_2,&sample5);
    int i = 0;
    while (!General_isShuttingDown()){
        model = getModel();
        volume = getVol();
        bpm = getBpm();
        nanoseconds = (long long)30*1000000000/bpm;
        struct timespec reqDelay = {seconds, nanoseconds};
        AudioMixer_setVolume(volume);
        if(model == 0){}
        else if(model == 1)
        {
            int result = i % 4;
            if(result == 0){
                AudioMixer_queueSound(&sample1);
                AudioMixer_queueSound(&sample2);
            }
            else if(result == 1)
            {
                AudioMixer_queueSound(&sample2);
            }
            else if(result == 2)
            {
                AudioMixer_queueSound(&sample2);
                AudioMixer_queueSound(&sample3);
            }
            else{
                AudioMixer_queueSound(&sample2);
            }
            i++;
        }
        else if(model == 2){
            int result = i % 4;
            if(result == 0){
                AudioMixer_queueSound(&sample1);
                AudioMixer_queueSound(&sample4);
            }
            else if(result == 1)
            {
                AudioMixer_queueSound(&sample2);
            }
            else if(result == 2)
            {
                AudioMixer_queueSound(&sample4);
                AudioMixer_queueSound(&sample5);
            }
            else{
                AudioMixer_queueSound(&sample5);
            }
            i++;
        }
        else if(model == 3)
        {
            AudioMixer_queueSound(&sample1);
        }
        else if(model == 4)
        {
            AudioMixer_queueSound(&sample2);
        }
        else if(model == 5)
        {
            AudioMixer_queueSound(&sample3);
        }
        else if(model == 6)
        {
            AudioMixer_queueSound(&sample1);
            model = 0;
            setModel(model);
        }
        else if(model == 7)
        {
            AudioMixer_queueSound(&sample2);
            model = 0;
            setModel(model);
        }
        else if(model == 8)
        {
            AudioMixer_queueSound(&sample3);
            model = 0;
            setModel(model);
        }

        nanosleep(&reqDelay, (struct timespec *) NULL);
    }
    AudioMixer_freeWaveFileData(&sample1);
    AudioMixer_freeWaveFileData(&sample2);
    AudioMixer_freeWaveFileData(&sample3);
    AudioMixer_freeWaveFileData(&sample4);
    AudioMixer_freeWaveFileData(&sample5);
    AudioMixer_cleanup();
    JoyStickThreadCleanUp();
    UdpListener_cleanup();
    return 0;

}
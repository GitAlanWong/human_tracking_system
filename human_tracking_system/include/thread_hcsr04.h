#ifndef __THREAD_HCSR04_H
#define __THREAD_HCSR04_H

#include <iostream>
#include <wiringPi.h>
#include <sys/time.h>

using namespace std;

#define hcsr04DistanceAmplitude 800
#define hcsr04DistanceCounterAmplitude 5

int  HCSR04setup();
void HCSR04distance(void);
double HCSR04get_time();

const int echoPin = 4;
const int trlgPin = 5;
double dwStTime = 0.0;
double dwEnTime = 0.0;


int hcsr04Distance = 0;
static int hcsr04DistanceTemp = 0;
static int hcsr04counter = 0;

void* threadhcsr04(void*) 
{
    int ret = HCSR04setup();
    if(0 != ret){
        return NULL;
    }

    while(true){
        digitalWrite(trlgPin,HIGH);
        delayMicroseconds(10);
        digitalWrite(trlgPin,LOW);
        delay(100);

        if(hcsr04counter>=hcsr04DistanceCounterAmplitude){
            hcsr04Distance = hcsr04DistanceTemp;
            hcsr04counter=0;
        }
    }

    return NULL;
}

int HCSR04setup()
{
    pinMode(trlgPin,OUTPUT);   
    digitalWrite(trlgPin,LOW); 

    pinMode(echoPin,INPUT);

    if(0>wiringPiISR(echoPin,INT_EDGE_BOTH,HCSR04distance))
    {
        cerr<<"interrupt function register failure"<<endl;
        return -1;
    }

    return 0;
}

void HCSR04distance(void)
{

    int ret = 0;
    int temp = 0;

    ret = digitalRead(echoPin);

    if(1 == ret){
        dwStTime = HCSR04get_time();
    }else if(0 == ret){
        dwEnTime = HCSR04get_time();
        temp = (int)((dwEnTime - dwStTime) * 17150);

        //限幅滤波法
        if ( ( temp-hcsr04DistanceTemp > hcsr04DistanceAmplitude ) 
            || ( hcsr04DistanceTemp - temp > hcsr04DistanceAmplitude ) )temp=0;
        else
            hcsr04DistanceTemp = temp;
        
        //消抖滤波法
        if(hcsr04DistanceTemp != hcsr04Distance)hcsr04counter++;
        else hcsr04counter=0;

        //限幅消抖滤波法结束
    }
}

double HCSR04get_time()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec + tv.tv_usec * 1e-6;
}


#endif

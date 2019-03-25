#include<iostream>
#include<wiringPi.h>
using namespace std;

const int PWMpinX = 23;
const int PWMpinZ = 1;

void setup();

int main()
{
    setup();
    return 0;
}

void setup()
{
    if(-1==wiringPiSetup())
    {
        cerr<<"setup error\n";
        exit(-1);
    }
    pinMode(PWMpinX,OUTPUT);
    pinMode(PWMpinZ,OUTPUT);
}

#ifndef	__THREAD_HARDWARE_CONTROL_H
#define __THREAD_HARDWARE_CONTROL_H

#include <string.h>
#include <wiringPi.h>
#include <unistd.h>
#include "mongoose.h"
#include "opencv.hpp"
#include "thread_web_handler.h"
#include "thread_hcsr04.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "unistd.h"
#include "fcntl.h"


#include "flight_control_sample.hpp"

using namespace DJI::OSDK;
using namespace DJI::OSDK::Telemetry;


using namespace std;

extern "C"
{
  #include "MHTConsole.h"
}

//extern struct mg_connection *nc;
extern int hcsr04Distance;

char broadcastArray[100];
const int PWMpinX = 23;
const int PWMpinZ = 1;
const int SPEAKERpin = 7;

void PWMsetup();

void* threadHardwareControl(void*) {

  int ret=0;
  int humnanNum=0,humnanId=0,humnanState=0;
  int ox=0,oy=0,dx=0,dy=0;
  int pic_count = 0;

  float duration = 0.10;
  float elapsed  = 0;

  PWMsetup();sleep(1);

  ret = MHT_Init();
  if(ret != 0){
    fprintf(stderr, "\nInitialize Failed\n");
    return NULL;
  }

  /* init camera */
  CvCapture* pCapture = cvCreateCameraCapture(0);
  cvSetCaptureProperty(pCapture, CV_CAP_PROP_FRAME_WIDTH, 640);
  cvSetCaptureProperty(pCapture, CV_CAP_PROP_FRAME_HEIGHT, 480);
  cvSetCaptureProperty(pCapture, CV_CAP_PROP_BRIGHTNESS, 20);
  cvSetCaptureProperty(pCapture, CV_CAP_PROP_CONTRAST, 10);

  IplImage *pFrame = 0;

  if (NULL == pCapture) {
    fprintf(stderr, "Can't initialize webcam!\n");
    return NULL;
  }

  pFrame = cvQueryFrame(pCapture);  // query a frame

  if(NULL == pFrame) {
      fprintf(stderr, "Can't get a frame!\n" );
      return NULL;
  }

  /////////////////////////DJI//////////////////

  // Initialize variables
  int functionTimeout = 5;

  char *djiUserConfig = "./human_tracking_system ./UserConfig.txt";
  // Setup OSDK.
  LinuxSetup linuxEnvironment(2, &djiUserConfig);
  Vehicle*   vehicle = linuxEnvironment.getVehicle();
  
  if (vehicle == NULL)
  {
    std::cout << "Vehicle not initialized, exiting.\n";
    return NULL;
  }

  // Obtain Control Authority
  vehicle->obtainCtrlAuthority(functionTimeout);

  monitoredTakeoff(vehicle);

  Control::CtrlData command(0, 0, 0, 0, 0);
  command.flag &= 0x3F;
  command.flag |= Control::HorizontalLogic::HORIZONTAL_VELOCITY;  //水平
  command.flag &= 0xCF;
  command.flag |= Control::VerticalLogic::VERTICAL_VELOCITY;   //垂直
  command.flag &= 0xF7;
  command.flag |= Control::YawLogic::YAW_RATE;   //旋转速率
  command.flag &= 0xF9;
  command.flag |= Control::HorizontalCoordinate::HORIZONTAL_GROUND;   //相对于陆地
  command.flag &= 0xFE;
  command.flag |= Control::StableMode::STABLE_DISABLE;   //悬停不随风飘荡
  //command.flag |= Control::StableMode::STABLE_ENABLE;

  command.x = 0;command.y = 0;command.z = 0;command.yaw = 0;

  /*for(int highi=0;highi<3;highi++){
    while(hcsr04Distance<20)
    {
      std::cout << highi << hcsr04Distance << "init high\n";
      command.x = 0;command.y = 0;command.z = 0;command.yaw = 0;
      command.z = 3;
      while (elapsed < duration)
      {
            vehicle->control->flightCtrl(command);
            usleep(20000);
            elapsed += 0.02;
            if(hcsr04Distance>30)break;
      }elapsed=0;
      command.x = 0;command.y = 0;command.z = 0;command.yaw = 0;
    }
  }*/
  std::cout << "End init high\n";

  ///////////////////DJI Init /////////////////////////

  while(true){
    pFrame = cvQueryFrame(pCapture);  // query a frame

    if(NULL == pFrame) {
      fprintf(stderr, "Can't get a frame!\n" );
      monitoredLanding(vehicle);
      return NULL;
    }

    char temp[5];
    strcpy(broadcastArray,"./web-root/");
    sprintf(temp,"%d",pic_count);
    strcat(broadcastArray,temp);
    strcat(broadcastArray,".bmp");

    const char *pImageFileName = broadcastArray;
    cvSaveImage(pImageFileName, pFrame);

    /* Tracking */
    ret = MHT_Exec(pImageFileName,&humnanNum,&humnanId,&humnanState,&ox,&oy,&dx,&dy);
    if(ret != 0){
      fprintf(stderr, "MHT process failed\n");
    }else{
      if(humnanNum == 0){
        ox=oy=dx=dy=0;
        digitalWrite(SPEAKERpin,LOW);
      }else{
        //fprintf(stdout, "\nhumnanNum:%d,humnanId:%d,humnanState:%d\n",humnanNum,humnanId,humnanState);
        ox = ox+dx/2; oy = oy+dy/2;
        fprintf(stdout, "ox:%d oy:%d dx:%d dy:%d\n",ox,oy,dx,dy);
        digitalWrite(SPEAKERpin,HIGH);

        if(ox < 140)
        {
          command.x = 0;command.y = 0;command.z = 0;command.yaw = 0;
          command.y = 3;
          while (elapsed < duration)
          {
            vehicle->control->flightCtrl(command);
            usleep(20000);
            elapsed += 0.02;
          }elapsed=0;
          command.x = 0;command.y = 0;command.z = 0;command.yaw = 0;
        }else if(ox > 500){
          command.x = 0;command.y = 0;command.z = 0;command.yaw = 0;
          command.y = -3;
          while (elapsed < duration)
          {
            vehicle->control->flightCtrl(command);
            usleep(20000);
            elapsed += 0.02;
          }elapsed=0;
          command.x = 0;command.y = 0;command.z = 0;command.yaw = 0;
        }
      }
    }

    /*websocket pub*/
    if(pic_count%5==0){
      strcpy(broadcastArray,"{\"img_url\":\"");
      sprintf(temp,"%d",pic_count);
      strcat(broadcastArray,temp);
      strcat(broadcastArray,".bmp\"");
      strcat(broadcastArray,",\"ox\":\"");
      sprintf(temp,"%d",ox);
      strcat(broadcastArray,temp);
      strcat(broadcastArray,"\",\"oy\":\"");
      sprintf(temp,"%d",oy);
      strcat(broadcastArray,temp);
      strcat(broadcastArray,"\",\"dx\":\"");
      sprintf(temp,"%d",dx);
      strcat(broadcastArray,temp);
      strcat(broadcastArray,"\",\"dy\":\"");
      sprintf(temp,"%d",dy);
      strcat(broadcastArray,temp);
      strcat(broadcastArray,"\"}");
      broadcast(nc, mg_mk_str(broadcastArray));
    }
    pic_count++;if(pic_count>59)pic_count=0;
  }

  cvReleaseCapture(&pCapture);  // free memory
  MHT_Free();
  
  return NULL;
}

void PWMsetup()
{
    pinMode(PWMpinX,PWM_OUTPUT);
    pinMode(PWMpinZ,PWM_OUTPUT);
    pwmSetRange(2048);
    pwmSetClock(39);
    pwmSetMode(PWM_MODE_MS);
    pwmWrite(PWMpinZ,730);
    pwmWrite(PWMpinX,750);

    pinMode(SPEAKERpin,OUTPUT);
    digitalWrite(SPEAKERpin,LOW);
}

#endif

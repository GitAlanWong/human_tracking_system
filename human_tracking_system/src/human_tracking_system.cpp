#include <string>
#include "thread_hardware_control.h"
#include "thread_web_handler.h"
#include "thread_hcsr04.h"
//#include "thread_dji.h"
#include <wiringPi.h>

using namespace std;

int msqueue_record;
int main(int argc,char* argv[]) {

  if(-1==wiringPiSetup())
  {
    fprintf(stderr, "\nwiringPi setup error\n");
    return -1;
  }

  int createThreadRet = -1;
  /* create the web events handling thread */
  pthread_t webHandlerThreadId = 0;
  createThreadRet = pthread_create(&webHandlerThreadId, NULL, threadWebHandler, NULL);
  if (0 != createThreadRet) {
    return 1;
  }

  /* create the hardrware control thread */
  pthread_t hardwareControlThreadId = 0;
  createThreadRet = pthread_create(&hardwareControlThreadId, NULL, threadHardwareControl, NULL);
  if (0 != createThreadRet) {
    return 1;
  }

  /* create the hcsr04 thread */
  pthread_t hcsr04ThreadId = 0;
  createThreadRet = pthread_create(&hcsr04ThreadId, NULL, threadhcsr04, NULL);
  if (0 != createThreadRet) {
    return 1;
  }

  /* create the dji thread */
  /*pthread_t djiThreadId = 0;
  createThreadRet = pthread_create(&djiThreadId, NULL, threaddji, NULL);
  if (0 != createThreadRet) {
    return 1;
  }*/

  /* wait thread to finish */
  //pthread_join(djiThreadId, NULL);
  pthread_join(hcsr04ThreadId, NULL);
  pthread_join(webHandlerThreadId, NULL);
  pthread_join(hardwareControlThreadId, NULL);
  


  return 0;
}
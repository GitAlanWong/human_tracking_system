CC = gcc
CXX = g++

FLAG = -Wall
BASIC_INCLUDE_PATH = -I./include
CXXFLAGS = ${FLAG} \
           ${BASIC_INCLUDE_PATH} \
           ${BASIC_INCLUDE_PATH}/3rd \
           -I./MHTConsoleSample/include \
           $(MHTSUBDIR_INCLUDES) \
           -I/usr/local/include/opencv2/ `pkg-config --cflags opencv --libs opencv`
CFLAGS = ${FLAG} \
         ${BASIC_INCLUDE_PATH} \
         ${BASIC_INCLUDE_PATH}/3rd
LDFLAGS = -lpthread -lwiringPi -lm -lstdc++ -pg
C_SOURCE_FILE = ./src/3rd/mongoose.c
CXX_SOURCE_FILE = ./src/human_tracking_system.cpp 
C_OBJECT = ./src/3rd/mongoose.o


MHTCC = arm-linux-gnueabihf-gcc
MHTCXX = arm-linux-gnueabihf-g++
MHTTOPDIR = ./MHTConsoleSample
MHTTOPCONFDIR = ./MHTConsoleConf
MHTSRCPATH = $(MHTTOPDIR)/source
MHTHDRPATH = $(MHTTOPDIR)/include
MHTMHTHDRPATH = $(MHTTOPDIR)/libmht/include
MHTLICENSEPATH = $(MHTTOPDIR)/license
MHTLIB_ENV = armv7hf
MHTLINK_LIB = $(MHTTOPDIR)/libmht/lib/$(MHTLIB_ENV)/libMHT_V1181_ArmLinux.a
MHTLIB_TYPE = NEON_statictic
MHTOPTIMIZE = -DLINUX -fPIC -Wno-endif-labels -Wno-unused -Wno-comment -Wno-unused-parameter
MHTOPTIMIZE += -DARM -pg -march=armv7-a -mhard-float
MHTSUBDIR_INCLUDES = -I. -I$(MHTHDRPATH) -I$(MHTMHTHDRPATH) -I$(MHTLICENSEPATH)

DJI_CXXSOURCE_FILE = ./flight-control/flight_control_sample.cpp
DJI_CXXSOURCE_FILE_helpers = ./common/dji_linux_helpers.cpp
DJI_CXXSOURCE_FILE_environment = ./common/dji_linux_environment.cpp
DJI_CXXFLAGS = -std=c++11 -pthread -g -O0 -I/usr/local/include/djiosdk/ -I./common -I./flight-control  -ldjiosdk-core
DJI_CXXOBJECT = ./flight-control/flight_control_sample.o
DJI_CXXOBJECT_helpers = ./common/dji_linux_helpers.o
DJI_CXXOBJECT_environment = ./common/dji_linux_environment.o

.c.o:
	$(MHTCC) $(MHTOPTIMIZE) -c -o $(MHTSUBDIR_INCLUDES) -o $@ $<

#--------------------------------
# sample application source
#--------------------------------

MHTLIBSRC := $(MHTSRCPATH)/bitmaps.c
MHTLIBSRC += $(MHTSRCPATH)/MHTConsole.c
MHTOBJS =  ${MHTLIBSRC:%.c=%.o}

# compile the main executable program
human_tracking_system: $(MHTOBJS)
	-rm -f ./human_tracking_system
	$(CC) $(C_SOURCE_FILE) -c -o ${C_OBJECT} $(CFLAGS)
	$(CXX) $(DJI_CXXSOURCE_FILE) -c -o $(DJI_CXXOBJECT) $(DJI_CXXFLAGS)
	$(CXX) $(DJI_CXXSOURCE_FILE_helpers) -c -o $(DJI_CXXOBJECT_helpers) $(DJI_CXXFLAGS)
	$(CXX) $(DJI_CXXSOURCE_FILE_environment) -c -o $(DJI_CXXOBJECT_environment) $(DJI_CXXFLAGS)
	$(CXX) ${C_OBJECT} $(MHTOBJS) $(DJI_CXXOBJECT) $(DJI_CXXOBJECT_helpers) $(DJI_CXXOBJECT_environment) $(CXX_SOURCE_FILE) -o $@ $(MHTLINK_LIB) $(CXXFLAGS) ${LDFLAGS} $(DJI_CXXFLAGS)
	-rm -f $(MHTOBJS)

# deploy the project to a directory
install:
	rm -rf deploy
	mkdir deploy
	mv -f human_tracking_system deploy/
	cp -rf web-root deploy/
	-cp -f $(MHTTOPCONFDIR)/MHT_dict.bin ./deploy/
	-cp -f $(MHTTOPCONFDIR)/list.txt ./deploy/
	-cp -rf $(MHTTOPCONFDIR)/Data ./deploy/
	-cp -rf ./PinReset ./deploy/
	-cp -rf ./UserConfig.txt ./deploy/

.PHONY: clean

clean:
	-rm -f $(MHTOBJS) $(DJI_CXXOBJECT) ${C_OBJECT}
	rm -f human_tracking_system *.o ./src/*.o ./src/3rd/*.o
	rm -rf ./deploy

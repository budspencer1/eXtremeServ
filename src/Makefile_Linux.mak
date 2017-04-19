#######################################################################
#
# Project:                  XtremeServ Sauerbraten
# File Name:                Makefile_Linux.mak
# Author:                   BudSpencer
# Date:                     05.01.2017
#
# Purpose:                  compile XtremeServ with gcc compiler
# Architecture (x->y):      Linux XX Bit ---> Linux XX Bit
#
#######################################################################
ENET_USE_SYSTEM_LIBS=false

### Tools
CXX=gcc
MV=mv
STRIP=strip


PLATFORM=$(shell uname -s)

# Use clang on freebsd
# Comment next lines if server not building or you want to use old GCC
ifneq (,$(findstring FreeBSD,$(PLATFORM)))
CXX=clang
endif

#result file name
PLATFORM_SUFFIX=
ifneq (,$(findstring x86_64,$(shell uname -a)))
PLATFORM_SUFFIX=64
endif
ifneq (,$(findstring amd64,$(shell uname -a)))
PLATFORM_SUFFIX=64
endif

SERVER_PREFIX=eXtremeServer
SERVER_NAME=$(SERVER_PREFIX)
ifneq (,$(findstring MINGW,$(PLATFORM)))
SERVER_NAME=$(SERVER_NAME)$(PLATFORM_SUFFIX).exe
endif
ifneq (,$(findstring Linux,$(PLATFORM)))
SERVER_NAME=$(SERVER_PREFIX)$(PLATFORM_SUFFIX)
endif
ifneq (,$(findstring FreeBSD,$(PLATFORM)))
SERVER_NAME=$(SERVER_PREFIX)_freebsd$(PLATFORM_SUFFIX)
endif

### Directories, libraries, includes
ifneq (,$(findstring MINGW,$(PLATFORM)))
SERVER_INCLUDES+= -DSTANDALONE $(INCLUDES) -Iinclude
SERVER_LIBS= -Llib -lzdll -lenet -lws2_32 -lwinmm -lstdc++
else
SERVER_INCLUDES+= -DSTANDALONE $(INCLUDES)
SERVER_LIBS= -L/usr/local/lib -lenet -lz -lstdc++ -lm
endif

ifeq ($(PLATFORM),SunOS)
SERVER_LIBS+= -lsocket -lnsl
endif

CXXFLAGS=-fomit-frame-pointer -Wall -fsigned-char -DSTANDALONE

INCLUDES= -Ishared -Iengine -Ifpsgame -Iextremeserver

### Embended ENET
ifeq ($(ENET_USE_SYSTEM_LIBS),true)
else
override INCLUDES += -Ienet/include
override SERVER_LIBS += -Lenet/.libs
endif

SERVER_OBJS= \
	shared/crypto-standalone.o \
	shared/stream-standalone.o \
	shared/tools-standalone.o \
	engine/worldio-standalone.o \
	engine/command-standalone.o \
	engine/server-standalone.o \
	fpsgame/entities-standalone.o \
	fpsgame/server-standalone.o \
	extremeserver/antiflood-standalone.o \
	extremeserver/cmdhandler-standalone.o \
	extremeserver/eventhandler-standalone.o 
	

default: release

release: release-flags 
release-flags: override CXXFLAGS+=-O2 -s
release-flags: build


debug: debug-flags 
debug-flags: override CXXFLAGS+=-O0 -g -DDEBUG -DDEBUG_SQL
debug-flags: build

build: revision server

revision:
SVNVERSION= $(shell svnversion -cn . 2>/dev/null | sed -e "s/.*://" -e "s/\([0-9]*\).*/\1/" | grep "[0-9]") 
ifneq "$(SVNVERSION)" " "
override CXXFLAGS+= -DREMOD_VERSION="\"SVN build rev: $(SVNVERSION)\""
endif

enet/Makefile:
	cd enet; autoreconf -vfi; ./configure --enable-shared=no --enable-static=yes
	
libenet: enet/Makefile
	$(MAKE)	-C enet/ all

clean-enet: enet/Makefile
	$(MAKE) -C enet/ maintainer-clean

geoip/Makefile:
	cd mod/GeoIP; chmod +x configure; ./configure --enable-shared=no --enable-static=yes

libgeoip: geoip/Makefile
	$(MAKE)	-C mod/GeoIP/libGeoIP/ all

clean-geoip: geoip/Makefile
	$(MAKE) -C mod/GeoIP/libGeoIP/ maintainer-clean

clean: clean-enet
	-$(RM) $(SERVER_OBJS) 

%.h.gch: %.h
	$(CXX) $(CXXFLAGS) -o $@.tmp $(subst .h.gch,.h,$@)
	$(MV) $@.tmp $@

%-standalone.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $(subst -standalone.o,.cpp,$@) 

%-standalone.o: %.c
	$(CXX) $(CXXFLAGS) -c -o $@ $(subst -standalone.o,.c,$@)	


$(SERVER_OBJS): CXXFLAGS += $(SERVER_INCLUDES)

ifneq (,$(findstring MINGW,$(PLATFORM)))
server: $(SERVER_OBJS)
	$(CXX) $(CXXFLAGS) -o $(SERVER_NAME) $(SERVER_OBJS) $(SERVER_LIBS)

else
ifeq ($(ENET_USE_SYSTEM_LIBS),true)
server:	$(SERVER_OBJS)
	$(CXX) $(CXXFLAGS) -o $(SERVER_NAME) $(SERVER_OBJS) $(SERVER_LIBS)  
	$(MV) $(SERVER_NAME) ../$(SERVER_NAME)
else
server:	libenet $(SERVER_OBJS)
	$(CXX) $(CXXFLAGS) -o $(SERVER_NAME) $(SERVER_OBJS) $(SERVER_LIBS)  
	$(MV) $(SERVER_NAME) ../$(SERVER_NAME)
endif
endif

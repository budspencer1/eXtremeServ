#######################################################################
#
# Project:                  XtremeServ Sauerbraten
# File Name:                Makefile_Windows.mak
# Author:                   BudSpencer
# Date:                     04.01.2017
#
# Purpose:                  compile XtremeServ with i686-w64-mingw32-gcc compiler
# Architecture (x->y):      Linux XX Bit ---> Windows 32 Bit
#
#######################################################################
ENET_USE_SYSTEM_LIBS=false

#tools used for compiling
CXX=i686-w64-mingw32-gcc 
MV=mv                                                                           
WINDRES=i686-w64-mingw32-windres
SERVER_NAME=XtremeServ.exe
CXXFLAGS= -fpermissive -fomit-frame-pointer -Wall -fsigned-char -DSTANDALONE -static -Wl,-subsystem,windows
SERVER_INCLUDES= -I/usr/i686-w64-mingw32/include/ $(INCLUDES)  
SERVER_LIBS= -lz.dll -lgcc -lstdc+
INCLUDES= -Ishared -Iengine -Ifpsgame+
#override CXXFLAGS+= -O0 -g -DDEBUG # uncomment for debugging

ifeq (,$(findstring -g,$(CXXFLAGS)))
ifeq (,$(findstring -pg,$(CXXFLAGS)))
  STRIP=strip
endif
endif

#includes

SERVER_LIBS= -lz.dll -lgcc -lstdc++
INCLUDES= -Ishared -Iengine -Ifpsgame+

ifeq ($(ENET_USE_SYSTEM_LIBS),true)
else
override INCLUDES += -Ienet/include
override SERVER_LIBS += -Lenet/.libs -lenet -lws2_32 -lwinmm
endif

SERVER_OBJS= \
	shared/crypto-standalone.o \
	shared/stream-standalone.o \
	shared/tools-standalone.o \
	engine/worldio-standalone.o \
	engine/command-standalone.o \
	engine/server-standalone.o \
	fpsgame/entities-standalone.o \
	fpsgame/server-standalone.o



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
#override CXXFLAGS+= -DREMOD_VERSION="\"SVN build rev: $(SVNVERSION)\""
endif

enet/Makefile:
	cd enet; autoreconf -vfi; ./configure --enable-shared=no --enable-static=yes --host=i686-w64-mingw32
	
libenet: enet/Makefile
	$(MAKE)	-C enet/ all

clean-enet: enet/Makefile
	$(MAKE) -C enet/ maintainer-clean

geoip/Makefile:
	cd mod/libGeoIP; chmod +x configure; ac_cv_func_malloc_0_nonnull=yes ac_cv_func_realloc_0_nonnull=yes ac_cv_func_pread=yes ./configure --enable-shared=no --enable-static=yes --host=i686-w64-mingw32

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

server: libenet $(SERVER_OBJS)
	$(WINDRES) -I res -i res/Windows.rc -J rc -o res/XtremeServ.res -O coff
	$(CXX) $(CXXFLAGS) -o $(SERVER_NAME) res/XtremeServ.res $(SERVER_OBJS) $(SERVER_LIBS)
	$(MV) $(SERVER_NAME) ../$(SERVER_NAME)
